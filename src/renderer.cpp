#include "3dr/renderer.hpp"

#include <iostream>

namespace tdr {

    Renderer::Renderer(Scene scene, Canvas<RGB24> viewport)
        : scene(std::move(scene)), viewport(viewport), z_buffer(viewport.get_size()) { }

    void Renderer::render(const Camera &camera) {
        viewport.fill({0, 0, 0});
        z_buffer.get_canvas().fill(DISTANCE_INFINITY);
        for (const auto &mesh : scene.meshes) {
            for (auto [u, v, w, mt] : mesh.triangles) {
                render_mesh_triangle(
                    camera, {mesh.vertices[u].coords, mesh.vertices[v].coords, mesh.vertices[w].coords}, mt
                );
            }
        }
    }

    ImageCoords Renderer::from_viewport_coords(Vec2D coords) {
        return (
            (coords * Vec2D {1, -1} + Vec2D {1, 1}) * 0.5 * viewport.get_size().cast<Vec2D::ScalarType>()
        ).cast<ImageCoords::ScalarType>();
    }

    void Renderer::render_mesh_triangle(const Camera &camera, Vec3<Vec3D> triangle, Scene::Mesh::Material material) {
        auto [u, v, w] = triangle;
        auto u_r = camera.project_onto_viewport(u);
        auto v_r = camera.project_onto_viewport(v);
        auto w_r = camera.project_onto_viewport(w);
        if (!u_r.has_value() || !v_r.has_value() || !w_r.has_value()) {
            return;
        }
        if (auto *wf_mt = std::get_if<Scene::Mesh::WireframeMaterial>(&material)) {
            rasterize_line(from_viewport_coords(u_r.value()), from_viewport_coords(v_r.value()));
            rasterize_line(from_viewport_coords(v_r.value()), from_viewport_coords(w_r.value()));
            rasterize_line(from_viewport_coords(w_r.value()), from_viewport_coords(u_r.value()));
        }
        if (auto *s_mt = std::get_if<Scene::Mesh::SolidMaterial>(&material)) {
            rasterize_triangle(camera, u, v, w, s_mt->color);
        }
    }

    void Renderer::rasterize_triangle(const Camera &camera, Vec3D u, Vec3D v, Vec3D w, Vec3D color) {
        ImageCoords u_i = from_viewport_coords(camera.project_onto_viewport(u).value()),
                    v_i = from_viewport_coords(camera.project_onto_viewport(v).value()),
                    w_i = from_viewport_coords(camera.project_onto_viewport(w).value());
        auto zb = z_buffer.get_canvas();
        auto x0 = std::min({u_i.x, v_i.x, w_i.x}), x1 = std::max({u_i.x, v_i.x, w_i.x});
        auto y0 = std::min({u_i.y, v_i.y, w_i.y}), y1 = std::max({u_i.y, v_i.y, w_i.y});
        auto uz = camera.distance_to_eye(u), vz = camera.distance_to_eye(v), wz = camera.distance_to_eye(w);
        std::pair box = {
            ImageCoords {x0, y0}.clamp({0, 0}, viewport.get_size() - ImageBounds {1, 1}),
            ImageCoords {x1, y1}.clamp({0, 0}, viewport.get_size() - ImageBounds {1, 1})
        };
        auto area = [](Vec2D p1, Vec2D p2, Vec2D p3) {
            return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        };
        auto total = area(u_i.cast<RealD>(), v_i.cast<RealD>(), w_i.cast<RealD>());
        for (ImageCoords::ScalarType y = box.first.y; y <= box.second.y; y++) {
            for (ImageCoords::ScalarType x = box.first.x; x <= box.second.x; x++) {
                ImageCoords pt = {x, y};
                auto bc_u = area(v_i.cast<RealD>(), w_i.cast<RealD>(), pt.cast<RealD>()) / total,
                     bc_v = area(w_i.cast<RealD>(), u_i.cast<RealD>(), pt.cast<RealD>()) / total,
                     bc_w = area(u_i.cast<RealD>(), v_i.cast<RealD>(), pt.cast<RealD>()) / total;
                if (bc_u < 0 || bc_v < 0 || bc_w < 0) {
                    continue;
                }
                auto z = uz * bc_u + vz * bc_v + wz * bc_w;
                if (z < zb.pixel({x, y})) {
                    zb.pixel({x, y}) = z;
                    viewport.pixel({x, y}) = camera.transform_color(color);
                }
            }
        }
    }

    void Renderer::rasterize_line(ImageCoords beg, ImageCoords end) {
        auto [w, h] = end - beg;
        auto wr = static_cast<RealD>(w), hr = static_cast<RealD>(h);
        if (std::abs(w) > std::abs(h)) {
            auto x0 = std::min(beg.x, end.x);
            auto x1 = std::max(beg.x, end.x);
            for (auto x = x0; x <= x1; x++) {
                auto y = beg.y;
                if (wr != 0) {
                    y += static_cast<ImageCoords::ScalarType>(static_cast<RealD>(x - beg.x) / wr * hr);
                }
                if (0 <= x && x < viewport.get_size().x && 0 <= y && y < viewport.get_size().y) {
                    viewport.pixel({x, y}) = {255, 255, 255};
                }
            }
        } else {
            auto y0 = std::min(beg.y, end.y);
            auto y1 = std::max(beg.y, end.y);
            for (auto y = y0; y <= y1; y++) {
                auto x = beg.x;
                if (hr != 0) {
                    x += static_cast<ImageCoords::ScalarType>(static_cast<RealD>(y - beg.y) / hr * wr);
                }
                if (0 <= x && x < viewport.get_size().x && 0 <= y && y < viewport.get_size().y) {
                    viewport.pixel({x, y}) = {255, 255, 255};
                }
            }
        }
    }

}
