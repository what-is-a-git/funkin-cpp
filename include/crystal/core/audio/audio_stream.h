#pragma once

#include <stb/stb_vorbis.h>

#include "core/audio/audio.h"

#define AUDIO_STREAM_BUFFER_COUNT 4
#define AUDIO_STREAM_BUFFER_SIZE 65536

namespace crystal {
    class AudioStream {
        public:
            // OpenAL
            int _format;

            // stb_vorbis
            stb_vorbis *_native_data;
            stb_vorbis_info _native_info;

            unsigned int buffers[AUDIO_STREAM_BUFFER_COUNT];
            int buffer_count;

            AudioStream(const char *path);
            ~AudioStream();

            void init_buffer_data(const unsigned int source);
            void buffer_data(const unsigned int source);

            float get_duration_seconds(void);

            static AudioStream *get_from_file(const char *path);
    };
}