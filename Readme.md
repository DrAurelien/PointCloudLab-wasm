# Configure

## Prerequisistes

### Emscripten SDK

[Emscripten](https://github.com/emscripten-core/emsdk) is required to build WebAssembly source files.

Then, build and install the SDK
```
git clone https://github.com/emscripten-core/emsdk.git
git pull
./emsdk install latest
./emsdk activate latest
./emsdk_env.bat
```

### CMake

[CMake](https://cmake.org) v3.10+ is required to configure the toolchain and build the project.
You will also need a ganarator installed (I suggest using [Ninja](https://ninja-build.org/) for instance).

### HTTP server

Although this project only provides a quite simple HTML standalone page, you will still need to run
it under a HTTP server. I suggest using Python as it is very convenient for many prupose, most
developpers have it installed anyway and it provides a lightweight builtin http server.

## Configure

In order to configure the project, make sure the Emscripten environment has been activated (`emsdk_env.bat`)
Then you can use `emcmake` to help configure the project using your current Emscripten SDK :
```
emcmake cmake --preset wasm .
```

# Build

Once configured, you can build the project using the following command line :
```
cmake --build build\wasm
```

# Run

The application is just a simple HTML page runing local scripts and webassemblies. But you browser will not allow to load those files directly from you local storage (due cross domain compatibility contraints). Thus, you will need to run a HTTP server to serve this simple HTTP page :
```
cd <Path_To_PointCloudLab>
python -m http.server
```

Then, open the url in your browser.
http://localhost:8000