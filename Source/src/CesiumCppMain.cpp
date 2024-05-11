
#include "Common/Common.h"
#include "GltfTests/GltfTests.h"
#include "TilesTests/TilesTests.h"
#include "Utils/Utils.h"

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Cesium3DTilesContent/registerAllTileContentTypes.h>

#include <Cesium3DTilesSelection/Tileset.h>
#include <Cesium3DTilesSelection/TilesetExternals.h>

void runViewUpdate(Cesium3DTilesSelection::Tileset &tileset,
                   const glm::dvec3 position, const glm::dvec3 direction) {

  std::cout << "Run view update" << std::endl;
  std::cout << "  position : " << glm::to_string(position) << std::endl;
  std::cout << "  direction: " << glm::to_string(direction) << std::endl;

  Cesium3DTilesSelection::ViewState viewState =
      CesiumCpp::Utils::createViewState(position, direction);
  Cesium3DTilesSelection::ViewUpdateResult r =
      tileset.updateViewOffline({viewState});
  CesiumCpp::Utils::printViewUpdateResult(r);
}

void testExternalLoading(const std::string &tilesetUrl) {

  Cesium3DTilesContent::registerAllTileContentTypes();
  Cesium3DTilesSelection::TilesetExternals externals =
      CesiumCpp::Utils::createDefaultExternals();
  externals.pLogger->set_level(spdlog::level::trace);
  Cesium3DTilesSelection::TilesetOptions options;
  Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);

  runViewUpdate(tileset, {0.0, 0.0, 5.0}, {0.0, 0.0, -1.0});
  runViewUpdate(tileset, {3.0, 0.0, 0.0}, {0.0, 0.0, -1.0});
}
int main(int argc, char **argv) {

  const std::string url = "C:/Data/externalsWithTransform/tileset.json";
  testExternalLoading(url);
  return 0;
}
