#include "3dr/image.hpp"
#include "3dr/ppm.hpp"
#include "3dr/scene.hpp"

#include <iostream>
#include <3dr/camera.hpp>
#include <3dr/renderer.hpp>

using namespace tdr;

constexpr std::size_t DEFAULT_VIEWPORT_WIDTH = 640, DEFAULT_VIEWPORT_HEIGHT = 480;

int main() {
    Scene scene = Json::parse(std::cin);

    Image<RGB24> frame({DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT}, {0, 0, 0});
    auto viewport = frame.get_canvas();

    Renderer renderer(scene, viewport);
    Camera camera = Camera::default_for_viewport(viewport);

    viewport.fill({0, 0, 0});
    renderer.render(camera);
    std::cout << PPM::serialize(frame);
}
