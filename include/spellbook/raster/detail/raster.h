#ifndef au_detail_raster_h
#define au_detail_raster_h

#include <algorithm>
#include <stdlib.h>

namespace au {

template <typename T>
void RasterizeLine(
    grid<2, T>& g,
    const typename grid<2, T>::grid_index& a,
    const typename grid<2, T>::grid_index& b,
    const T& fill)
{
    auto filler = [&](const typename grid<2,T>::grid_index& i) { g(i) = fill; };
    RasterizeLine(g, a, b, filler);
}

template <typename T, typename Function>
void RasterizeLine(
    grid<2, T>& g,
    typename grid<2, T>::grid_index a,
    typename grid<2, T>::grid_index b,
    const Function& fn)
{
    bool steep = abs((int)(b(1) - a(1))) > abs((int)(b(0) - a(0)));
    if (steep) {
        std::swap(a(0), a(1));
        std::swap(b(0), b(1));
    }
    if (a(0) > b(0)) {
        std::swap(a(0), b(0));
        std::swap(a(1), b(1));
    }
    int dx = b(0) - a(0);
    int dy = abs((int)(b(1) - a(1)));
    int error = dx / 2;
    int ystep = a(1) < b(1) ? 1 : -1;
    int y = a(1);
    for (int x = a(0); x <= b(0); x++) {
        if (steep) {
            fn(typename grid<2, T>::grid_index(x, y));
        }
        else {
            fn(typename grid<2, T>::grid_index(y, x));
        }
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

} // namespace au

#endif
