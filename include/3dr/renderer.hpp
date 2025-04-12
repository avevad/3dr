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
        void rasterize_line(ImageCoords beg, ImageCoords end);

        Scene scene;
        Canvas<RGB24> viewport;
    };

}
