/**
 * @file copy_simplifier.cc
 * @brief Copy simplifier implementation
 *
 * @date 2010-11-03
 * @authors Fabio R. Panettieri
 */

#include "../constants/constants.h"
#include "../core/point.h"
#include "./copy_simplifier.h"
#include <cstring>

namespace acid_maps {

void CopySimplifier::simplify(Point* dataset, int dataset_size, Point* simplified_dataset, int& simplify_size) {
  if (dataset_size < simplify_size) simplify_size = dataset_size;
  std::memcpy((Point*)simplified_dataset, (Point*)dataset, sizeof(Point) * simplify_size);
}

};  // namespace acid_maps
