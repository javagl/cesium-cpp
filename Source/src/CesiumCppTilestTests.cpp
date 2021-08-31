#include "CesiumCppTilesTests.h"

#include "CesiumCppCommon.h"
#include "CesiumCppUtils.h"

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

namespace CesiumCpp {

  namespace TilesTests {

    void runCameraFlight(Cesium3DTilesSelection::Tileset& tileset,
      const glm::dvec3& startPosition, const glm::dvec3& endPosition,
      uint32_t steps, uint32_t frameDelayMs) {

      for (int i = 0; i < steps; i++)
      {
        SPDLOG_INFO("Frame {0} of {1}", i, steps);

        double alpha = (double)i / ((double)steps - 1.0);
        glm::dvec3 position = glm::mix(startPosition, endPosition, alpha);

        SPDLOG_INFO("Camera at {}", glm::to_string(position));

        SPDLOG_INFO("Calling Tileset::updateView");

        Cesium3DTilesSelection::ViewUpdateResult r;
        glm::dvec3 direction{ 0.0, 0.0, -1.0 };
        Cesium3DTilesSelection::ViewState viewState = 
          CesiumCpp::Utils::createViewState(position, direction);
        r = tileset.updateView({ viewState });
        CesiumCpp::Utils::printViewUpdateResult(r);

        SPDLOG_INFO("Calling Tileset::updateView DONE");

        CesiumCpp::Common::sleepMsLogged(frameDelayMs);
      }
    }

  } // namespace TilesTests

} // namespace CesiumCpp

