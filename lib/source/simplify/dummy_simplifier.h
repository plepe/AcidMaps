#ifndef LIB_SOURCE_SIMPLIFY_DUMMY_SIMPLIFIER_H_
#define LIB_SOURCE_SIMPLIFY_DUMMY_SIMPLIFIER_H_
/**
 * @file dummy_simplifier.h
 * @brief Dummy simplifier implementation
 *
 * Keeps the dataset untouched
 *
 * @date 2010-11-03
 * @authors Fabio R. Panettieri
 */

#include "./simplifier.h"

namespace acid_maps {

/**
 * @brief Keeps the dataset untouched
 */
class DummySimplifier : public Simplifier {
  /**
   * @brief It does nothing with the dataset
   */
  void simplify(Configuration* configuration);
};

};  // namespace acid_maps

#endif  // LIB_SOURCE_SIMPLIFY_DUMMY_SIMPLIFIER_H_
