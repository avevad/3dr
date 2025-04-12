#include "3dr/geom.hpp"

tdr::Vec3D tdr::rodrigues_rotate(Vec3D point, Vec3D axis, RealD angle) {
    return point * std::cos(angle) +
           axis.cross(point) * std::sin(angle) +
           axis * (axis.dot(point) * (1 - std::cos(angle)));
}
