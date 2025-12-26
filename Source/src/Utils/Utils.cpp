
#include "Utils/Utils.h"

#include "CesiumNativeImpl/FileAssetAccessor.h"
#include "CesiumNativeImpl/NullResourcePreparer.h"
#include "CesiumNativeImpl/SimpleTaskProcessorEx.h"

#include <Cesium3DTilesSelection/Tile.h>
#include <Cesium3DTilesSelection/TileID.h>
#include <Cesium3DTilesSelection/ViewState.h>
#include <Cesium3DTilesSelection/ViewUpdateResult.h>
#include <Cesium3DTilesSelection/TilesetExternals.h>

#include <CesiumGeometry/OctreeTileID.h>
#include <CesiumGeometry/QuadtreeTileID.h>

#include <spdlog/spdlog.h>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

namespace CesiumCpp {

  namespace Utils {

    namespace {
      constexpr double MATH_PI = 3.14159265358979323846;
    }

		Cesium3DTilesSelection::TilesetExternals createDefaultExternals() {
      CesiumAsync::AsyncSystem asyncSystem{
        std::make_shared<CesiumCpp::CesiumNativeImpl::SimpleTaskProcessorEx>(0, true)
      };
		  Cesium3DTilesSelection::TilesetExternals externals{
			  std::make_shared<CesiumCpp::CesiumNativeImpl::FileAssetAccessor>(),
			  std::make_shared<CesiumCpp::CesiumNativeImpl::NullResourcePreparer>(),
			  asyncSystem
		  };
      return externals;
    }


    Cesium3DTilesSelection::ViewState
      createViewState(const glm::dvec3& p, const glm::dvec3 d) {
      const glm::dvec3 position{ p };
      const glm::dvec3 direction{ d };
      const glm::dvec3 up{ 0.0, 1.0, 0.0 };
      const glm::dvec2 viewportSize{ 800.0, 600.0 };
      const double horizontalFieldOfView = MATH_PI / 3.0;
      const double verticalFieldOfView = MATH_PI / 3.0;
      Cesium3DTilesSelection::ViewState viewState = Cesium3DTilesSelection::ViewState::create(
        position,
        direction,
        up,
        viewportSize,
        horizontalFieldOfView,
        verticalFieldOfView);
      return viewState;
    }

    Cesium3DTilesSelection::ViewState createViewState() {
      return createViewState(glm::dvec3{ 0.0, 0.0, 0.0 }, glm::dvec3{ 0.0, 0.0, -1.0 });
    }


    namespace {
      int32_t computeHeight(Cesium3DTilesSelection::Tile const* tile) {
        int32_t height = 0;
        Cesium3DTilesSelection::Tile const* t = tile;
        while (t != nullptr) {
          height++;
          t = t->getParent();
        }
        return height;
      }

    } // namespace

    std::string viewUpdateResultToString(const Cesium3DTilesSelection::ViewUpdateResult& r) {
      int w = 35;
      std::ostringstream s;

      s << std::setw(w)
        << "tilesToRenderThisFrame : " << r.tilesToRenderThisFrame.size()
        << std::endl;

      // TODO Here's the option to add more details. Could be a parameter...
      bool details = true;
      if (details) {
        for (auto const& tile : r.tilesToRenderThisFrame) {
          s << std::setw(w) << ""
            << "  ID " << Cesium3DTilesSelection::TileIdUtilities::createTileIdString(tile->getTileID()) << " error "
            << tile->getGeometricError() << " height " << computeHeight(tile)
            << std::endl;
        }
      }

      s << std::setw(w) << "tilesVisited : " << r.tilesVisited << std::endl;
      s << std::setw(w) << "tilesCulled : " << r.tilesCulled << std::endl;
      s << std::setw(w) << "maxDepthVisited : " << r.maxDepthVisited << std::endl;
      return s.str();
    }

    void printViewUpdateResult(const Cesium3DTilesSelection::ViewUpdateResult& r) {
      SPDLOG_INFO("ViewUpdateResult:\n" + viewUpdateResultToString(r));
    }

  } // namespace Utils
} // namespace CesiumCpp

