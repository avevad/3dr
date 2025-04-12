#pragma once
#include <algorithm>
#include <ostream>
#include <cmath>

namespace tdr {

    template<typename Scalar>
    struct Vec2 {
        Scalar x { }, y { };

        using ScalarType = Scalar;

        Vec2 operator+(Vec2 vec) const {
            return {x + vec.x, y + vec.y};
        }

        Vec2 operator-(Vec2 vec) const {
            return {x - vec.x, y - vec.y};
        }

        Vec2 operator*(Vec2 vec) const {
            return {x * vec.x, y * vec.y};
        }

        Vec2 operator*(Scalar k) const {
            return {x * k, y * k};
        }

        template<typename NewScalar>
        Vec2<NewScalar> cast() const {
            return {
                static_cast<NewScalar>(x),
                static_cast<NewScalar>(y)
            };
        }

        Vec2 clamp(Vec2 lo, Vec2 hi) {
            return {
                std::clamp(x, lo.x, hi.x),
                std::clamp(y, lo.y, hi.y)
            };
        }

        Scalar len2() const {
            return x * x + y * y;
        }

        Scalar len() const {
            return std::sqrt(len2());
        }

        Scalar dot(Vec2 vec) const {
            return x * vec.x + y * vec.y;
        }
    };

    template<typename Scalar>
    std::ostream &operator<<(std::ostream &out, Vec2<Scalar> vec) {
        out << "{";
        out << vec.x;
        out << ", ";
        out << vec.y;
        out << "}";
        return out;
    }

    template<typename Scalar>
    struct Vec3 {
        Scalar x { }, y { }, z { };

        using ScalarType = Scalar;

        Vec3 operator+(Vec3 vec) const {
            return {x + vec.x, y + vec.y, z + vec.z};
        }

        Vec3 operator-(Vec3 vec) const {
            return {x - vec.x, y - vec.y, z - vec.z};
        }

        Vec3 operator*(Vec3 vec) const {
            return {x * vec.x, y * vec.y, z * vec.z};
        }

        Vec3 operator*(Scalar k) const {
            return {x * k, y * k, z * k};
        }

        template<typename NewScalar>
        Vec3<NewScalar> cast() const {
            return {
                static_cast<NewScalar>(x),
                static_cast<NewScalar>(y),
                static_cast<NewScalar>(z)
            };
        }

        Vec3 clamp(Vec3 lo, Vec3 hi) {
            return {
                std::clamp(x, lo.x, hi.x),
                std::clamp(y, lo.y, hi.y),
                std::clamp(z, lo.z, hi.z)
            };
        }

        Scalar len2() const {
            return x * x + y * y + z * z;
        }

        Scalar len() const {
            return std::sqrt(len2());
        }

        Scalar dot(Vec3 vec) const {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        Vec3 cross(Vec3 vec) const {
            return {
                y * vec.z - z * vec.y,
                z * vec.x - x * vec.z,
                x * vec.y - y * vec.x
            };
        }
    };

    template<typename Scalar>
    std::ostream &operator<<(std::ostream &out, Vec3<Scalar> vec) {
        out << "{";
        out << vec.x;
        out << ", ";
        out << vec.y;
        out << ", ";
        out << vec.z;
        out << "}";
        return out;
    }

    using RealD = long double;
    using Vec3D = Vec3<RealD>;
    using Vec2D = Vec2<RealD>;

    Vec3D rodrigues_rotate(Vec3D point, Vec3D axis, RealD angle);

}
