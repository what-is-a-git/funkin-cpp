#pragma once

#include <unordered_map>
#include <string>

#include "scene/audio_stream_player.h"

namespace funkin {
    class Audio {
        public:
            static std::unordered_map<std::string, crystal::AudioStreamPlayer*> streams;

            Audio() = delete;

            static void step(const double delta);

            static crystal::AudioStreamPlayer *get_stream(std::string key);
            static void dispose_stream(std::string key);
    };
}