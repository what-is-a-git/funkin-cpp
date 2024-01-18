#pragma once

#include <stdio.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <glm/glm.hpp>

#include "core/audio/audio.h"

namespace crystal {
    class AudioServer {
        private:
            static ALCdevice *_device;
            static ALCcontext *_context;

            static glm::dvec3 _position;
            static glm::dvec3 _velocity;
            static glm::mat2x3 _orientation;

            static double _volume;
        public:
            AudioServer() = delete;

            static void init(void);
            static void dispose(void);

            static glm::dvec3 get_position(void);
            static void set_position(glm::dvec3 position);

            static glm::dvec3 get_velocity(void);
            static void set_velocity(glm::dvec3 velocity);

            static glm::mat2x3 get_orientation(void);
            static void set_orientation(glm::mat2x3 orientation);

            static double get_volume(void);
            static void set_volume(double volume);
    };
}