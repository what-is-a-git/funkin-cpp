#pragma once

#include <list>
#include <glm/glm.hpp>

namespace crystal {
    typedef struct _sparrow_frame {
        const glm::ivec4 source_rect, offset_rect;
        char *name;
    } sparrow_frame;

    // mostly singleton class because we don't really need it not to be for now :3 (or ever most likely)
    class Sparrow {
        public:
            Sparrow() = delete;

            static sparrow_frame get_frame(const std::list<sparrow_frame> frames, const char *name);
            static void free_frame_names(std::list<sparrow_frame> frames);
            static std::list<sparrow_frame> load_from_path(const char *path);
    };
}