#pragma once
#include "camera.hpp"
#include "image.hpp"
#include "ppm.hpp"
#include "scene.hpp"

namespace tdr {

    class Renderer {
    public:
        Renderer(Scene scene, Canvas<RGB24> viewport);

        void render(const Camera &camera);

    private:
        ImageCoords from_viewport_coords(Vec2D coords);

        void render_mesh_triangle(const Camera &camera, Vec3<Vec3D> triangle, Scene::Mesh::Material material);
        void rasterize_triangle(const Camera &camera, Vec3D u, Vec3D v, Vec3D w, Vec3D color);
        void rasterize_line(ImageCoords beg, ImageCoords end);

        Scene scene;
        Canvas<RGB24> viewport;
        Image<RealD> z_buffer;

        static constexpr RealD DISTANCE_INFINITY = 1e9;
    };

}
