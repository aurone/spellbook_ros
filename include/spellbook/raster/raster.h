#ifndef au_raster_h
#define au_raster_h

#include <spellbook/grid/grid.h>

namespace au {

template <typename T>
void RasterizeLine(
    grid<2, T>& g,
    const typename grid<2, T>::GridIndex& a,
    const typename grid<2, T>::GridIndex& b,
    const T& fill);

/// \tparam Function
template <typename T, typename Function>
void RasterizeLine(
    grid<2, T>& g,
    typename grid<2, T>::GridIndex a,
    typename grid<2, T>::GridIndex b,
    const Function& fn);

} // namespace au

#include "detail/raster.h"

#endif
