/**
 * @file sparse_renderer.cc
 *
 * @date 2010-11-08
 * @authors Fabio R. Panettieri
 */

#include "../constants/constants.h"
#include "../core/color.h"
#include "../core/configuration.h"
#include "../core/size.h"
#include "./sparse_renderer.h"
#include <cstring>

namespace acid_maps {

/**
 * @todo Marching squares
 */ 
void SparseRenderer::render(float interpolated_bitmap[], float opacity_bitmap[], Size* tile_size, float intervals[],
  int intervals_size, Color* intervals_colors, unsigned char* output_buffer) {
  int interval_index;
  int bitmap_size = tile_size->width * tile_size->height;

  for (unsigned int i = 0; i < bitmap_size; i++) {
    interval_index = this->interval(interpolated_bitmap[i], intervals, intervals_size);
    std::memcpy(output_buffer + i * RGBA, intervals_colors + interval_index, sizeof(Color));
  }
}

int SparseRenderer::interval(float value, float intervals[], int intervals_size) {
  int first = 0;
  int last = intervals_size - 1;
  int mid = 0;
  
  if (value < intervals[first]) {
    return first;
  } else if (value >= intervals[last]) {
    return last;
  }

  while (first <= last) {
    mid = (first + last) / 2;
    if (value > intervals[mid]) {  
      first = mid + 1;
    } else if (value < intervals[mid]) {
      if (mid >= 1 && value > intervals[mid - 1]) {
        return mid;
      } else {
        last = mid - 1;
      }
    } else {
      return mid;
    }
  }
  
  // HACK: changed -1 to avoid segmentation faults. 
  // This error should be made explicit
  return 0;
}


};  // namespace acid_maps

