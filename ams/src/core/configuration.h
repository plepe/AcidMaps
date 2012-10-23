#ifndef LIB_SOURCE_CONFIGURATION_H_
#define LIB_SOURCE_CONFIGURATION_H_
/**
 * @file configuration.h
 * @brief 
 * 
 * @date 2010-11-02
 * @authors Fabio R. Panettieri
 * 
 * @todo Brief and description
 * 
 */

#include "./color.h"
#include "./point.h"
#include "./bounds.h"
#include "./size.h"

namespace acid_maps {

/**
 * @brief 
 * 
 * @todo Brief and description
 */
struct Configuration {
  /// Set of valued points, stored 3 floats per point. X, Y, Value
  Point* dataset;

  /// Number of points in the dataset
  int dataset_size;
  
  /// Desired simplify method
  int simplify_method;

  /// Size of the simplified sample
  int simplify_size;
  
  /// Requested tile bounds
  Bounds* bounds;
  
  /// Strategy used to interpolate missing values
  int interpolation_strategy;
  
  /// Radius
  int radius;
  
  /// Output image size
  Size* tile_size;
  
  /// Intervals array
  float* intervals;
  
  /// Intervals array
  Color* intervals_colors;
  
  /// Size of the itervals array
  int intervals_size;
  
  /// Determines how intervals are going to be accessed in the rendering step
  int renderer_type;
  
  /// How to calculate the weight depending on the distance of each points
  int distanceMethod;

  /// Image format
  int format;

  /// Quantil Map: which quantil should be used
  float quantil;
  float quantil_offset;
  int quantil_method;

  /// Gradient Renderer can calculate opacity from weight -> (value 1)
  int opacity_method;
  float *weights;
  float *weights_values;
  int weights_size;

  /// The zoom factor (pixels per data units)
  float zoom;

  /// Raster
  int raster;
};

};  // namespace acid_maps

#endif  // LIB_SOURCE_CONFIGURATION_H_

