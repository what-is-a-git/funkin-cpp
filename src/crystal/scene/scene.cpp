#include "scene/scene.h"

namespace crystal {
    Scene::Scene() {
        _active = true;
    };
    
    Scene::~Scene() {};
    void Scene::init(void) {};
    void Scene::step(const double delta) {};
    void Scene::draw(void) {};
    void Scene::on_switch_dispose(void) {};
}