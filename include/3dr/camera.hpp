#pragma once

#include <optional>

#include "geom.hpp"
#include "image.hpp"

namespace tdr {

    class Camera {
    public:
        std::optional<Vec2D> project_onto_viewport(Vec3D point) const;

        template<typename Pixel>
        static Camera default_for_viewport(const Canvas<Pixel> &viewport);

        Vec3D get_sight_direction() const;

        void move_along(Vec3D direction);

    private:
        struct SpatialParams;

        explicit Camera(SpatialParams params);

        struct SpatialParams {
            Vec3D eye_pos, sight_dir;
            Vec3D viewport_vert, viewport_horiz;
        } params;
    };

    template<typename Pixel>
    Camera Camera::default_for_viewport(const Canvas<Pixel> &viewport) {
        auto w = static_cast<Vec3D::ScalarType>(viewport.get_size().x);
        auto h = static_cast<Vec3D::ScalarType>(viewport.get_size().y);
        if (w > h) {
            Camera camera(
                {
                    .eye_pos = {0, 0, 0},
                    .sight_dir = {0, 0, 1},
                    .viewport_vert = {0, 1, 0},
                    .viewport_horiz = {w / h, 0, 0}
                }
            );
            return camera;
        } else {
            Camera camera(
                {
                    .eye_pos = {0, 0, 0},
                    .sight_dir = {0, 0, 1},
                    .viewport_vert = {0, h / w, 0},
                    .viewport_horiz = {1, 0, 0}
                }
            );
            return camera;
        }
    }

}
