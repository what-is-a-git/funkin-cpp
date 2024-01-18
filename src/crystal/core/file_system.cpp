#include <stdio.h>
#include <stdlib.h>

#include "core/file_system.h"

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

namespace crystal {
    char *FileSystem::get_file_contents(const char *path) {
        // More "efficient" check for file existence beforehand
        // may be redundant in most cases but shouldn't impact
        // performance too much.
        if (!get_file_exists(path)) {
            fprintf(stderr, "ERROR: File does not exist at path: %s\n", path);
            return nullptr;
        }

        FILE *file = fopen(path, "r");

        // This may seem redundant, but it is assumed fopen() can have
        // more problems than just a file not existing, so that's what we check for here.
        if (file == NULL) {
            fprintf(stderr, "ERROR: Couldn't open file at path: %s\n", path);
            return nullptr;
        }

        // Get file size
        size_t _file_size;
        fseek(file, 0, SEEK_END);
        _file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // If this string is not handled later, it will cause a memory leak.
        // Oh well! - what-is-a-git 12/1/2023
        char *_output_string = (char*) malloc(_file_size + 1);
        fread(_output_string, _file_size, 1, file);
        fclose(file);

        // This is required because without a terminating byte we can easily get corruption
        // in our text data.
        _output_string[_file_size] = 0;

        return _output_string;
    }

    bool FileSystem::get_file_exists(const char *path) {
        return access(path, F_OK) == 0;
    }
}