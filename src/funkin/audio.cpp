#include "funkin/audio.h"

namespace funkin {
    std::unordered_map<std::string, crystal::AudioStreamPlayer*> Audio::streams;

    crystal::AudioStreamPlayer *Audio::get_stream(std::string key) {
        if (streams.count(key) != 0) {
            return streams[key];
        }

        return NULL;
    }

    void Audio::dispose_stream(std::string key) {
        if (streams.count(key) == 0) {
            return;
        }

        crystal::AudioStreamPlayer *stream = streams[key];
        streams.erase(key);
        delete stream;
    }

    void Audio::step(const double delta) {
        for (std::pair<std::string, crystal::AudioStreamPlayer*> pair : streams) {
            pair.second->step(delta);
        }
    }
}