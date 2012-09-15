#ifndef LIB_SOURCE_INTERPOLATE_MM_H_
#define LIB_SOURCE_INTERPOLATE_MM_H_
/**
 * @file measure_map.h
 * @brief Map of Measurements - avg/median/percentile as value, opacity from weight of measurements
 *
 * @date 2012-08-07 (created)
  
 * @authors Stephan Plepelits
 */

#include "./interpolation.h"

struct measure_map_data_element {
  acid_maps::Pixel* element;
  float weight;
  bool within;
};

namespace acid_maps {

/**
 * @todo brief and description
 */
class MeasureMap : public Interpolation {
  /**
   * @todo brief and description
   */
  void interpolate(Size* tile_size, Pixel* dataset, int dataset_size, 
    Configuration* configuration, float interpolated_bitmap[], float weight_bitmap[]);
};

};  // namespace acid_maps

#endif  // LIB_SOURCE_INTERPOLATE_LINEAR_INTERPOLATION_H_

