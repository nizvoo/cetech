#pragma once

#include <cinttypes>

#include "common/string/stringid_types.h"
#include "common/memory/memory_types.h"

namespace cetech {
    class PackageManager {
        public:
            virtual ~PackageManager() {}

            virtual void load(StringId64_t name) = 0;
            virtual void unload(StringId64_t name) = 0;

            virtual bool is_loaded(StringId64_t name) = 0;
            virtual void flush(StringId64_t name) = 0;

            static PackageManager* make(Allocator& alocator);
            static void destroy(Allocator& alocator, PackageManager* pm);
    };
}