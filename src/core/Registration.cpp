#include "../../include/core/Engine.h"
#include "../../include/core/MethodDescriptor.h"

#include <ogdf/basic/Graph.h>

#include <rttr/registration.h>

namespace engine {

std::vector<MethodDescriptor> getAvailableMethods() {
  return Engine::instance().getAvailableMethods();
}

void registerTypes() {
  using namespace rttr;

  registration::method("setSeed", &ogdf::setSeed)(
      metadata("category", "engine"),
      metadata("description", "Sets the seed for random number generation"));
  registration::method("randomNumber", &ogdf::randomNumber)(
      metadata("category", "engine"),
      metadata("description", "Generates a random number"));

  registration::method("getAvailableMethods", &getAvailableMethods)(
      metadata("category", "engine"),
      metadata("description", "Retrieves available methods"));
}

} // namespace engine