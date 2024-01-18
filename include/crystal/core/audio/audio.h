#pragma once

// AL Error Macros for easier error checking
#ifndef alCheck
#define _alError(message) {\
	ALenum error = alGetError();\
	if (error != AL_NO_ERROR) {\
        fprintf(stderr, "OpenAL Error: %i, when calling %s.\n", error, #message);\
	}\
}

#define alCheck(FUNCTION_CALL)\
FUNCTION_CALL;\
_alError(FUNCTION_CALL);
#endif