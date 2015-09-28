#include <cstring>

#include "filesystem/file.h"
#include "filesystem/filesystem.h"

#include "common/memory/memory.h"

#include "os/os.h"

namespace cetech {
    class DiskFile : public File {
        public:
            os::File _file;

            DiskFile(const char* path, File::OpenMode mode) {
                _file = os::file::from_file(path, mode == File::WRITE ? "w" : "r");
            }

            virtual ~DiskFile() {
                os::file::close(_file);
            }

            virtual bool is_valid() {
                return !os::file::is_null(_file);
            }

            virtual void seek(size_t position) final {
                os::file::seek(_file, position, os::file::SW_SEEK_SET);
            };

            virtual void seek_to_end() final {
                os::file::seek(_file, 0, os::file::SW_SEEK_END);
            };

            virtual void skip(size_t bytes) final {
                os::file::seek(_file, bytes, os::file::SW_SEEK_CUR);
            };

            virtual void read(void* buffer, size_t size) final {
                os::file::read(_file, buffer, sizeof(char), size);
            };

            virtual void write(const void* buffer, size_t size)  final {
                os::file::write(_file, buffer, sizeof(char), size);
            };

            virtual void flush() final {};

            virtual bool end_of_file()  final {
                return position() == size();
            };

            virtual size_t size()  final {
                const size_t curent_pos = position();
                seek_to_end();
                const size_t sz = position();
                seek(curent_pos);

                return sz;
            };

            virtual size_t position() final {
                return os::file::tell(_file);
            };
    };

    class DiskFileSystem : public FileSystem {
        public:
            char _root_path[1024];
            Allocator& _allocator;

            DiskFileSystem(const char* root_path, Allocator & allocator) : _root_path({0}), _allocator(allocator) {
                strcpy(_root_path, root_path);

                const size_t len = strlen(_root_path);
                if (_root_path[len - 1] != '/') {
                    _root_path[len] = '/';
                }
            }

            virtual File* open(const char* path, File::OpenMode mode) final {
                char abs_path[2048] = {0};
                absolute_path(abs_path, path);

                return MAKE_NEW(_allocator, DiskFile, abs_path, mode);
            }

            virtual void close(File* file) final {
                MAKE_DELETE(_allocator, File, file);
            };

            virtual bool exists(const char* path) final {/*TODO:*/
            };

            virtual bool is_directory(const char* path) final {/*TODO:*/
            };

            virtual bool is_file(const char* path)  final {/*TODO:*/
            };

            virtual void create_directory(const char* path)  final {
                os::dir::mkpath(path);
            };

            virtual void delete_directory(const char* path) final {/*TODO:*/
            };

            virtual void list_directory ( const char* path, cetech::Array < char* >& files ) final {
                os::dir::listdir(path, files);
            }

            virtual void create_file(const char* path)  final {/*TODO:*/
            };

            virtual void delete_file(const char* path)  final {/*TODO:*/
            };

            void absolute_path(char* buffer, const char* path) {
                std::sprintf(buffer, "%s%s", _root_path, path);
            }
    };

    namespace disk_filesystem {
        FileSystem* make(Allocator& alocator, const char* root_path) {
            return MAKE_NEW(alocator, DiskFileSystem, root_path, alocator);
        }

        void destroy(Allocator& alocator, FileSystem* fs) {
            MAKE_DELETE(alocator, FileSystem, fs);
        }
    }
}