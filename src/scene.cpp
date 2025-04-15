#include "3dr/scene.hpp"
#include "3dr/util.hpp"

namespace tdr {
    void from_json(const Json &json, Scene &scene) {
        scene.meshes.clear();

        TDR_ASSERT(json.contains("m"));
        Json meshes = json["m"];

        TDR_ASSERT(meshes.is_array());
        for (Scene::Mesh mesh : meshes) {
            scene.meshes.push_back(mesh);
        }
    }

    void from_json(const Json &json, Scene::Mesh &mesh) {
        mesh.vertices.clear();
        mesh.triangles.clear();

        TDR_ASSERT(json.contains("v"));
        Json vertices = json["v"];
        TDR_ASSERT(json.contains("t"));
        Json triangles = json["t"];

        TDR_ASSERT(vertices.is_array());
        for (Scene::Mesh::Vertex vertex : vertices) {
            mesh.vertices.push_back(vertex);
        }

        TDR_ASSERT(triangles.is_array());
        for (Scene::Mesh::Triangle triangle : triangles) {
            mesh.triangles.push_back(triangle);
        }
    }

    void from_json(const Json &json, Scene::Mesh::Vertex &vertex) {
        TDR_ASSERT(json.is_array());
        TDR_ASSERT(json.size() == 3);
        TDR_ASSERT(json[0].is_number());
        TDR_ASSERT(json[1].is_number());
        TDR_ASSERT(json[2].is_number());
        vertex = {json[0], json[1], json[2]};
    }

    void from_json(const Json &json, Scene::Mesh::Triangle &triangle) {
        TDR_ASSERT(json.is_array());
        TDR_ASSERT(json.size() == 3);
        TDR_ASSERT(json[0].is_number_integer());
        TDR_ASSERT(json[1].is_number_integer());
        TDR_ASSERT(json[2].is_number_integer());
        triangle = {json[0], json[1], json[2]};
    }

    void to_json(Json &json, const Scene &scene) {
        json = {{"m", Json(scene.meshes)}};
    }

    void to_json(Json &json, const Scene::Mesh &mesh) {
        json = {
            {"v", Json(mesh.vertices)},
            {"t", Json(mesh.triangles)}
        };
    }

    void to_json(Json &json, const Scene::Mesh::Vertex &vertex) {
        json = {vertex.coords.x, vertex.coords.y, vertex.coords.z};
    }

    void to_json(Json &json, const Scene::Mesh::Triangle &triangle) {
        json = {triangle.u, triangle.v, triangle.w};
    }


}
