# fudge - 3D force directed graph explorer

Fudge is a 

## Build

Install dependencies (Emscripten, GLFW3, GLEW)

``` 
sudo apt-get install libglew-dev emscripten libglfw3 libglfw3-dev
```

To build the OpenGL version, run
```make && ./fdg```

To build for WebGL, run
```make -f Makefile_emscripten```

Emscripten will generate `index.js`, `index.wasm` under the `build` directory. The webpage can be opened at `build/index.html`.