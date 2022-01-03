# cesium-cpp

A small test project for [`cesium-native`](https://github.com/CesiumGS/cesium-native).

---

Clone this repo:

    git clone https://github.com/javagl/cesium-cpp.git
    
Clone `cesium-native`:

    cd cesium-cpp
    cd extern
    git clone https://github.com/CesiumGS/cesium-native.git
    
Create makefiles:

    cmake -B build -S . -G "Visual Studio 16 2019" -A x64
    
Open `build/cesium-cpp.sln` with visual studio, compile it, 
and run the resulting executable:

    cd build
    cd Debug
    cesium-cpp.exe
    
It will print a short usage hint. In order to read all glTF sample models
from a local directory (that contains the `model-index.json` file), use

    cesium-cpp -gltf C:/Samples/

    
