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

    RealD Camera::distance_to_eye(Vec3D point) const {
        return (point - params.eye_pos).len();
    }

    Vec3D Camera::get_sight_direction() const {
        return params.sight_dir;
    }

    Vec3D Camera::get_upwards_perpendicular() const {
        return params.viewport_vert;
    }

    Vec3D Camera::get_rightwards_perpendicular() const {
        return params.viewport_horiz;
    }

    RGB24 Camera::transform_color(Vec3D color) const {
        return (color * Vec3D {256, 256, 256}).clamp({0, 0, 0}, {255, 255, 255}).cast<RGB24::ScalarType>();
    }

    void Camera::move_along(Vec3D direction) {
        params.eye_pos = params.eye_pos + direction;
    }

    void Camera::rotate_about(Vec3D axis, Vec3D::ScalarType angle) {
        axis = axis * (1 / axis.len());
        params.sight_dir = rodrigues_rotate(params.sight_dir, axis, angle);
        params.viewport_horiz = rodrigues_rotate(params.viewport_horiz, axis, angle);
        params.viewport_vert = rodrigues_rotate(params.viewport_vert, axis, angle);
    }

    Camera::Camera(SpatialParams params) : params(params) { }

    Camera Camera::default_for_viewport_size(ImageBounds viewport_size) {
        auto w = static_cast<Vec3D::ScalarType>(viewport_size.x);
        auto h = static_cast<Vec3D::ScalarType>(viewport_size.y);
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

    void to_json(Json &json, const Camera &camera) {
        auto [e, d, h, v] = std::tie(
            camera.params.eye_pos, camera.params.sight_dir, camera.params.viewport_horiz, camera.params.viewport_vert
        );
        json = {
            {"e", Json {e.x, e.y, e.z}},
            {"d", Json {d.x, d.y, d.z}},
            {"h", Json {h.x, h.y, h.z}},
            {"v", Json {v.x, v.y, v.z}}
        };
    }

    void from_json(const Json &json, Camera &camera) {
        camera.params = {
            .eye_pos = {json["e"][0], json["e"][1], json["e"][2]},
            .sight_dir = {json["d"][0], json["d"][1], json["d"][2]},
            .viewport_vert = {json["v"][0], json["v"][1], json["v"][2]},
            .viewport_horiz = {json["h"][0], json["h"][1], json["h"][2]},
        };
    }

}
