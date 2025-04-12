#include "3dr/ppm.hpp"
#include "3dr/image.hpp"

namespace tdr {
    PPM PPM::serialize(const ConstCanvas<RGB24> &canvas) {
        return PPM{canvas};
    }

    PPM PPM::serialize(const Image<RGB24> &image) {
        return serialize(image.get_canvas());
    }

    PPM::PPM(ConstCanvas<RGB24> canvas) : canvas(canvas) { }

    std::ostream & operator<<(std::ostream &out, const PPM &ppm) {
        auto [w, h] = ppm.canvas.get_size();
        out << "P3" << '\n';
        out << w << ' ' << h << '\n';
        out << 255 << '\n';
        for (ImageCoords::ScalarType y = 0; y < h; y++) {
            for (ImageCoords::ScalarType x = 0; x < w; x++) {
                auto [r, g, b] = ppm.canvas.pixel({x, y});
                out << static_cast<int>(r) << '\t' << static_cast<int>(g) << '\t' << static_cast<int>(b) << '\n';
            }
        }
        return out;
    }

}
