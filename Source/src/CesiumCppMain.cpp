
#include "CesiumCppUtils.h"
#include "CesiumCppCommon.h"
#include "CesiumCppTilesTests.h"

#include "Cesium3DTilesSelection/registerAllTileContentTypes.h"
#include "Cesium3DTilesSelection/TilesetExternals.h"
#include "Cesium3DTilesSelection/Tileset.h"
#include "Cesium3DTilesSelection/ViewState.h"
#include "Cesium3DTilesSelection/ViewUpdateResult.h"
#include "Cesium3DTilesSelection/RasterOverlay.h"
#include "Cesium3DTilesSelection/GltfContent.h"

#include "CesiumGltf/Writer.h"

#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstddef>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <filesystem>


void runBasicCesiumNativeExmple(const std::string& tilesetUrl) {

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




void runLodTest(const std::string& tilesetUrl) {

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
		options.maximumScreenSpaceError = 100.0;
		Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);
		SPDLOG_INFO("Creating Cesium3DTilesSelection::Tileset DONE");


		// XXX Some hard-wired values, extracted from the Sandcastle
		// showing the LOD example, at which (for a certain screen
		// size...) the different LOD levels should appear...
		const glm::dvec3 direction{ 0.0, 0.0, -1.0 };
		const glm::dvec3 position0{ 0.0, 0.0, 100.0 };
		const glm::dvec3 position1{ 0.0, 0.0, 20.0 };
		const glm::dvec3 position2{ 0.0, 0.0, 10.0 };
		const glm::dvec3 position3{ 0.0, 0.0, 3.0 };
		std::vector<glm::dvec3> positions{ position0, position1, position2, position3 };

		for (const auto& position : positions) {
			SPDLOG_INFO("Camera at {}", glm::to_string(position));

			const int32_t loadDurationMs = 2500;
			CesiumCpp::Common::sleepMsLogged(loadDurationMs);

			Cesium3DTilesSelection::ViewState viewState = CesiumCpp::Utils::createViewState(position, direction);
			Cesium3DTilesSelection::ViewUpdateResult r = tileset.updateView({viewState});
			CesiumCpp::Utils::printViewUpdateResult(r);
		}
	}
	catch (const std::exception& e) {
		SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
	}

	const int32_t exitDurationMs = 500;
	CesiumCpp::Common::sleepMsLogged(exitDurationMs);
	SPDLOG_INFO("Bye cesium-native!");
}


void runBasicCesiumNativeBenchmark(const std::string& tilesetUrl) {

	SPDLOG_INFO("Hello cesium-native!");
	try
	{
		Cesium3DTilesSelection::registerAllTileContentTypes();
		Cesium3DTilesSelection::TilesetExternals externals = 
			CesiumCpp::Utils::createDefaultExternals();
		Cesium3DTilesSelection::TilesetOptions options;
		Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);

		const glm::dvec3 startPosition{ 0.0, 0.0, 5.1 };
		const glm::dvec3 endPosition{ 0.0, 0.0, 0.1 };
		const uint32_t steps = 5000;
		const int32_t frameDurationMs = 0;

		int runs = 10;
		for (int r=0; r<runs; r++)
		{
			CesiumCpp::TilesTests::runCameraFlight(
				tileset, startPosition, endPosition, steps, frameDurationMs);
			CesiumCpp::TilesTests::runCameraFlight(
				tileset, endPosition, startPosition, steps, frameDurationMs);
		}
	}
	catch (const std::exception& e) {
		SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
	}
	SPDLOG_INFO("Bye cesium-native!");
}





void removeAllNormals(CesiumGltf::Model& gltf)
{
  gltf.forEachPrimitiveInScene(
      -1,
      [](CesiumGltf::Model& gltf_,
         CesiumGltf::Node& /*node*/,
         CesiumGltf::Mesh& /*mesh*/,
         CesiumGltf::MeshPrimitive& primitive,
         const glm::dmat4& /*transform*/) {
        auto normalIt = primitive.attributes.find("NORMAL");
        if (normalIt != primitive.attributes.end()) {
					//SPDLOG_INFO("Found normals, removing them!");
					primitive.attributes.erase(normalIt);
        }
		  }
	 );

}


void testGltfProcessing()
{
	std::shared_ptr<spdlog::logger> pLogger = spdlog::default_logger();
	std::string inputFileName = "C:/Develop/KhronosGroup/glTF-Sample-Models/2.0/MetalRoughSpheres/glTF-Binary/MetalRoughSpheres.glb";
	std::string outputFileName = "C:/Develop/KhronosGroup/glTF-Sample-Models/2.0/MetalRoughSpheres/glTF-Binary/MetalRoughSpheres-OUT.gltf";

	SPDLOG_INFO("Reading {0}", inputFileName);

	std::vector<std::byte> inputData = CesiumCpp::Common::readFile(inputFileName);

	SPDLOG_INFO("Creating model");

	CesiumGltf::GltfReader gltfReader;
	CesiumGltf::ModelReaderResult modelReaderResult = gltfReader.readModel(inputData);
	if (!modelReaderResult.model)
	{
		SPDLOG_CRITICAL("Could not load {0}", inputFileName);
		return;
	}
	CesiumGltf::Model model = modelReaderResult.model.value();
	
	SPDLOG_INFO("Removing all existing normals for test");
	removeAllNormals(model);

	SPDLOG_INFO("Creating normals");
	CesiumCpp::Common::runTimed("Normal creation", [&model]() {
    model.generateMissingNormalsSmooth();
	});

	/*
	int runs = 1000;
	double totalMs = 0.0;
	for (int i=0; i<runs; i++) {
		removeAllNormals(model);
		auto start = std::chrono::high_resolution_clock::now();
		model.generateMissingNormalsSmooth();
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
		double ms = duration.count() / 1e6;
		totalMs += ms;
	}
	SPDLOG_INFO("Total duration for creating normals {} times: {}", runs, totalMs);
	*/

	SPDLOG_INFO("Writing model");
	CesiumGltf::WriteModelOptions writeModelOptions;
	writeModelOptions.prettyPrint = true;
	writeModelOptions.autoConvertDataToBase64 = true;
  writeModelOptions.exportType = CesiumGltf::GltfExportType::GLTF;
	const CesiumGltf::WriteModelResult writeModelResult = CesiumGltf::writeModelAsEmbeddedBytes(model, writeModelOptions);
	const std::vector<std::byte> outputData = writeModelResult.gltfAssetBytes;

	SPDLOG_INFO("Writing {0}", outputFileName);
	CesiumCpp::Common::writeFile(outputFileName, outputData);

	SPDLOG_INFO("Done");
}



int main(int argc, char** argv) {

	//runBasicLoggingExample();
	//if (true) return 0;

	//std::string lodTilesetUrl = "C:/Develop/CesiumUnreal/Data/Icospheres/tileset.json";
	//runLodTest(lodTilesetUrl);
	//if (true) return 0;

	testGltfProcessing();
	if (true) return 0;

	
	//CesiumGltf::ImageCesium target;
	//CesiumGltf::PixelRectangle targetPixels;
	//CesiumGltf::ImageCesium source;
	//CesiumGltf::PixelRectangle sourcePixels;
	//CesiumGltf::ImageManipulation::blitImage(target, targetPixels, source, sourcePixels);


	std::string defaultTilesetUrl = "C:/Develop/CesiumUnreal/cesium-cpp/Data/Icospheres/tileset.json";
	
	std::string tilesetUrl = defaultTilesetUrl;
	//tilesetUrl = "C:/Develop/CesiumUnreal/Data/Planes/tileset.json";
	//tilesetUrl = "C:/Develop/CesiumUnreal/Data/AddTileset/tileset.json";
	//tilesetUrl = "C:/Develop/CesiumUnreal/ExternalData/3dtiles/tileset.json";
	if (argc > 1) {
		tilesetUrl = argv[1];
	}

	//testOverlayStuff(tilesetUrl);

	runBasicCesiumNativeExmple(tilesetUrl);


	return 0;
}


