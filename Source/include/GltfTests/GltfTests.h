#pragma once

#include <rapidjson/document.h>

#include <fstream>
#include <functional>
#include <optional>
#include <vector>

namespace CesiumCpp {
namespace GltfTests {

/**
 * @brief A description of a sample model variant.
 *
 * This corresponds to the entries in `variants` dictionary
 * in the `model-index.json` of the glTF Sample Models
 * repository, for example an entry like
 * `"glTF": "2CylinderEngine.gltf"`
 */
struct SampleModelVariant {

  /**
   * @brief The name of the variant.
   *
   * This can be `glTF`, `glTF-Embedded`, `glTF-Draco`, or `glTF-Binary`.
   */
  std::string variantName;

  /**
   * @brief The file name for the variant
   */
  std::string variantFileName;
};

/**
 * @brief A description of a sample model.
 *
 * This corresponds to one entry of the top-level array
 * in the `model-index.json` of the glTF Sample Models
 * repository.
 */
struct SampleModel {

  /**
   * @brief The name of the sample model
   */
  std::string name;

  /**
   * @brief The variants that are available for this
   * sample model.
   *
   * See {@link SampleModelVariant}.
   */
  std::vector<SampleModelVariant> sampleModelVariants;
};

/**
 * @brief Interface for something that can read data.
 *
 * It receives a relative path, and returns the data from
 * the file.
 */
typedef std::function<std::vector<std::byte>(const std::string &relativePath)>
    DataReader;

/**
 * @brief Creates a {@link DataReader} for the given base path.
 *
 * It will return a {@link DataReader} that resolves the relative
 * paths that it receives against the given base path.
 */
DataReader createFileReader(const std::string &basePath);

/**
 * @brief Read the list of available sample models.
 *
 * This receives a {@link DataReader} for the `model-index.json` from
 * the glTF Sample Models repository, and returns information about
 * all sample models that are described in this file, as a list of
 * {@link SampleModel} objects.
 *
 * If the file or, one of its models, or one of its variants cannot
 * be read, then an error message will be printed, and the respective
 * entry will be omited.
 *
 * @param dataReader The {@link DataReader}
 * @return The {@link SampleModel}
 */
std::vector<SampleModel> readSampleModels(const DataReader &dataReader);

/**
 * @brief Try to create a {@link @SampleModel} from the given JSON value.
 *
 * If the entry cannot be processed, then an error message
 * will be printed and an empty optional will be returned.
 *
 * @param index The index in the top-level array of the `model-index.json`
 * @param entry The entry
 * @return The {@link SampleModel}, or an empty optional if the
 * entry could not be processed.
 */
std::optional<SampleModel> processSampleModel(rapidjson::SizeType index,
                                              const rapidjson::Value &entry);

/**
 * @brief Read the sample model variants from the given JSON value.
 *
 * This receives the `variants` array of a sample model from the
 * `model-index.json`, and returns an array of all
 * {@link SampleModelVariant} objects that can be found.
 *
 * If any entry cannot be processed, then an error message
 * will be printed and the entry will be omitted.
 *
 * @param dataReader The {@link DataReader}
 * @return The {@link SampleModel}
 */
std::vector<SampleModelVariant>
processVariants(const std::string &name, const rapidjson::Value &variants);

/**
 * @brief Internal method to read a single model.
 *
 * This may print information-, warning-, or error messages
 * about the reading process.
 *
 * @param dataReader The {@link DataReader} for the model file.
 * @param name The name of the sample model
 * @param variantName The variant name
 * @param variantFileName The file name for the sample model file
 * that the data reader was created for.
 * @return Whether the model could be read.
 */
bool testReadModel(const DataReader &dataReader, const std::string &name,
                   const std::string &variantName,
                   const std::string &variantFileName);

/**
 * @brief Read all sample models from the given base path.
 *
 * The path is assumed to contain the `model-index.json` file
 * as it is contained in the the glTF Sample Models repository.
 *
 * This may print information-, warning-, or error messages
 * about the reading process.
 *
 * @param basePath The base path
 * @return Whether the models could all be read
 */
bool testReadSampleModels(const std::string &basePath);

/**
 * @brief Read a single sample model.
 *
 * This may print information-, warning-, or error messages
 * about the reading process.
 *
 * @param basePath The base path
 * @param name The name of the sample model
 * @param variantName The variant name
 * @param variantFileName The file name for the sample model file
 * @return Whether the model could be read.
 */
bool testReadSampleModel(const std::string &basePath, const std::string &name,
                         const std::string &variantName,
                         const std::string &variantFileName);

} // namespace GltfTests

} // namespace CesiumCpp
