#include <cstring>
#include <stdlib.h>

#include "littlexml/littlexml.h"

#include "assets/sparrow.h"

namespace crystal {
    sparrow_frame Sparrow::get_frame(const std::list<sparrow_frame> frames, const char *name) {
        if (frames.empty()) {
            return { glm::ivec4(0), glm::ivec4(0), NULL };
        }

        auto iterator = frames.begin();

        // lol weird syntax :]
        for (; iterator != frames.end(); iterator++) {
            if (strcmp(iterator->name, name) == 0) {
                return *iterator;
            }
        }

        // this will return the last value as a fallback \_(:3)_/
        return *iterator;
    }

    void Sparrow::free_frame_names(std::list<sparrow_frame> frames) {
        auto iterator = frames.begin();

        for (; iterator != frames.end(); iterator++) {
            free(iterator->name);
        }
    }

    // using atoi() because :3
    int get_integer_from_attribute(XMLNode *node, const char *attribute) {
        XMLAttribute *xml_attribute = XMLNode_attr(node, attribute);
        
        if (xml_attribute == NULL) {
            fprintf(stderr, "Warning: Tried to convert attribute '%s' to an integer when it does not exist!\n", attribute);
            return -1;
        }

        return atoi(xml_attribute->value);
    }

    bool has_attribute(XMLNode *node, const char *attribute) {
        return XMLNode_attr(node, attribute) != NULL;
    }

    std::list<sparrow_frame> Sparrow::load_from_path(const char *path) {
        std::list<sparrow_frame> frames;
        XMLDocument document;

        if (XMLDocument_load(&document, path)) {
            XMLNode *root_node = XMLNode_child(document.root, 0);
            XMLNodeList *sub_textures = XMLNode_children(root_node, "SubTexture");

            if (sub_textures->size == 0) {
                XMLNodeList_free(sub_textures);
                // this should handle *basically* all edge cases, if not, your xml is kinda messed up :]
                sub_textures = XMLNode_children(root_node, "subtexture");
            }

            for (int i = 0; i < sub_textures->size; i++) {
                XMLNode *sub_texture = XMLNodeList_at(sub_textures, i);
                
                // fields that are REQUIRED to be present: name, x, y, width, height
                // fields that ARE NOT required: frameX, frameY, frameWidth, frameHeight
                // fields that are UNSUPPORTED: rotated (maybe in future ;))

                // special handling because strings are funky lol
                char *attribute_name = XMLNode_attr(sub_texture, "name")->value;
                // 255 chars and the ending \0 :3
                // also yes ik sizeof(char) is 1 or whatever but you gotta do it for consistency bro trust me :]
                char *name = (char*) malloc(sizeof(char) * 256);

                // separate it for a VERY specific reason that is elaborated later
                int k = 0;
                for (k = 0; k < 256 && attribute_name[k] != '\0'; k++) {
                    name[k] = attribute_name[k];
                }
                // the elaboration is: the null byte :3
                name[k] = '\0';

                // used for source_rect
                int x = get_integer_from_attribute(sub_texture, "x");
                int y = get_integer_from_attribute(sub_texture, "y");
                int width = get_integer_from_attribute(sub_texture, "width");
                int height = get_integer_from_attribute(sub_texture, "height");

                // pre-set to 0 just for later stuff to be easier :]
                int offset_x, offset_y, offset_width, offset_height;
                offset_x = offset_y = offset_width = offset_height = 0;

                if (has_attribute(sub_texture, "frameX")) {
                    offset_x = get_integer_from_attribute(sub_texture, "frameX");
                    offset_y = get_integer_from_attribute(sub_texture, "frameY");
                    offset_width = get_integer_from_attribute(sub_texture, "frameWidth");
                    offset_height = get_integer_from_attribute(sub_texture, "frameHeight");
                }

                sparrow_frame frame = {
                    glm::ivec4(x, y, width, height),
                    glm::ivec4(offset_x, offset_y, offset_width, offset_height),
                    name
                };

                frames.push_back(frame);
            }

            // node lists not handled automatically but that's ok
            XMLNodeList_free(sub_textures);
            XMLDocument_free(&document);
        } else {
            fprintf(stderr, "Warning: No XML found at path '%s', an empty list of frames will be returned instead.\n", path);
        }

        return frames;
    }
}