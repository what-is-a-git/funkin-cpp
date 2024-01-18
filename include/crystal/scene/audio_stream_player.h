#pragma once

#include <glm/glm.hpp>

#include <AL/al.h>

#include "assets/asset_server.h"

#include "core/audio/audio.h"
#include "core/audio/audio_stream.h"

namespace crystal {
    class AudioStreamPlayer {
        private:
            unsigned int _source;

            glm::vec3 _position;
            glm::vec3 _velocity;

            double _pitch;
            double _volume;

            double _time;

            bool _looping;
        public:
            AudioStream *stream;

            AudioStreamPlayer();
            ~AudioStreamPlayer();

            void set_stream(AudioStream *audio_stream);

            glm::vec3 get_position(void);
            void set_position(glm::vec3 position);

            glm::vec3 get_velocity(void);
            void set_velocity(glm::vec3 velocity);

            double get_pitch(void);
            void set_pitch(double pitch);

            double get_volume(void);
            void set_volume(double volume);

            bool get_looping(void);
            void set_looping(bool looping);

            double get_time_seconds(void);
            void set_time_seconds(double time_seconds);

            double get_duration_seconds(void);

            bool get_playing(void);
            void set_playing(bool playing);

            void play(void);
            void play(double start_offset_seconds);

            void pause(void);
            void stop(void);

            void step(void);
            void step(const double delta);
    };
}