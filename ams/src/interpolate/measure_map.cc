/**
 * @file measure_map.cc
 * @brief Map of Measurements - avg/median/percentile as value, opacity for density of measurements
 *
 * @date 2012-08-07 (created)
  
 * @authors Stephan Plepelits
 */

#include "./measure_map.h"

#include <cstring>
#include <cmath>
#include <list>
#include <stdio.h>
using namespace std;

#include "../constants/constants.h"
#include "../core/size.h"
#include "../core/pixel.h"
#include "../core/rectangle.h"

namespace acid_maps {

bool measure_map_data_element_cmp(measure_map_data_element first, measure_map_data_element second) {
  if(first.element->value < second.element->value)
    return true;

  return false;
}

/**
 * @todo
 */
void MeasureMap::interpolate(Size* tile_size, Pixel* dataset, int dataset_size,
  int radius, float interpolated_bitmap[]) {
  Pixel* pixel;
  float distance_x, distance_y, weight, accummulated_value, accummulated_weight;

  // create an array dataset_ordered, which will hold pointers to elements of
  // the dataset and a distance which will be calculated for each tile-position
  list<measure_map_data_element> dataset_ordered;
  list<measure_map_data_element>::iterator dataset_it;

  // insert whole dataset into the list
  for (int i = 0; i < dataset_size; i++) {
    measure_map_data_element element;

    pixel = dataset + i;
    element.element=pixel;
    element.distance=0;

    dataset_ordered.push_back(element);
  }

  // sort
  dataset_ordered.sort(measure_map_data_element_cmp);

  for (int y = 0; y < tile_size->height; y++) {
    for (int x = 0; x < tile_size->width; x++) {
      accummulated_value = 0;
      accummulated_weight = 0;
      for (int i = 0; i < dataset_size; i++) {
        pixel = dataset + i;
        distance_x = static_cast<float>(x - pixel->x);
        distance_y = static_cast<float>(y - pixel->y);
        if (distance_x == 0 && distance_y == 0) {
          weight = 1.0f;
        } else {
          weight = 1.0f / (std::pow(distance_x, 2.0f) + std::pow(distance_y, 2.0f));
        }
        accummulated_value += pixel->value  * weight;
        accummulated_weight += weight;
      }
      interpolated_bitmap[y * tile_size->width + x] = accummulated_value / accummulated_weight;
    }
  }
}

};  // namespace acid_maps

