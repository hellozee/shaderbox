# ShaderBox

ShaderBox is a tiny local shader playground for experimenting with fragment
shaders in a Shadertoy-like environment. It opens a resizable OpenGL window,
updates common Shadertoy uniforms every frame, and reloads the fragment shader
when it changes on disk.

## Build

The only runtime dependency is SDL2; OpenGL is provided by the platform.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/shaderbox
```

You can load another fragment shader by passing its path:

```sh
./build/shaderbox /path/to/my-shader.glsl
```

Edit the shader while the app is running to reload it. Press Escape or close
the window to exit.

## Uniforms

Fragment shaders can use `iResolution` (`vec3`), `iTime`, `iTimeDelta`,
`iFrame`, and `iMouse` (`vec4`, in bottom-left-origin pixel coordinates).
The default shader is in `src/res/fragment.glsl`.
