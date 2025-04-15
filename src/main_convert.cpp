#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <3dr/argparse.hpp>
#include <3dr/scene.hpp>

using namespace tdr;

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

    std::string part_type;
    while (in >> part_type) {
        if (part_type == "v") {
            RealD x, y, z;
            in >> x >> y >> z;
            scene.meshes[0].vertices.push_back({{x, y, z}});
        } else if (part_type == "f") {
            Scene::Mesh::vx_id_t u, v, w;
            in >> u >> v >> w;
            scene.meshes[0].triangles.push_back({u - 1, v - 1, w - 1});
        } else {
            assertion_failed("invalid OBJ part");
        }
    }

    std::cout << Json(scene);
}
