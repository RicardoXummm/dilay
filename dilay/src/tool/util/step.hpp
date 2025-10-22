/* This file is part of Dilay
 * Copyright © 2015-2018 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_TOOL_UTIL_STEP
#define DILAY_TOOL_UTIL_STEP

#include <functional>
#include <glm/fwd.hpp>
#include "macro.hpp"

class ToolUtilStep
{
public:
  DECLARE_BIG3 (ToolUtilStep)

  const glm::vec3& position () const;
  void             position (const glm::vec3&);
  void             stepWidth (float);
  void             step (const glm::vec3&, const std::function<bool(const glm::vec3&)>&);

private:
  IMPLEMENTATION
};

#endif
