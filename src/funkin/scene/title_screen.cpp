#include "glad/glad.h"

#include "core/engine.h"
#include "core/input/input_server.h"

#include "assets/asset_server.h"
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

        girlfriend = new crystal::Sprite2D(GAME_SIZE.x * 0.5, GAME_SIZE.y * 0.5, crystal::AssetServer::get_texture("assets/images/menus/title_screen/gf.png"));
        girlfriend->source_rect = glm::vec4(0.0f, 0.0f, 717.0f, 648.0f);
    }

    void TitleScreen::step(const double delta) {
        girlfriend->step(delta);
        Audio::step(delta);
    }

    void TitleScreen::draw(void) {
        girlfriend->draw();
    }
}