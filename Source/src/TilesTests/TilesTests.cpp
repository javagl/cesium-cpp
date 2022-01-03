#include "TilesTests/TilesTests.h"

#include "Common/Common.h"
#include "Utils/Utils.h"

#include <Cesium3DTilesSelection/registerAllTileContentTypes.h>

#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>

namespace CesiumCpp {

namespace TilesTests {


void runBasicCesiumNativeExample(const std::string& tilesetUrl) {

	SPDLOG_INFO("Hello cesium-native!");

	// Start scope to see destructors called and handle uncaught errors
	try
	{
		SPDLOG_INFO("Calling Cesium3DTilesSelection::registerAllTileContentTypes");
		Cesium3DTilesSelection::registerAllTileContentTypes();
		SPDLOG_INFO("Calling Cesium3DTilesSelection::registerAllTileContentTypes DONE");


		SPDLOG_INFO("Creating Cesium3DTilesSelection::TilesetExternals");
		Cesium3DTilesSelection::TilesetExternals externals =
			CesiumCpp::Utils::createDefaultExternals();
		SPDLOG_INFO("Creating Cesium3DTilesSelection::TilesetExternals DONE");

		SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset");
		Cesium3DTilesSelection::TilesetOptions options;
		options.maximumScreenSpaceError = 100;
		Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);
		SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset DONE");

		const glm::dvec3 startPosition{ 0.0, 0.0, 5.1 };
		const glm::dvec3 endPosition{ 0.0, 0.0, 0.1 };
		const uint32_t steps = 10;
		const int32_t frameDurationMs = 200;

		CesiumCpp::TilesTests::runCameraFlight(
			tileset, startPosition, endPosition, steps, frameDurationMs);
	}
	catch (const std::exception& e) {
		SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
	}

	const int32_t exitDurationMs = 500;
	CesiumCpp::Common::sleepMsLogged(exitDurationMs);
	SPDLOG_INFO("Bye cesium-native!");
}


void runCameraFlight(Cesium3DTilesSelection::Tileset &tileset,
                     const glm::dvec3 &startPosition,
                     const glm::dvec3 &endPosition, uint32_t steps,
                     uint32_t frameDelayMs) {

  for (int i = 0; i < steps; i++) {
    SPDLOG_INFO("Frame {0} of {1}", i, steps);

    double alpha = (double)i / ((double)steps - 1.0);
    glm::dvec3 position = glm::mix(startPosition, endPosition, alpha);

    SPDLOG_INFO("Camera at {}", glm::to_string(position));

    SPDLOG_INFO("Calling Tileset::updateView");

    Cesium3DTilesSelection::ViewUpdateResult r;
    glm::dvec3 direction{0.0, 0.0, -1.0};
    Cesium3DTilesSelection::ViewState viewState =
        CesiumCpp::Utils::createViewState(position, direction);
    r = tileset.updateView({viewState});
    CesiumCpp::Utils::printViewUpdateResult(r);

    SPDLOG_INFO("Calling Tileset::updateView DONE");

    CesiumCpp::Common::sleepMsLogged(frameDelayMs);
  }
}

} // namespace TilesTests

} // namespace CesiumCpp
