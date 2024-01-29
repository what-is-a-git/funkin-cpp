#include "glad/glad.h"

#include "core/engine.h"
#include "core/input/input_server.h"

#include "assets/asset_server.h"
#include "assets/sparrow.h"
#include "core/audio/audio_server.h"

#include "funkin/audio.h"
#include "funkin/scene/title_screen.h"

namespace funkin {
    TitleScreen::TitleScreen() {}

    TitleScreen::~TitleScreen() {
        delete girlfriend;
    }

    void TitleScreen::on_switch_dispose(void) {}

    void TitleScreen::init(void) {
        if (Audio::streams.count("MUSIC") == 0) {
            crystal::AudioStreamPlayer *music = new crystal::AudioStreamPlayer();
            music->set_stream(crystal::AssetServer::get_audio_stream("assets/audio/music/freakyMenu.ogg"));
            music->play();
            Audio::streams["MUSIC"] = music;
        }

        girlfriend = new crystal::Sprite(GAME_SIZE.x * 0.5, GAME_SIZE.y * 0.5, crystal::AssetServer::get_texture("assets/images/menus/title_screen/gf.png"));

        std::list<crystal::sparrow_frame> frames = crystal::Sparrow::load_from_path("assets/images/menus/title_screen/gf.xml");
        girlfriend->source_rect = crystal::Sparrow::get_frame(frames, "gfDance0029").source_rect;
        crystal::Sparrow::free_frame_names(frames);
        // the rest of the list should be freed automatically by the stack and shit because this is on the stack :]
    }

    void TitleScreen::step(const double delta) {
        girlfriend->step(delta);
        Audio::step(delta);
    }

    void TitleScreen::draw(void) {
        girlfriend->draw();
    }
}