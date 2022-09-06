# fudge - 3D force directed graph explorer

### **[Try the WebGL app here!](https://justinleung.me/fudge)**

[demo_video.webm](https://user-images.githubusercontent.com/28817028/188521947-5a06ec75-d847-4001-9804-0318c9ab3ac6.webm)

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
