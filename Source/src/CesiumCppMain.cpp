
// This file is a mess. But it is only for internal testing.

#include "Cesium3DTilesSelection/registerAllTileContentTypes.h"
#include "Cesium3DTilesSelection/TilesetExternals.h"
#include "Cesium3DTilesSelection/Tileset.h"
#include "Cesium3DTilesSelection/ViewState.h"
#include "Cesium3DTilesSelection/ViewUpdateResult.h"
#include "Cesium3DTilesSelection/RasterOverlay.h"
#include "Cesium3DTilesSelection/GltfContent.h"

#include "CesiumGltf/Writer.h"

#include "Cesium3DTilesTestUtils.h"
#include "FileAssetAccessor.h"
#include "NullResourcePreparer.h"
#include "SimpleTaskProcessorEx.h"

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



void runTimed(const std::string& name, std::function<void()> function) {
	auto start = std::chrono::high_resolution_clock::now(); 
	function();
	auto stop = std::chrono::high_resolution_clock::now(); 
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start); 
	double ms = duration.count() / 1e6;
	std::cout << name << ": " << ms << " ms " << std::endl; 
}

std::vector<std::byte> readFile(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<std::byte> buffer(static_cast<size_t>(size));
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

void writeFile(const std::string& fileName, const std::vector<std::byte>& buffer) {
    std::ofstream file(fileName, std::ios::binary | std::ios::ate);
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}








void runLoggingExample() {

	SPDLOG_TRACE("A first trace message, {}", 12345);
	SPDLOG_ERROR("A first error message, {}", 23456);

	SPDLOG_TRACE("A trace message, {}", 12345);
	SPDLOG_ERROR("A error message, {}", 23456);

	SPDLOG_TRACE("A last trace message, {}", 12345);
	SPDLOG_ERROR("A last error message, {}", 23456);
}

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
		Cesium3DTilesSelection::ViewState viewState = Cesium3DTilesTests::createViewState(position, direction);
		r = tileset.updateView({viewState});
		Cesium3DTilesTests::printViewUpdateResult(r);

		SPDLOG_INFO("Calling Tileset::updateView DONE");

		Cesium3DTilesTests::sleepMsLogged(frameDelayMs);
	}
}


void runBasicCesiumNativeExmple(const std::string& tilesetUrl) {

	SPDLOG_INFO("Hello cesium-native!");

	// Start scope to see destructors called and handle uncaught errors
	try
	{
		SPDLOG_INFO("Calling Cesium3DTiles::registerAllTileContentTypes");
		Cesium3DTilesSelection::registerAllTileContentTypes();
		SPDLOG_INFO("Calling Cesium3DTiles::registerAllTileContentTypes DONE");


		SPDLOG_INFO("Creating Cesium3DTiles::TilesetExternals");
		Cesium3DTilesSelection::TilesetExternals externals{
			std::make_shared<Cesium3DTilesTests::FileAssetAccessor>(),
			std::make_shared<Cesium3DTilesTests::NullResourcePreparer>(),
			std::make_shared<Cesium3DTilesTests::SimpleTaskProcessorEx>()
		};
		SPDLOG_INFO("Creating Cesium3DTiles::TilesetExternals DONE");

		SPDLOG_INFO("Creating Cesium3DTiles::Tileset");
		Cesium3DTilesSelection::TilesetOptions options;
		options.maximumScreenSpaceError = 100;
		Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);
		SPDLOG_INFO("Creating Cesium3DTiles::Tileset DONE");

		const glm::dvec3 startPosition{ 0.0, 0.0, 5.1 };
		const glm::dvec3 endPosition{ 0.0, 0.0, 0.1 };
		const uint32_t steps = 10;
		const int32_t frameDurationMs = 200;

		runCameraFlight(tileset, startPosition, endPosition, steps, frameDurationMs);
	}
	catch (const std::exception& e) {
		SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
	}

	const int32_t exitDurationMs = 500;
	Cesium3DTilesTests::sleepMsLogged(exitDurationMs);
	SPDLOG_INFO("Bye cesium-native!");
}




void runLodTest(const std::string& tilesetUrl) {

	SPDLOG_INFO("Hello cesium-native!");

	// Start scope to see destructors called and handle uncaught errors
	try
	{
		SPDLOG_INFO("Calling Cesium3DTiles::registerAllTileContentTypes");
		Cesium3DTilesSelection::registerAllTileContentTypes();
		SPDLOG_INFO("Calling Cesium3DTiles::registerAllTileContentTypes DONE");


		SPDLOG_INFO("Creating Cesium3DTiles::TilesetExternals");
		Cesium3DTilesSelection::TilesetExternals externals{
			std::make_shared<Cesium3DTilesTests::FileAssetAccessor>(),
			std::make_shared<Cesium3DTilesTests::NullResourcePreparer>(),
			std::make_shared<Cesium3DTilesTests::SimpleTaskProcessorEx>()
		};
		SPDLOG_INFO("Creating Cesium3DTiles::TilesetExternals DONE");

		SPDLOG_INFO("Creating Cesium3DTiles::Tileset");
		Cesium3DTilesSelection::TilesetOptions options;
		options.maximumScreenSpaceError = 100.0;
		Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);
		SPDLOG_INFO("Creating Cesium3DTiles::Tileset DONE");


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
			Cesium3DTilesTests::sleepMsLogged(loadDurationMs);

			Cesium3DTilesSelection::ViewState viewState = Cesium3DTilesTests::createViewState(position, direction);
			Cesium3DTilesSelection::ViewUpdateResult r = tileset.updateView({viewState});
			Cesium3DTilesTests::printViewUpdateResult(r);
		}
	}
	catch (const std::exception& e) {
		SPDLOG_CRITICAL("Unhandled error: {0}", e.what());
	}

	const int32_t exitDurationMs = 500;
	Cesium3DTilesTests::sleepMsLogged(exitDurationMs);
	SPDLOG_INFO("Bye cesium-native!");
}


void runBasicCesiumNativeBenchmark(const std::string& tilesetUrl) {

	SPDLOG_INFO("Hello cesium-native!");
	try
	{
		Cesium3DTilesSelection::registerAllTileContentTypes();
		Cesium3DTilesSelection::TilesetExternals externals{
			std::make_shared<Cesium3DTilesTests::FileAssetAccessor>(),
			std::make_shared<Cesium3DTilesTests::NullResourcePreparer>(),
			std::make_shared<Cesium3DTilesTests::SimpleTaskProcessorEx>(0, true)
		};
		Cesium3DTilesSelection::TilesetOptions options;
		Cesium3DTilesSelection::Tileset tileset(externals, tilesetUrl, options);

		const glm::dvec3 startPosition{ 0.0, 0.0, 5.1 };
		const glm::dvec3 endPosition{ 0.0, 0.0, 0.1 };
		const uint32_t steps = 5000;
		const int32_t frameDurationMs = 0;

		int runs = 10;
		for (int r=0; r<runs; r++)
		{
			runCameraFlight(tileset, startPosition, endPosition, steps, frameDurationMs);
			runCameraFlight(tileset, endPosition, startPosition, steps, frameDurationMs);
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
        // if normals already exist, there is nothing to do
        auto normalIt = primitive.attributes.find("NORMAL");
        if (normalIt != primitive.attributes.end()) {
					SPDLOG_INFO("Found normals, removing them!");
					primitive.attributes.erase(normalIt);
        }
		  }
	 );

}


void testGltfProcessing()
{
	std::shared_ptr<spdlog::logger> pLogger = spdlog::default_logger();
	std::string inputFileName = "C:/Develop/KhronosGroup/glTF-Sample-Models/2.0/Lantern/glTF-Binary/Lantern.glb";
	std::string outputFileName = "C:/Develop/KhronosGroup/glTF-Sample-Models/2.0/Lantern/glTF-Binary/Lantern-OUT.glb";

	SPDLOG_INFO("Reading {0}", inputFileName);

	std::vector<std::byte> inputData = readFile(inputFileName);

	SPDLOG_INFO("Creating model");

	CesiumGltf::GltfReader gltfReader;
	CesiumGltf::ModelReaderResult modelReaderResult = gltfReader.readModel(inputData);
	if (!modelReaderResult.model)
	{
		SPDLOG_CRITICAL("Could not load {0}", inputFileName);
		return;
	}
	CesiumGltf::Model model = modelReaderResult.model.value();
	
	removeAllNormals(model);

	SPDLOG_INFO("Creating normals");
	runTimed("Normal creation", [&model]() {
		model.generateMissingNormalsSmooth();
	});


	CesiumGltf::WriteModelOptions writeModelOptions;
  writeModelOptions.exportType = CesiumGltf::GltfExportType::GLB;
	const CesiumGltf::WriteModelResult writeModelResult = CesiumGltf::writeModelAsEmbeddedBytes(model, writeModelOptions);
	const std::vector<std::byte> outputData = writeModelResult.gltfAssetBytes;

	writeFile(outputFileName, outputData);

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


