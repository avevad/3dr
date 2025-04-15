#include "3dr/renderer.hpp"

#include <iostream>

namespace tdr {

    Renderer::Renderer(Scene scene, Canvas<RGB24> viewport) : scene(std::move(scene)), viewport(viewport) { }

    void Renderer::render(const Camera &camera) {
        for (const auto &mesh : scene.meshes) {
            for (auto triangle : mesh.triangles) {
                render_mesh_triangle(camera, mesh, triangle);
            }
        }
    }

    ImageCoords Renderer::from_viewport_coords(Vec2D coords) {
        return (
            (coords * Vec2D {1, -1} + Vec2D {1, 1}) * 0.5 * viewport.get_size().cast<Vec2D::ScalarType>()
        ).cast<ImageCoords::ScalarType>().clamp({0, 0}, viewport.get_size() - ImageCoords {1, 1});
    }

    void Renderer::render_mesh_triangle(const Camera &camera, const Scene::Mesh &mesh, Scene::Mesh::Triangle triangle) {
        auto [ui, vi, wi, mt] = triangle;
        auto u = mesh.vertices[ui], v = mesh.vertices[vi], w = mesh.vertices[wi];
        auto u_r = camera.project_onto_viewport(u.coords);
        auto v_r = camera.project_onto_viewport(v.coords);
        auto w_r = camera.project_onto_viewport(w.coords);
        if (!u_r.has_value() || !v_r.has_value() || !w_r.has_value()) {
            return;
        }
        if (auto *wf_mt = std::get_if<Scene::Mesh::WireframeMaterial>(&mt)) {
            rasterize_line(from_viewport_coords(u_r.value()), from_viewport_coords(v_r.value()));
            rasterize_line(from_viewport_coords(v_r.value()), from_viewport_coords(w_r.value()));
            rasterize_line(from_viewport_coords(w_r.value()), from_viewport_coords(u_r.value()));
        }
        if (auto *s_mt = std::get_if<Scene::Mesh::SolidMaterial>(&mt)) {
            rasterize_triangle(camera, u.coords, v.coords, w.coords, s_mt->color);
        }
    }

    void Renderer::rasterize_triangle(const Camera &camera, Vec3D u, Vec3D v, Vec3D w, Vec3D color) {
        assertion_failed("not implemented");
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
                viewport.pixel({x, y}) = {255, 255, 255};
            }
        } else {
            auto y0 = std::min(beg.y, end.y);
            auto y1 = std::max(beg.y, end.y);
            for (auto y = y0; y <= y1; y++) {
                auto x = beg.x;
                if (hr != 0) {
                    x += static_cast<ImageCoords::ScalarType>(static_cast<RealD>(y - beg.y) / hr * wr);
                }
                viewport.pixel({x, y}) = {255, 255, 255};
            }
        }
    }

}
