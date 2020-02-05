#ifndef COMMONINCLUDE_HPP_
#define COMMONINCLUDE_HPP_

// glad
#include <glad/glad.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"  // to print vect/mat with glm::to_string

namespace glm {
  template <typename T>
  bool operator<(tvec3<T> const &lhs, tvec3<T> const &rhs) {
    return (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y)
	|| (lhs.x < rhs.x && lhs.y < rhs.y)
	|| (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z < rhs.z));
  }
};

#endif  // COMMONINCLUDE_HPP_
