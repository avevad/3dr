#pragma once

#include "3dr/geom.hpp"

#include <memory>

namespace tdr {

    using ImageBounds = Vec2<std::ptrdiff_t>;
    using ImageCoords = Vec2<std::ptrdiff_t>;

    template<typename Pixel>
    class Canvas;

    template<typename Pixel>
    using ConstCanvas = Canvas<const Pixel>;

    template<typename Pixel>
    class Image {
    public:
        using PixelType = Pixel;

        explicit Image(ImageBounds size, Pixel fill = Pixel());

        [[nodiscard]] Canvas<PixelType> get_canvas();
        [[nodiscard]] ConstCanvas<PixelType> get_canvas() const;

        [[nodiscard]] ImageBounds get_size() const;

    private:
        ImageBounds size;
        std::unique_ptr<Pixel[]> pixels;
    };

    template<typename Pixel>
    Image<Pixel>::Image(ImageBounds size, Pixel fill)
        : size(size), pixels(std::make_unique<Pixel[]>(size.x * size.y)) {
        std::fill_n(pixels.get(), size.x * size.y, fill);
    }

    template<typename Pixel>
    Canvas<typename Image<Pixel>::PixelType> Image<Pixel>::get_canvas() {
        return {pixels.get(), size, {0, 0}, size};
    }

    template<typename Pixel>
    ConstCanvas<typename Image<Pixel>::PixelType> Image<Pixel>::get_canvas() const {
        return {pixels.get(), size, {0, 0}, size};
    }

    template<typename Pixel>
    ImageBounds Image<Pixel>::get_size() const {
        return size;
    }

    template<typename Pixel>
    class Canvas {
    public:
        using PixelType = Pixel;
        using ConstCanvas = ConstCanvas<Pixel>;

        Canvas(PixelType *pixels, ImageBounds img_size, ImageBounds offset, ImageBounds size);

        PixelType &pixel(ImageCoords coords);
        const PixelType &pixel(ImageCoords coords) const;

        [[nodiscard]] Canvas crop(ImageBounds offset, ImageBounds size);
        [[nodiscard]] ConstCanvas crop(ImageBounds offset, ImageBounds size) const;

        [[nodiscard]] ImageBounds get_size() const;

        void fill(PixelType pixel);

        // ReSharper disable once CppNonExplicitConversionOperator
        operator ConstCanvas() const { // NOLINT
            return {pixels, img_size, offset, size};
        }

    private:
        PixelType *pixels;
        ImageBounds img_size, offset, size;
    };

    template<typename Pixel>
    typename Canvas<Pixel>::PixelType &Canvas<Pixel>::pixel(ImageCoords coords) {
        auto img_coords = offset + coords;
        return pixels[img_size.x * img_coords.y + img_coords.x];
    }

    template<typename Pixel>
    const typename Canvas<Pixel>::PixelType &Canvas<Pixel>::pixel(ImageCoords coords) const {
        auto img_coords = offset + coords;
        return pixels[img_size.x * img_coords.y + img_coords.x];
    }

    template<typename Pixel>
    Canvas<Pixel> Canvas<Pixel>::crop(ImageBounds offset, ImageBounds size) {
        return {pixels, img_size, this->offset + offset, size};
    }

    template<typename Pixel>
    ConstCanvas<Pixel> Canvas<Pixel>::crop(ImageBounds offset, ImageBounds size) const {
        return {pixels, img_size, this->offset + offset, size};
    }

    template<typename Pixel>
    ImageBounds Canvas<Pixel>::get_size() const {
        return size;
    }

    template<typename Pixel>
    void Canvas<Pixel>::fill(PixelType pixel) {
        for (std::size_t y = offset.y; y < offset.y + size.y; y++) {
            std::fill_n(pixels + (img_size.x * y + offset.x), size.x, pixel);
        }
    }

    template<typename Pixel>
    Canvas<Pixel>::Canvas(PixelType *pixels, ImageBounds img_size, ImageBounds offset, ImageBounds size)
        : pixels(pixels), img_size(img_size), offset(offset), size(size) { }

    constexpr static inline ImageBounds DEFAULT_VIEWPORT_SIZE = {640, 480};
    constexpr static inline RealD DEFAULT_FRAMERATE = 60;
}
