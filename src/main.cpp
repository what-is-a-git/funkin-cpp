#include <stdlib.h>

#include "crystal.h"
#include "funkin.h"

// Currently not using int argc, char **argv so they are not included here.
int main(void) {
    crystal::init_arguments arguments = {
        glm::uvec2(1280, 720), // width, height //
        (char*)"Friday Night Funkin'", // name //
        0, crystal::VSyncMode::OFF, // fps cap, vsync (no adaptive or any special stuff yet) //
        crystal::Color(0.0, 0.0, 0.0), // clear color //
        new crystal::StartupScene(new funkin::TitleScreen()), // scene //
    };

    crystal::Engine::init(arguments);
    crystal::Engine::current_debug_info = new funkin::ConsoleDebugInfo();
    crystal::RenderingServer::get_primary_window()->set_icon("assets/images/icon.png");

    while (!crystal::Engine::get_should_close()) {
        crystal::Engine::step(); // basically just your update functions //
        crystal::Engine::draw(); // rendering shit i guess //
    }

    crystal::Engine::dispose();

    return EXIT_SUCCESS;
}
