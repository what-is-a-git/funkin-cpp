#include "core/audio/audio_stream.h"

#include <stdlib.h>

#include <AL/al.h>
#include <stb/stb_vorbis.h>

namespace crystal {
    AudioStream::AudioStream(const char *path) {
        alCheck(alGenBuffers(AUDIO_STREAM_BUFFER_COUNT, &buffers[0]));

        int error;
        _native_data = stb_vorbis_open_filename(path, &error, NULL);
        _native_info = stb_vorbis_get_info(_native_data);

        if (_native_info.channels == 1) {
            _format = AL_FORMAT_MONO16;
        } else {
            _format = AL_FORMAT_STEREO16;
        }

        buffer_count = AUDIO_STREAM_BUFFER_COUNT;
    }

    AudioStream::~AudioStream() {
        // Hopefully you're not still using these!
        for (int i = 0; i < AUDIO_STREAM_BUFFER_COUNT; i++) {
            alDeleteBuffers(AUDIO_STREAM_BUFFER_COUNT, &buffers[0]);
        }
    }

    void AudioStream::init_buffer_data(const unsigned int source) {
        for (int i = 0; i < AUDIO_STREAM_BUFFER_COUNT; i++) {
            short *buffer = (short*)malloc(sizeof(short) * AUDIO_STREAM_BUFFER_SIZE);
            int frame = stb_vorbis_get_samples_short_interleaved(_native_data, _native_info.channels, buffer, AUDIO_STREAM_BUFFER_SIZE / sizeof(short));

            alCheck(alBufferData(buffers[i], _format, buffer, AUDIO_STREAM_BUFFER_SIZE, _native_info.sample_rate));
            free(buffer);

            if (frame == 0) {
                buffer_count--;
                alCheck(alDeleteBuffers(1, &buffers[i]));
            }
        }

        alCheck(alSourceQueueBuffers(source, buffer_count, &buffers[0]));
    }

    void AudioStream::buffer_data(const unsigned int source) {
        unsigned int buffer;
        alCheck(alSourceUnqueueBuffers(source, 1, &buffer));

        short *vorbis_buffer = (short*)malloc(sizeof(short) * AUDIO_STREAM_BUFFER_SIZE);
        int frame = stb_vorbis_get_samples_short_interleaved(_native_data, _native_info.channels, vorbis_buffer, AUDIO_STREAM_BUFFER_SIZE / sizeof(short));

        alCheck(alBufferData(buffer, _format, vorbis_buffer, AUDIO_STREAM_BUFFER_SIZE, _native_info.sample_rate));
        free(vorbis_buffer);

        if (frame > 0) {
            alCheck(alSourceQueueBuffers(source, 1, &buffer));
        }
    }

    float AudioStream::get_duration_seconds(void) {
        return stb_vorbis_stream_length_in_seconds(_native_data);
    }

    AudioStream *AudioStream::get_from_file(const char *path) {
        return new AudioStream(path);
    }
}