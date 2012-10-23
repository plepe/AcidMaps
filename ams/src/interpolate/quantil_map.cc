/**
 * @file quantil_map.cc
 * @brief Calculate quantil of surrounding data points
 *
 * @date 2012-08-07 (created)
  
 * @authors Stephan Plepelits
 */

#include "./quantil_map.h"

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

float get_quantil(Configuration *configuration, list<quantil_map_data_element> dataset_ordered, float accummulated_weight, float* quantil_values, int quantil_values_count) {
  list<quantil_map_data_element>::iterator dataset_it, dataset_prev;
  float accummulated_value=0;
  bool is_first=true;
  bool found=false;
  int quantil_values_index;
  float current_weight = 0.0f;
  float current_quantils[4];

  // process through data points until quantils of accummulated_weight are
  // reached
  for (dataset_it=dataset_ordered.begin(); dataset_it!=dataset_ordered.end(); ++dataset_it) {
    if (dataset_it->within) {

      // the weight of the first element will not be counted in the percentil-function
      if (is_first) {
	for (quantil_values_index = 0;
	  quantil_values_index < quantil_values_count;
	  quantil_values_index++) {

	  current_quantils[quantil_values_index] =
	    (accummulated_weight - dataset_it->weight) *
	    quantil_values[quantil_values_index];
	}

	quantil_values_index = 0;

	is_first=false;
      }

      // after the first element there might be a match
      else {
	current_weight += dataset_it->weight;

	while (current_weight >= current_quantils[quantil_values_index]) {
	  // interpolate between current and next value
	  accummulated_value +=
	    dataset_prev->element->value +
	    (dataset_it->element->value - dataset_prev->element->value)
	    * ((current_quantils[quantil_values_index] -
	        current_weight + dataset_it->weight)
	    / dataset_it->weight);

	  // after calculating all required quantils, break
	  if(++quantil_values_index == quantil_values_count)
	    break;
	}
      }

      dataset_prev = dataset_it;

      // after calculating all required quantils, break
      if(quantil_values_index == quantil_values_count)
	break;
    }
  }

  // there was only one value
  if (quantil_values_index == 0) {
    accummulated_value = dataset_prev->element->value;
  }
  // otherwise devide accummulated_value by count of found quantils
  else {
    accummulated_value = accummulated_value / quantil_values_index;
  }

  return accummulated_value;
}

int quantil_configure(Configuration *configuration, float* quantil_values) {
  int quantil_values_count;
  int i;

  switch (configuration->quantil_method) {
    case 1:
      quantil_values[0] =
        configuration->quantil - configuration->quantil_offset;
      quantil_values[1] =
        configuration->quantil + configuration->quantil_offset;
      quantil_values_count = 2;
      break;
    case 2:
      quantil_values[0] =
        configuration->quantil - configuration->quantil_offset;
      quantil_values[1] =
        configuration->quantil;
      quantil_values[2] =
        configuration->quantil;
      quantil_values[3] =
        configuration->quantil + configuration->quantil_offset;
      quantil_values_count = 4;
      break;
    case 0:
    default:
      quantil_values[0] = configuration->quantil;
      quantil_values_count = 1;
      break;
  }

  for( i = 0; i < quantil_values_count; i++ ) {
    if (quantil_values[i] < 0.0f)
      quantil_values[i] = 0.0f;
    if (quantil_values[i] > 1.0f)
      quantil_values[i] = 1.0f;
  }

  return quantil_values_count;
}

bool quantil_map_data_element_cmp(quantil_map_data_element first, quantil_map_data_element second) {
  if(first.element->value < second.element->value)
    return true;

  return false;
}

/**
 * @todo
 */
void QuantilMap::interpolate(Size* tile_size, Pixel* dataset, int dataset_size,
  Configuration* configuration, float interpolated_bitmap[], float weight_bitmap[]) {
  Pixel* pixel;
  float distance_x, distance_y, weight, accummulated_value, accummulated_weight, current_weight;
  int radius=configuration->radius;
  time_t t=time(NULL);
  int bbox[4];
  int raster=configuration->raster;
  float quantil_values[4];
  int quantil_values_count;

  quantil_values_count = quantil_configure(configuration, quantil_values);

  // create an array dataset_ordered, which will hold pointers to elements of
  // the dataset and a distance which will be calculated for each tile-position
  list<quantil_map_data_element> dataset_ordered;
  list<quantil_map_data_element>::iterator dataset_it, dataset_prev;

  // insert whole dataset into the list
  for (int i = 0; i < dataset_size; i++) {
    quantil_map_data_element element;

    pixel = dataset + i;
    element.element=pixel;
    element.weight=0;

    dataset_ordered.push_back(element);
  }

  // sort
  dataset_ordered.sort(quantil_map_data_element_cmp);

  // for every pixel on the tile
  for (int y = 0; y < tile_size->height; y+=raster) {
    for (int x = 0; x < tile_size->width; x+=raster) {
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

      // no data points around position? -> use 0 as value
      if (accummulated_weight <= 0.0f) {
	accummulated_value=0;
      }

      // find percentile
      else {
	accummulated_value = get_quantil(configuration, dataset_ordered, accummulated_weight, quantil_values, quantil_values_count);
      }

      for(int y1 = y; y1 <= y + raster - 1; y1++)
	for(int x1 = x; x1 <= x + raster - 1; x1++) {
	  interpolated_bitmap[y1 * tile_size->width + x1] = accummulated_value;
	  weight_bitmap[y1 * tile_size->width + x1] = accummulated_weight;
	}
    }
  }
}

};  // namespace acid_maps

