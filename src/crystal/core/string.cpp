#include "core/string.h"

#include <stdio.h>

namespace crystal {
    char *string_get_extension(const char *input) {
        char *output;
        size_t input_length = strlen(input);
        size_t last_period = 0;
        size_t index = 0;

        while (index < input_length) {
            if (input[index] == '.') {
                last_period = index;
            }

            index++;
        }

        if (last_period == 0) {
            return NULL;
        }

        size_t output_size = input_length - (last_period + 1) + 1;
        size_t output_index = 0;

        output = (char*) malloc(sizeof(char) * output_size);
        index = last_period + 1;

        while (index < input_length) {
            output[output_index] = input[index];

            index++;
            output_index++;
        }

        output[output_size - 1] = '\0';
        return output;
    }
}