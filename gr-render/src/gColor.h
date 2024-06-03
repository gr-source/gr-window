#pragma once

#include <istream>
#include <ostream>

namespace  grr {
    struct gColor {
        union {
            struct {
                float r;
                float g;
                float b;
                float a;
            };
            float data[4];
        };

        gColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

        gColor(const gColor& other) : r(other.r), g(other.g), b(other.b), a(other.a) {}

        static const gColor red;

        static const gColor green;

        static const gColor blue;

        static const gColor white;

        static const gColor black;

        gColor& operator=(const gColor& other);

        friend std::istream& operator>>(std::istream& is, gColor& gColor) {
            is >> gColor.r >> gColor.g >> gColor.b >> gColor.a;
            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, const gColor& gColor) {
            os << gColor.r << " " << gColor.g << " " << gColor.b << " " << gColor.a;
            return os;
        }
    };

    inline const gColor gColor::red = {1.0f, 0.0f, 0.0f};

    inline const gColor gColor::green = {0.0f, 1.0f, 0.0f};

    inline const gColor gColor::blue = {0.0f, 0.0f, 1.0f};

    inline const gColor gColor::white = {1.0f, 1.0f, 1.0f};

    inline const gColor gColor::black {0.0f, 0.0f, 0.0f};

    inline gColor& gColor::operator=(const gColor& other) {
        if (this != &other) {
            r = other.r;
            g = other.g;
            b = other.b;
            a = other.a;
        }
        return *this;
    }

} // namespace  gr
