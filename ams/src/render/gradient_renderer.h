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
  void render(float interpolated_bitmap[], float weight_bitmap[], Size*
  tile_size, Configuration *configuration, unsigned char* output_buffer);
  
  int interval(float value, float intervals[], int intervals_size);
  void render_weight(unsigned char* output_buffer , float weight, Configuration *configuration);
  int interpolate(float value, float value_min, float value_max, int result_min, int result_max);
  
  static const int GRADIENT_INTERVAL_SIZE = 255;
};

};  // namespace acid_maps

#endif  // LIB_SOURCE_RENDER_GRADIENT_RENDERER_H_



