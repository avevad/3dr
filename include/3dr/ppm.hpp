#pragma once

#include "image.hpp"

namespace tdr {

    using RGB24 = Vec3<uint8_t>;

    class PPM {
    public:
        PPM(const PPM &) = delete;
        PPM(PPM &&) = delete;

        PPM &operator=(const PPM &) = delete;
        PPM &operator=(PPM &&) = delete;

        [[nodiscard]] static PPM serialize(const ConstCanvas<RGB24> &canvas);
        [[nodiscard]] static PPM serialize(const Image<RGB24> &image);

        friend std::ostream &operator<<(std::ostream &out, const PPM &ppm);

    private:
        explicit PPM(ConstCanvas<RGB24> canvas);

        ConstCanvas<RGB24> canvas;
    };

    std::ostream &operator<<(std::ostream &out, const PPM &ppm);
}
