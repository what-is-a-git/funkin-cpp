#include "core/audio/audio_server.h"

#include <glm/gtc/type_ptr.hpp>

namespace crystal {
    ALCdevice *AudioServer::_device;
    ALCcontext *AudioServer::_context;

    glm::dvec3 AudioServer::_position;
    glm::dvec3 AudioServer::_velocity;
    glm::mat2x3 AudioServer::_orientation;

    double AudioServer::_volume;

    void AudioServer::init(void) {
        // AL init
        const char *device_string = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		_device = alcOpenDevice(device_string);

		if (!_device) {
			fprintf(stderr, "Failed to get default OpenAL device.\n");
			exit(1);
		}

		// Make audio context
		_context = alcCreateContext(_device, nullptr);

		// Activate context
		if (!alcMakeContextCurrent(_context)) {
			fprintf(stderr, "Failed to make OpenAL context active to the current device.\n");
			exit(1);
		}

		// Create audio listener
        set_position(glm::dvec3(0.0));
        set_velocity(glm::dvec3(0.0));
        set_orientation(glm::mat2x3(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        ));
    }

    void AudioServer::dispose(void) {
        alcDestroyContext(_context);
        alcCloseDevice(_device);
    }

    glm::dvec3 AudioServer::get_position(void) {
        return _position;
    }

    void AudioServer::set_position(glm::dvec3 position) {
        _position = position;
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }

    glm::dvec3 AudioServer::get_velocity(void) {
        return _velocity;
    }

    void AudioServer::set_velocity(glm::dvec3 velocity) {
        _velocity = velocity;
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    glm::mat2x3 AudioServer::get_orientation(void) {
        return _orientation;
    }

    void AudioServer::set_orientation(glm::mat2x3 orientation) {
        _orientation = orientation;
        alListenerfv(AL_ORIENTATION, glm::value_ptr(orientation));
    }

    double AudioServer::get_volume(void) {
        return _volume;
    }

    void AudioServer::set_volume(double volume) {
        _volume = volume;
        alListenerf(AL_GAIN, volume);
    }
}