#ifndef LIB_SOURCE_RENDER_SPARSE_RENDERER_H_
#define LIB_SOURCE_RENDER_SPARSE_RENDERER_H_
/**
 * @file sparse_renderer.h
 * @brief Sparse renderer definition
 *
 * @date 2010-11-03
 * @authors Fabio R. Panettieri
 */

#include "./renderer.h"

namespace acid_maps {

/**
 * @brief Used to render sparse intervals
 */
class SparseRenderer : public Renderer {
  void render(float interpolated_bitmap[], float weight_bitmap[],
    Size* tile_size, Configuration *configuration,
    unsigned char* output_buffer);
  
  int interval(float value, float intervals[], int intervals_size);
};

};  // namespace acid_maps

#endif  // LIB_SOURCE_RENDER_SPARSE_RENDERER_H_
