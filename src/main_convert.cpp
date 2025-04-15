#include <fstream>
#include <iostream>
#include <random>
#include <nlohmann/json.hpp>
#include <3dr/argparse.hpp>
#include <3dr/ppm.hpp>
#include <3dr/scene.hpp>

using namespace tdr;

constexpr static size_t COLORS_SEED = 777;

int main(int argc, char **argv) {
    ArgParser parse;
    std::optional<std::filesystem::path> obj_path;
    parse.register_free(
        [&](const auto &cur) {
            obj_path = cur;
        }
    );
    parse(argc, argv);

    obj_path = obj_path.value_or("/dev/stdin");
    std::ifstream in(obj_path.value());

    Scene scene;
    scene.meshes.emplace_back();

    std::mt19937_64 color_rng {COLORS_SEED}; // NOLINT(*-msc51-cpp)
    std::uniform_real_distribution<RealD> color_gen(0, 1);

    std::string part_type;
    while (in >> part_type) {
        if (part_type == "v") {
            RealD x, y, z;
            in >> x >> y >> z;
            scene.meshes[0].vertices.push_back({{x, y, z}});
        } else if (part_type == "f") {
            Scene::Mesh::vx_id_t u, v, w;
            in >> u >> v >> w;
            auto r = color_gen(color_rng), g = color_gen(color_rng), b = color_gen(color_rng);
            Scene::Mesh::Material material = Scene::Mesh::SolidMaterial({r, g, b});
            scene.meshes[0].triangles.push_back({u - 1, v - 1, w - 1, material});
        } else {
            assertion_failed("invalid OBJ part");
        }
    }

    scene.ambient_light = 0.5;
    scene.direct_lights.push_back({
        .center = {1, 2, 0},
        .direction = {-1, -2, 1},
        .intensity = 0.5
    });

    std::cout << Json(scene);
}
