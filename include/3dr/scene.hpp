#pragma once

#include <cstddef>
#include <vector>
#include <nlohmann/json.hpp>

#include "geom.hpp"
#include "util.hpp"

namespace tdr {

    struct Scene {
        struct Mesh {
            struct Vertex {
                Vec3D coords;
            };

            using vx_id_t = std::size_t;

            struct Triangle {
                vx_id_t u, v, w;
            };

            std::vector<Vertex> vertices;
            std::vector<Triangle> triangles;
        };

        std::vector<Mesh> meshes;
    };

    void from_json(const Json &json, Scene &scene);
    void from_json(const Json &json, Scene::Mesh &mesh);
    void from_json(const Json &json, Scene::Mesh::Vertex &vertex);
    void from_json(const Json &json, Scene::Mesh::Triangle &triangle);

}
