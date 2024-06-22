#include <memory.h>

#include "core/file_system.h"
#include "assets/png.h"

namespace crystal {
    // taken almost entirely from an example of the spng website
    // lol - what-is-a-git

    void PNG::libpng_read(png_structp png, png_bytep data, png_size_t length) {
        png_io *state = (png_io*) png_get_io_ptr(png);

        if (length > state->bytes_left) {
            png_error(png, "read_fn error");
        }

        memcpy(data, state->data, length);
        state->bytes_left -= length;
        state->data += length;
    }

    unsigned char *PNG::load_rgba8_from_path(const char *path, glm::uvec2 *size) {
        png_infop info_ptr;
        png_structp png_ptr;
        png_io state;

        int bit_depth, colour_type, interlace_type, compression_type, filter_method;
        unsigned char *volatile image = NULL;
        png_bytep *volatile row_pointers = NULL;

        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (png_ptr == NULL) {
            return NULL;
        }

        info_ptr = png_create_info_struct(png_ptr);

        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return NULL;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            fprintf(stderr, "libpng error: Failed to call png_jmpbuf(png_ptr).\n");
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

            if (image != NULL) {
                free(image);
            }

            if (row_pointers != NULL) {
                free(row_pointers);
            }

            return NULL;
        }

        file_binary file = FileSystem::get_file_binary(path);

        state.data = file.data;
        state.bytes_left = file.size;

        if (png_sig_cmp(file.data, 0, 8) != 0) {
            fprintf(stderr, "libpng error: Invalid file signature.\n");
            return NULL;
        }

        png_set_read_fn(png_ptr, &state, libpng_read);
        png_read_info(png_ptr, info_ptr);

        if (!png_get_IHDR(png_ptr, info_ptr, &size->x, &size->y, &bit_depth, &colour_type,
                    &interlace_type, &compression_type, &filter_method)) {
            fprintf(stderr, "libpng error: png_get_IHDR() .\n");
            return NULL;
        }

        /* Decode to RGBA8 layout regardless of PNG format */
        png_set_gray_to_rgb(png_ptr);
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
        png_set_expand(png_ptr);
        png_set_strip_16(png_ptr);

        png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);

        size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        size_t image_size = size->y * row_bytes;

        image = (unsigned char*) malloc(image_size);
        row_pointers = (png_bytep *volatile) malloc(size->y * sizeof(png_bytep));

        if (image == NULL || row_pointers == NULL) {
            if (image != NULL) {
                free(image);
            }

            if (row_pointers != NULL) {
                free(row_pointers);
            }

            return NULL;
        }

        for(unsigned long i = 0; i < size->y; i++) {
            row_pointers[i] = image + i * row_bytes;
        }

        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, info_ptr);

        free(row_pointers);
        free(file.data);

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return image;
    }
}