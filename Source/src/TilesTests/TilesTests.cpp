#include "TilesTests/TilesTests.h"

#include "Common/Common.h"
#include "Utils/Utils.h"

#include <Cesium3DTilesSelection/registerAllTileContentTypes.h>

#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>

namespace CesiumCpp {

namespace TilesTests {

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

void runCameraFlightExample(const std::string &tilesetUrl) {
  // Start scope to see destructors called and handle uncaught errors
  try {
    SPDLOG_INFO("Calling Cesium3DTilesSelection::registerAllTileContentTypes");
    Cesium3DTilesSelection::registerAllTileContentTypes();
    SPDLOG_INFO(
        "Calling Cesium3DTilesSelection::registerAllTileContentTypes DONE");

    SPDLOG_INFO("Creating Cesium3DTilesSelection::TilesetExternals");
    Cesium3DTilesSelection::TilesetExternals externals =
        CesiumCpp::Utils::createDefaultExternals();
    SPDLOG_INFO("Creating Cesium3DTilesSelection::TilesetExternals DONE");

    SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset");
    Cesium3DTilesSelection::TilesetOptions options;
    options.maximumScreenSpaceError = 100;
    Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);
    SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset DONE");

    const glm::dvec3 startPosition{0.0, 0.0, 5.1};
    const glm::dvec3 endPosition{0.0, 0.0, 0.1};
    const uint32_t steps = 10;
    const int32_t frameDurationMs = 200;

    runCameraFlight(tileset, startPosition, endPosition, steps,
                    frameDurationMs);
  } catch (const std::exception &e) {
    SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
  }

  const int32_t exitDurationMs = 500;
  CesiumCpp::Common::sleepMsLogged(exitDurationMs);
}

//============================================================================
// EXPERIMENTAL STUFF START
/*
bool waitFor(const std::string &name, CesiumAsync::AsyncSystem &asyncSystem,
             uint32_t maxWaitTimeMs, std::function<bool()> condition) {
  uint32_t sleepMs = 50;
  auto start = std::chrono::high_resolution_clock::now();
  while (true) {
    bool conditionFulfilled = condition();
    if (conditionFulfilled) {
      SPDLOG_INFO("Achieved state of {}", name);
      break;
    }
    asyncSystem.dispatchMainThreadTasks();
    auto current = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(current - start);
    double currentMs = duration.count() / 1e6;
    if (currentMs > maxWaitTimeMs) {
      SPDLOG_WARN("Waited {} ms for {}, bailing out", currentMs, name);
      return false;
    }
    SPDLOG_INFO("Waiting up to {} ms for {}, current wait time {} ms",
                maxWaitTimeMs, name, currentMs);
    CesiumCpp::Common::sleepMsLogged(sleepMs);
  }
  return true;
}

// Some of the things that are done here are derived from
// reverse engineering Tileset::addTileToLoadQueue,
// because ... (TODO phrase that diplomatically)

bool hasEmptyContentUri(const Cesium3DTilesSelection::Tile &tile) {
  const std::string *pStringID = std::get_if<std::string>(&tile.getTileID());
  return pStringID && pStringID->empty();
}

bool hasToLoadForImplicitTileset(
    const Cesium3DTilesSelection::Tile &tile,
    const Cesium3DTilesSelection::ImplicitTraversalInfo &implicitInfo) {
  const bool usingImplicitTiling = implicitInfo.usingImplicitQuadtreeTiling ||
                                   implicitInfo.usingImplicitOctreeTiling;
  const bool subtreeLoaded =
      implicitInfo.pCurrentNode && implicitInfo.pCurrentNode->subtree;
  return !subtreeLoaded;
}

bool hasContent(
    Cesium3DTilesSelection::Tile &tile,
    const Cesium3DTilesSelection::ImplicitTraversalInfo &implicitInfo) {
  const bool usingImplicitTiling = implicitInfo.usingImplicitQuadtreeTiling ||
                                   implicitInfo.usingImplicitOctreeTiling;
  if (usingImplicitTiling) {
    const bool subtreeLoaded =
        implicitInfo.pCurrentNode && implicitInfo.pCurrentNode->subtree;
    if (subtreeLoaded) {
      const bool implicitContentAvailability =
          implicitInfo.availability &
          CesiumGeometry::TileAvailabilityFlags::CONTENT_AVAILABLE;
      return implicitContentAvailability;
    } else {
      SPDLOG_INFO(
          "Called hasContent with implicit tile that does not have a subtree");
      return false;
    }
  }
  return !hasEmptyContentUri(tile);
}

void fullyLoad(
    Cesium3DTilesSelection::Tile &tile,
    const Cesium3DTilesSelection::ImplicitTraversalInfo &implicitInfo) {

  if (tile.getState() != Cesium3DTilesSelection::TileLoadState::Unloaded) {
    // Nothing to do here, I guess...
    return;
  }
  if (hasToLoadForImplicitTileset(tile, implicitInfo)) {
    if (hasContent(tile, implicitInfo)) {
      // This is not supposed to be called by clients,
      // but ... there is no other option
      tile.loadContent();
      return;
    }
    return;
  }
  if (hasContent(tile, implicitInfo)) {
    tile.loadContent();
    return;
  }
  if (tile.getState() == Cesium3DTilesSelection::TileLoadState::Unloaded) {
    tile.setState(Cesium3DTilesSelection::TileLoadState::ContentLoaded);
  }

  uint32_t maxWaitTimeMs = 1000;
  waitFor("Tile not being Unloaded", tile.getTileset()->getAsyncSystem(),
          maxWaitTimeMs, [&]() {
            return tile.getState() !=
                   Cesium3DTilesSelection::Tile::LoadState::Unloaded;
          });
}

/ *
void callUpdateView(Cesium3DTilesSelection::Tileset &tileset)
{
    glm::dvec3 position{0.0, 0.0, 0.0};
    glm::dvec3 direction{0.0, 0.0, -1.0};
    Cesium3DTilesSelection::ViewState viewState =
        CesiumCpp::Utils::createViewState(position, direction);
    tileset.updateView({viewState});
}
* /
void fullyTraverse(
    Cesium3DTilesSelection::Tile *tile,
    const Cesium3DTilesSelection::ImplicitTraversalInfo &implicitInfo,
    const std::string &indentation) {
  SPDLOG_INFO("{}Traversing {}", indentation,
              Cesium3DTilesSelection::TileIdUtilities::createTileIdString(
                  tile->getTileID()));
  fullyLoad(*tile, implicitInfo);
  for (Cesium3DTilesSelection::Tile &child : tile->getChildren()) {
    Cesium3DTilesSelection::ImplicitTraversalInfo childInfo(&child,
                                                            &implicitInfo);
    fullyTraverse(&child, childInfo, indentation + "  ");
  }
}

void fullyTraverse(Cesium3DTilesSelection::Tile *tile) {
  fullyTraverse(tile, Cesium3DTilesSelection::ImplicitTraversalInfo(tile), "");
}

void fullyTraverse(Cesium3DTilesSelection::Tileset &tileset) {

  SPDLOG_INFO("Traversing tileset");

  uint32_t maxWaitTimeMs = 1000;
  bool success =
      waitFor("Tile not being Unloaded", tileset.getAsyncSystem(),
              maxWaitTimeMs, [&]() { return tileset.getRootTile(); });
  if (!success) {
    return;
  }

  Cesium3DTilesSelection::Tile *rootTile = tileset.getRootTile();
  fullyTraverse(rootTile);

  SPDLOG_INFO("Traversing tileset DONE");
}

void runTraversalExample(const std::string &tilesetUrl) {
  // Start scope to see destructors called and handle uncaught errors
  try {
    SPDLOG_INFO("Calling Cesium3DTilesSelection::registerAllTileContentTypes");
    Cesium3DTilesSelection::registerAllTileContentTypes();
    SPDLOG_INFO(
        "Calling Cesium3DTilesSelection::registerAllTileContentTypes DONE");

    SPDLOG_INFO("Creating Cesium3DTilesSelection::TilesetExternals");
    Cesium3DTilesSelection::TilesetExternals externals =
        CesiumCpp::Utils::createDefaultExternals();
    SPDLOG_INFO("Creating Cesium3DTilesSelection::TilesetExternals DONE");

    SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset");
    Cesium3DTilesSelection::TilesetOptions options;
    options.maximumScreenSpaceError = 100;
    Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);
    SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset DONE");

    fullyTraverse(tileset);

  } catch (const std::exception &e) {
    SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
  }
}
*/
// EXPERIMENTAL STUFF END
//============================================================================

void runBasicCesiumNativeExample(const std::string &tilesetUrl) {

  SPDLOG_INFO("Hello cesium-native!");

  runCameraFlightExample(tilesetUrl);
  //runTraversalExample(tilesetUrl);

  SPDLOG_INFO("Bye cesium-native!");
}

} // namespace TilesTests

} // namespace CesiumCpp
