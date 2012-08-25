#ifndef LIB_SOURCE_RENDER_GRADIENT_RENDERER_H_
#define LIB_SOURCE_RENDER_GRADIENT_RENDERER_H_
/**
 * @file gradient_renderer.h
 * @brief Gradient renderer definition
 *
 * @date 2011-03-03
 * @authors Fabio R. Panettieri
 */

#include "./renderer.h"

namespace acid_maps {

/**
 * @brief Used to render sparse intervals
 */
class GradientRenderer : public Renderer {
  void render(float interpolated_bitmap[], float opacity_bitmap[], Size*
  tile_size, float intervals[], int intervals_size, Color* intervals_colors,
  unsigned char* output_buffer);
  
  int interval(float value, float intervals[], int intervals_size);
  
  static const int GRADIENT_INTERVAL_SIZE = 255;
};

};  // namespace acid_maps

#endif  // LIB_SOURCE_RENDER_GRADIENT_RENDERER_H_



