#include "device.h"

#include <unistd.h>

#include "common/memory/memory.h"
#include "common/lua/lua_env.h"
#include "common/command_line/command_line.h"
#include "resource_manager.h"
#include "package_manager.h"
#include "common/log/handlers.h"
#include "develop_manager.h"

#include "resources/package.h"

#include "cvars/cvars.h"
#include "runtime/runtime.h"

namespace cetech {
    class DeviceImplementation : public Device {
        friend class Device;

        uint32_t frame_id;
        uint32_t last_frame_ticks;
        float delta_time;
        ResourceManager* resource_manager_;
        PackageManager* package_manager_;
        DevelopManager* develop_manager_;
        ConsoleServer* console_server_;


        virtual float get_delta_time() {
            return this->delta_time;
        }
        virtual uint32_t get_frame_id() {
            return this->frame_id;
        }

        virtual void init(int argc, const char** argv) {
            command_line_globals::set_args(argc, argv);

            log::init();
            log::register_handler(&log_handlers::stdout_handler);

            develop_manager_ = DevelopManager::make(memory_globals::default_allocator());
            parse_command_line();

            runtime::init();

            resource_manager_ = ResourceManager::make(memory_globals::default_allocator());
            package_manager_ = PackageManager::make(memory_globals::default_allocator());
            console_server_ = ConsoleServer::make(memory_globals::default_allocator());

            load_config_json();

            console_server_->register_command("lua.execute", &cmd_lua_execute);

            log::register_handler(&log_handlers::console_server_handler);

            register_resources();

            if (command_line_globals::has_argument("compile", 'c')) {
                compile_all_resource();
            }

            init_boot();

            this->last_frame_ticks = runtime::get_ticks();
        }

        virtual void shutdown() {
            PackageManager::destroy(memory_globals::default_allocator(), package_manager_);
            ResourceManager::destroy(memory_globals::default_allocator(), resource_manager_);
            DevelopManager::destroy(memory_globals::default_allocator(), develop_manager_);
            ConsoleServer::destroy(memory_globals::default_allocator(), console_server_);

            runtime::shutdown();
            //memory_globals::shutdown();
        }

        virtual void run() {
            if (command_line_globals::has_argument("--wait", 'w')) {
                log::info("main", "Wating for clients.");
                while (!console_server_->has_clients()) {
                    console_server_->tick();
                }

                log::debug("main", "Client connected.");
            }

            float dt = 0.0f;
            while (1) {
                develop_manager_->push_begin_frame();

                uint32_t now_ticks = runtime::get_ticks();
                dt = (now_ticks - this->last_frame_ticks) * 0.001f;
                this->delta_time = dt;
                this->last_frame_ticks = now_ticks;

                develop_manager_->push_record_float("engine.delta_time", dt);
                develop_manager_->push_record_float("engine.frame_rate", 1.0f / dt);

                runtime::frame_start();
                console_server_->tick();
                //

                usleep(3 * 1000);

                //
                runtime::frame_end();
                develop_manager_->push_end_frame();
                develop_manager_->send_buffer();
                develop_manager_->clear();
                ++(this->frame_id);
            }
        }

        virtual ResourceManager& resource_manager() {
            return *(this->resource_manager_);
        }

        virtual PackageManager& package_manager() {
            return *(this->package_manager_);
        }

        virtual DevelopManager& develop_manager() {
            return *(this->develop_manager_);
        }

        virtual ConsoleServer& console_server() {
            return *(this->console_server_);
        }

        CE_INLINE void register_resources() {
            struct ResourceRegistration {
                StringId64_t type;

                ResourceManager::resource_compiler_clb_t compiler;
                ResourceManager::resource_loader_clb_t loader;
                ResourceManager::resource_unloader_clb_t unloader;
            };

            static ResourceRegistration resource_regs[] = {
                /* package */
                {resource_package::type_hash(), & resource_package::compiler, & resource_package::loader,
                 & resource_package::unloader},

                /* lua */
                {resource_lua::type_hash(), & resource_lua::compiler, & resource_lua::loader, & resource_lua::unloader},

                /* LAST */
                {0, nullptr, nullptr, nullptr}
            };

            const ResourceRegistration* it = resource_regs;
            while (it->type != 0) {
                resource_manager_->register_unloader(it->type, it->unloader);
                resource_manager_->register_loader(it->type, it->loader);
                resource_manager_->register_compiler(it->type, it->compiler);
                ++it;
            }
        }

        static void cmd_lua_execute(const rapidjson::Document& in, rapidjson::Document& out) {
            lua_enviroment::execute_string(lua_enviroment_globals::global_env(), in["args"]["script"].GetString());
        }

        void join_build_dir(char* buffer, size_t max_len, const char* basename) {
            memset(buffer, 0, max_len);

            size_t len = strlen(cvars::rm_build_dir.value_str);
            memcpy(buffer, cvars::rm_build_dir.value_str, len);

            strcpy(buffer + len, basename);
        }

        void load_config_json() {
            char config_path[1024] = {0};
            join_build_dir(config_path, 1024, "config.json");

            File f = runtime::file::from_file(config_path, "rb");
            const uint64_t f_sz = runtime::file::size(f);
            void* mem = memory_globals::default_allocator().allocate(f_sz + 1);
            memset(mem, 0, f_sz + 1);

            runtime::file::read(f, mem, sizeof(char), f_sz);

            rapidjson::Document document;
            document.Parse((const char*)mem);
            cvar::load_from_json(document);

            memory_globals::default_allocator().deallocate(mem);

            runtime::file::close(f);
        }

        void make_path(char* buffer, size_t max_size, const char* path) {
            memset(buffer, 0, max_size);
            strcpy(buffer, path);

            const size_t len = strlen(buffer);
            if (buffer[len - 1] != '/') {
                buffer[len] = '/';
            }
        }

        void parse_command_line() {
            char buffer[1024] = {0};

            const char* source_dir = command_line_globals::get_parameter("source-dir", 'i');
            const char* build_dir = command_line_globals::get_parameter("build-dir", 'd');

            if (source_dir) {
                make_path(buffer, 1024, source_dir);
                cvar_internal::force_set(cvars::rm_source_dir, buffer);
            }

            if (build_dir) {
                make_path(buffer, 1024, build_dir);
                cvar_internal::force_set(cvars::rm_build_dir, buffer);
            }
        }

        void init_boot() {
            StringId64_t boot_pkg_name_h = murmur_hash_64(cvars::boot_pkg.value_str, strlen(
                                                              cvars::boot_pkg.value_str), 22);
            StringId64_t boot_script_name_h =
                murmur_hash_64(cvars::boot_script.value_str, strlen(cvars::boot_script.value_str), 22);

            resource_manager_->load(resource_package::type_hash(), &boot_pkg_name_h, 1);
            package_manager_->load(boot_pkg_name_h);

            StringId64_t lua_hash = murmur_hash_64("lua", 3, 22);

            const resource_lua::Resource* res_lua = (const resource_lua::Resource*)resource_manager_->get(lua_hash,
                                                                                                          boot_script_name_h);
            lua_enviroment::execute_resource(lua_enviroment_globals::global_env(), res_lua);
        }

        void compile_all_resource() {
            char* files[4096] = {0};
            uint32_t files_count = 0;
            const size_t source_dir_len = cvars::rm_source_dir.str_len;

            runtime::dir::listdir(cvars::rm_source_dir.value_str, "", files, &files_count);

            for (uint32_t i = 0; i < files_count; ++i) {
                const char* path_base = files[i] + source_dir_len; /* Base path */

                resource_manager_->compile(path_base);
            }
        }
    };

    Device* Device::make(Allocator& alocator) {
        return MAKE_NEW(alocator, DeviceImplementation);
    }

    void Device::destroy(Allocator& alocator, Device* rm) {
        MAKE_DELETE(memory_globals::default_allocator(), Device, rm);
    }

    namespace device_globals {
        static Device* _device = nullptr;

        void init() {
            _device = Device::make(memory_globals::default_allocator());
        }

        void shutdown() {
            Device::destroy(memory_globals::default_allocator(), _device);
            _device = nullptr;
        }

        Device& device() {
            return *_device;
        }
    }
}