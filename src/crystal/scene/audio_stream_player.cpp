#include "scene/audio_stream_player.h"

namespace crystal {
    AudioStreamPlayer::AudioStreamPlayer() {
        alCheck(alGenSources(1, &_source));
        set_position(glm::vec3(1.0, 0.0, 0.0));
        set_velocity(glm::vec3(0.0));
        set_pitch(1.0);
        set_volume(1.0);
        set_looping(false);
        alCheck(alSourcei(_source, AL_LOOPING, AL_FALSE));
        _time = 0.0;
    }

    AudioStreamPlayer::~AudioStreamPlayer() {
        alCheck(alDeleteSources(1, &_source));
    }

    void AudioStreamPlayer::set_stream(AudioStream *audio_stream) {
        stream = audio_stream;
        stream->init_buffer_data(_source);
    }

    glm::vec3 AudioStreamPlayer::get_position(void) {
        return _position;
    }

    void AudioStreamPlayer::set_position(glm::vec3 position) {
        _position = position;
        alCheck(alSource3f(_source, AL_POSITION, position.x, position.y, position.z));
    }

    glm::vec3 AudioStreamPlayer::get_velocity(void) {
        return _velocity;
    }

    void AudioStreamPlayer::set_velocity(glm::vec3 velocity) {
        _velocity = velocity;
        alCheck(alSource3f(_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
    }

    double AudioStreamPlayer::get_pitch(void) {
        return _pitch;
    }

    void AudioStreamPlayer::set_pitch(double pitch) {
        _pitch = pitch;
        alCheck(alSourcef(_source, AL_PITCH, pitch));
    }

    double AudioStreamPlayer::get_volume(void) {
        return _volume;
    }

    void AudioStreamPlayer::set_volume(double volume) {
        _volume = volume;
        alCheck(alSourcef(_source, AL_GAIN, volume));
    }

    bool AudioStreamPlayer::get_looping(void) {
        return _looping;
    }

    void AudioStreamPlayer::set_looping(bool looping) {
        _looping = looping;
    }

    double AudioStreamPlayer::get_time_seconds(void) {
        return _time;
    }

    void AudioStreamPlayer::set_time_seconds(double time_seconds) {
        bool _playing = get_playing();
        _time = time_seconds;

        stb_vorbis_seek(stream->_native_data, stream->_native_info.sample_rate * time_seconds);

        int buffers_queued;
        alCheck(alGetSourcei(_source, AL_BUFFERS_QUEUED, &buffers_queued));

        if (_playing) {
            stop();
        }

        while (buffers_queued--) {
            unsigned int buffer;
            alCheck(alSourceUnqueueBuffers(_source, 1, &buffer));
        }

        stream->init_buffer_data(_source);

        if (_playing) {
            play();
        }
    }

    double AudioStreamPlayer::get_duration_seconds(void) {
        return stream->get_duration_seconds();
    }

    bool AudioStreamPlayer::get_playing(void) {
        int state;
        alCheck(alGetSourcei(_source, AL_SOURCE_STATE, &state));
        return state == AL_PLAYING;
    }

    void AudioStreamPlayer::set_playing(bool playing) {
        alCheck(alSourcei(_source, AL_SOURCE_STATE, playing ? AL_PLAYING : AL_STOPPED));
    }

    void AudioStreamPlayer::play(void) {
        alCheck(alSourcePlay(_source));
    }

    void AudioStreamPlayer::play(double start_offset_seconds) {
        set_time_seconds(start_offset_seconds);
        alCheck(alSourcePlay(_source));
    }

    void AudioStreamPlayer::pause(void) {
        alCheck(alSourcePause(_source));
    }

    void AudioStreamPlayer::stop(void) {
        alCheck(alSourceStop(_source));
    }
    
    void AudioStreamPlayer::step(void) {
        int state;
        alCheck(alGetSourcei(_source, AL_SOURCE_STATE, &state));

        bool is_playing = state == AL_PLAYING;

        if (_looping && !is_playing) {
            play(0.0f);
        }

        if (stream->buffer_count < 4) {
            return;
        }

        int buffers_processed = 0;
        alCheck(alGetSourcei(_source, AL_BUFFERS_PROCESSED, &buffers_processed));

        if (buffers_processed <= 0) {
            return;
        }

        while (buffers_processed--) {
            stream->buffer_data(_source);
        }
    }

    void AudioStreamPlayer::step(const double delta) {
        step();
        
        if (get_playing()) {
            _time += delta * _pitch;
        }
    }
}