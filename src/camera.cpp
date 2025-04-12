#include "3dr/camera.hpp"

namespace tdr {

    std::optional<Vec2D> Camera::project_onto_viewport(Vec3D point) const {
        point = point - params.eye_pos;
        auto z = params.sight_dir.dot(point);
        if (z <= params.sight_dir.len2()) {
            return std::nullopt;
        }
        point = point * (params.sight_dir.len2() / params.sight_dir.dot(point));
        return {
            {
                point.dot(params.viewport_horiz) * (1 / params.viewport_horiz.len2()),
                point.dot(params.viewport_vert) * (1 / params.viewport_vert.len2())
            }
        };
    }

    Vec3D Camera::get_sight_direction() const {
        return params.sight_dir;
    }

    void Camera::move_along(Vec3D direction) {
        params.eye_pos = params.eye_pos + direction;
    }

    Camera::Camera(SpatialParams params) : params(params) { }

}
