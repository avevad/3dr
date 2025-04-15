#pragma once

#include <optional>

#include "geom.hpp"
#include "image.hpp"
#include "util.hpp"

namespace tdr {

    class Camera {
    public:
        [[nodiscard]] std::optional<Vec2D> project_onto_viewport(Vec3D point) const;

        [[nodiscard]] static Camera default_for_viewport_size(ImageBounds viewport_size);

        [[nodiscard]] Vec3D get_sight_direction() const;
        [[nodiscard]] Vec3D get_upwards_perpendicular() const;
        [[nodiscard]] Vec3D get_rightwards_perpendicular() const;

        void move_along(Vec3D direction);
        void rotate_about(Vec3D axis, Vec3D::ScalarType angle);

    private:
        struct SpatialParams;

        explicit Camera(SpatialParams params);

        friend void to_json(Json &json, const Camera &camera);
        friend void from_json(const Json &json, Camera &camera);

        struct SpatialParams {
            Vec3D eye_pos, sight_dir;
            Vec3D viewport_vert, viewport_horiz;
        } params;
    };

    void to_json(Json &json, const Camera &camera);
    void from_json(const Json &json, Camera &camera);


}
