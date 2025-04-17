#include <cmath>
#include <cstdio>
#define CY_SHORT_TYPENAMES
#include "CY/types.hpp"

struct Vec2
{

    f64 x;
    f64 y;

    Vec2(const Vec2 &) = default;
    Vec2(Vec2 &&) = default;
    Vec2 &operator=(const Vec2 &) = default;
    Vec2 &operator=(Vec2 &&) = default;

    Vec2(f64 x, f64 y)
        : x(x)
        , y(y)
    {
    }
    Vec2()
        : x(0.0f)
        , y(0.0f)
    {
    }

    inline constexpr f64 DistanceFrom(Vec2 const &other)
    {
        return std::sqrt((other.x - this->x) * (other.x - this->x) +
                         (other.y - this->y) * (other.y - this->y));
    }
};

i32 main(void)
{
    Vec2 a(16, 16);
    Vec2 b(48, 48);

    std::printf("Point A is at (%.2f, %.2f).\nPoint B is at (%.2f, %.2f).\n",
                a.x,
                a.y,
                b.x,
                b.y);

    f64 distance = a.DistanceFrom(b);
    std::printf("The distance between A and B is %.2f.\n", distance);
    return 0;
}