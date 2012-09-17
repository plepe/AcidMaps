/**
 * @file measure_map.cc
 * @brief Map of Measurements - avg/median/percentile as value, opacity from weight of measurements
 *
 * @date 2012-08-07 (created)
  
 * @authors Stephan Plepelits
 */

#include "./measure_map.h"

#include <cstring>
#include <cmath>
#include <list>
using namespace std;

#include "../constants/constants.h"
#include "../core/size.h"
#include "../core/pixel.h"
#include "../core/rectangle.h"
#include "../core/configuration.h"

namespace acid_maps {

float calculate_weight_from_distance(float distance_x, float distance_y, Configuration *configuration) {
  float distance;

  if (distance_x == 0 && distance_y == 0)
    return 1.0f;

  distance=std::sqrt((std::pow(distance_x, 2.0f) + std::pow(distance_y, 2.0f)));

  switch(configuration->distanceMethod) {
    case 0: // linear decline
      return 1.0f - distance / configuration->radius;

    case 1: // cos
      if(distance > configuration->radius)
	return 0.0f;

      return std::cos((distance / configuration->radius) * (M_PI / 2.0f));

    case 2: // cos squared
      if(distance > configuration->radius)
	return 0.0f;

      return std::pow(std::cos((distance / configuration->radius) *
        (M_PI / 2.0f)), 2.0f);
  }

          //weight = 1.0f / (std::pow(distance_x, 2.0f) + std::pow(distance_y, 2.0f));

}

float get_quantil_at(Configuration *configuration, list<measure_map_data_element> dataset_ordered, float accummulated_weight, float quantil) {
  list<measure_map_data_element>::iterator dataset_it, dataset_prev;
  float accummulated_value=0;
  float current_weight;
  bool is_first=true;
  bool found=false;

  if (quantil < 0.0f)
    quantil = 0.0f;
  if (quantil > 1.0f)
    quantil = 1.0f;

  for (dataset_it=dataset_ordered.begin(); dataset_it!=dataset_ordered.end(); ++dataset_it) {
    if (dataset_it->within) {

      // the weight of the first element will not be counted in the percentil-function
      if (is_first) {
	current_weight = (accummulated_weight - dataset_it->weight) * quantil;
	is_first=false;
      }

      // after the first element there might be a match
      else {
	current_weight -= dataset_it->weight;

	if (current_weight <= 0) {
	  // interpolate between current and next value
	  accummulated_value =
	    dataset_prev->element->value +
	    (dataset_it->element->value - dataset_prev->element->value)
	    * ((dataset_it->weight + current_weight) / dataset_it->weight);

	  found=true;
	  break;
	}
      }

      dataset_prev = dataset_it;
    }
  }

  // there was only one value
  if (!found) {
    accummulated_value = dataset_prev->element->value;
  }

  return accummulated_value;
}


float get_quantil(Configuration *configuration, list<measure_map_data_element> dataset_ordered, float accummulated_weight) {
  switch (configuration->quantil_method) {
    case 1:
      return (
	get_quantil_at(configuration, dataset_ordered, accummulated_weight,
		configuration->measure_quantil - configuration->quantil_offset) +
	get_quantil_at(configuration, dataset_ordered, accummulated_weight,
		configuration->measure_quantil + configuration->quantil_offset)
      )
      / 2.0f;

    case 2:
      return (
	get_quantil_at(configuration, dataset_ordered, accummulated_weight,
		configuration->measure_quantil - configuration->quantil_offset) +
	get_quantil_at(configuration, dataset_ordered, accummulated_weight,
		configuration->measure_quantil) * 2.0f +
	get_quantil_at(configuration, dataset_ordered, accummulated_weight,
		configuration->measure_quantil + configuration->quantil_offset)
      )
      / 4.0f;

    case 0:
    default:
      return get_quantil_at(configuration, dataset_ordered, accummulated_weight,
		configuration->measure_quantil);

  }

  return 0;
}

bool measure_map_data_element_cmp(measure_map_data_element first, measure_map_data_element second) {
  if(first.element->value < second.element->value)
    return true;

  return false;
}

/**
 * @todo
 */
void MeasureMap::interpolate(Size* tile_size, Pixel* dataset, int dataset_size,
  Configuration* configuration, float interpolated_bitmap[], float weight_bitmap[]) {
  Pixel* pixel;
  float distance_x, distance_y, weight, accummulated_value, accummulated_weight, current_weight;
  int radius=configuration->radius;
  time_t t=time(NULL);
  int bbox[4];

  // create an array dataset_ordered, which will hold pointers to elements of
  // the dataset and a distance which will be calculated for each tile-position
  list<measure_map_data_element> dataset_ordered;
  list<measure_map_data_element>::iterator dataset_it, dataset_prev;

  // insert whole dataset into the list
  for (int i = 0; i < dataset_size; i++) {
    measure_map_data_element element;

    pixel = dataset + i;
    element.element=pixel;
    element.weight=0;

    dataset_ordered.push_back(element);
  }

  // sort
  dataset_ordered.sort(measure_map_data_element_cmp);

  // for every pixel on the tile
  for (int y = 0; y < tile_size->height; y++) {
    for (int x = 0; x < tile_size->width; x++) {
      accummulated_weight = 0;

      // bbox to speed up exclusion of elements
      bbox[0] = x - radius;
      bbox[1] = x + radius;
      bbox[2] = y - radius;
      bbox[3] = y + radius;

      // calculate the weight for each dataset element
      for (dataset_it=dataset_ordered.begin(); dataset_it!=dataset_ordered.end(); ++dataset_it) {
	// outside? it's outside, we can continue with next element
	if ((dataset_it->element->x < bbox[0])||
	    (dataset_it->element->x > bbox[1])||
	    (dataset_it->element->y < bbox[2])||
	    (dataset_it->element->y > bbox[3])) {

	  dataset_it->within = false;
	  continue;
	}

        distance_x = static_cast<float>(x - dataset_it->element->x);
        distance_y = static_cast<float>(y - dataset_it->element->y);
	weight = calculate_weight_from_distance(distance_x, distance_y, configuration);

	dataset_it->weight=weight;
	if (weight >= 0) {
	  accummulated_weight+=weight;
	  dataset_it->within = true;
	}
	else {
	  dataset_it->within = false;
	}
      }

      // no measure points around position? -> use 0 as value
      if (accummulated_weight <= 0.0f) {
	accummulated_value=0;
      }

      // find percentile
      else {
	accummulated_value = get_quantil(configuration, dataset_ordered, accummulated_weight);
      }

      interpolated_bitmap[y * tile_size->width + x] = accummulated_value;
      weight_bitmap[y * tile_size->width + x] = accummulated_weight;
    }
  }
}

};  // namespace acid_maps

