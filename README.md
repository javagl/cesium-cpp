# cesium-cpp

Internal tests for cesium-native

---

Clone this repo:

    git clone https://github.com/javagl/cesium-cpp.git
    
Clone `cesium-native`:

    cd cesium-cpp
    cd extern
    git clone https://github.com/CesiumGS/cesium-native.git
    
Create makefiles:

    cmake -B build -S . -G "Visual Studio 16 2019" -A x64
    
Open `build/cesium-cpp.sln` with visual studio and look at
a pile of messy code in `CesiumCppMain.cpp`.

Compile it, and run the resulting executable:

    cd build
    cd Debug
    cesium-cpp.exe
    
    

    
