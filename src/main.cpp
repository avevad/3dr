#include "3dr/argparse.hpp"
#include "3dr/camera.hpp"
#include "3dr/image.hpp"
#include "3dr/ppm.hpp"
#include "3dr/renderer.hpp"
#include "3dr/scene.hpp"

#include <fstream>
#include <iostream>

using namespace tdr;

constexpr std::size_t DEFAULT_VIEWPORT_WIDTH = 640, DEFAULT_VIEWPORT_HEIGHT = 480;

int main(int argc, char **argv) {
    ArgParser parse;
    std::optional<std::filesystem::path> scene_path, camera_path, out_path;
    parse.register_keys({"-s", "--scene"}, [&] (const auto& cur, auto &next, auto end) {
        if (next == end) {
            std::cerr << argv[0] << ": " << cur << ": scene path expected" << std::endl;
            std::exit(1);
        }
        if (scene_path.has_value()) {
            std::cerr << argv[0] << ": " << cur << ": multiple scenes in arguments" << std::endl;
            std::exit(1);
        }
        scene_path = *next++;
    });
    parse.register_keys({"-c", "--camera"}, [&] (const auto &cur, auto &next, auto end) {
        if (next == end) {
            std::cerr << argv[0] << ": " << cur << ": camera path expected" << std::endl;
            std::exit(1);
        }
        if (camera_path.has_value()) {
            std::cerr << argv[0] << ": " << cur << ": multiple cameras in arguments" << std::endl;
            std::exit(1);
        }
        camera_path = *next++;
    });
    parse.register_keys({"-o", "--output"}, [&] (const auto &cur, auto &next, auto end) {
        if (next == end) {
            std::cerr << argv[0] << ": " << cur << ": output path expected" << std::endl;
            std::exit(1);
        }
        if (out_path.has_value()) {
            std::cerr << argv[0] << ": " << cur << ": multiple outputs in arguments" << std::endl;
            std::exit(1);
        }
        out_path = *next++;
    });
    parse.register_free([&] (const auto& cur) {
        if (scene_path.has_value()) {
            std::cerr << argv[0] << ": " << cur << ": multiple scenes in arguments" << std::endl;
            std::exit(1);
        }
        scene_path = cur;
    });
    parse(argc, argv);

    scene_path = scene_path.value_or("/dev/stdin");
    out_path = out_path.value_or("/dev/stdout");

    std::ifstream scene_in(scene_path.value());
    Scene scene = Json::parse(scene_in);

    Image<RGB24> frame({DEFAULT_VIEWPORT_WIDTH, DEFAULT_VIEWPORT_HEIGHT}, {0, 0, 0});
    auto viewport = frame.get_canvas();
    std::ofstream viewport_out(out_path.value());

    Renderer renderer(scene, viewport);
    Camera camera = Camera::default_for_viewport(viewport);
    auto produce_frame = [&] {
        viewport.fill({0, 0, 0});
        renderer.render(camera);
        viewport_out << PPM::serialize(viewport);
    };

    if (camera_path.has_value()) {
        std::ifstream camera_in(camera_path.value());
        try {
            Json camera_json;
            while (camera_in >> camera_json) {
                from_json(camera_json, camera);
                produce_frame();
            }
        } catch (nlohmann::json::parse_error &) { }
    } else {
        produce_frame();
    }
}
