# fudge - 3D force directed graph explorer

### **[Try the demo here!](https://justinleung.me/fudge)**


https://user-images.githubusercontent.com/28817028/188521745-d74fd8bc-e781-489f-b46d-f302734bb4fb.mov

Fudge is a 3D graph visualization tool that uses the force-directed technique of drawing graphs.  We can consider a graph as a physical system where every vertex of a graph is a particle exerting a repelling force on every other vertex, and any two vertices connected by an edge are pulled towards each other by a spring force. Over time, the system settles into a stable configuration which results in an aesthetically pleasing visualization of the graph.

## Build

Install dependencies (Emscripten, GLFW3, GLEW)

``` 
sudo apt-get install libglew-dev emscripten libglfw3 libglfw3-dev
```

To build the OpenGL version, run

```
make && ./fdg
```

To build for WebGL, run

```
make -f Makefile_emscripten
```

Emscripten will generate `index.js`, `index.wasm` under the `build` directory. The webpage can be opened at `build/index.html`.
