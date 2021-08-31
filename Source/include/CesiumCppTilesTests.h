#pragma once

#include <Cesium3DTilesSelection/Tileset.h>

#include <glm/glm.hpp>

#include <cstdint>

namespace CesiumCpp {

  namespace TilesTests {

    /**
     * @brief Perform the specified camera flight.
     * 
     * This will move the camera, with the specified number of steps,
     * from the start- to the end position, with the specified delay
     * for each frame, printing the view update result f in each step.
     * 
     * @param tileset The tileset
     * @param startPosition The start position
     * @param endPosition The end position
     * @param steps The number of steps
     * @param frameDelayMs The delay, per frame, in milliseconds
     */
    void runCameraFlight(Cesium3DTilesSelection::Tileset& tileset,
      const glm::dvec3& startPosition, const glm::dvec3& endPosition,
      uint32_t steps, uint32_t frameDelayMs);

  } // namespace TilesTests

} // namespace CesiumCpp

