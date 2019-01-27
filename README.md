[![Build Status](https://bryphe.visualstudio.com/reason-glfw/_apis/build/status/bryphe.reason-glfw)](https://bryphe.visualstudio.com/reason-glfw/_build/latest?definitionId=9)
[![Build Status](https://travis-ci.org/bryphe/reason-glfw.svg?branch=master)](https://travis-ci.org/bryphe/reason-glfw) 
[![npm version](https://badge.fury.io/js/reason-glfw.svg)](https://badge.fury.io/js/reason-glfw)

# reason-glfw

#### Cross-platform Reason / OCaml bindings for GLFW - supports Native (OpenGL) and Javascript (WebGL))

## Building

`esy` is used for building, so if you don't have `esy`, install it:

```
npm install -g esy@0.2.11
```

- `esy install`
- `esy build`

> __NOTE:__ If you've never built with `esy` before, the first compile could take some time - it builds the `ocaml` compiler as well as `cmake`. Incremental builds, though, are extremely fast!

## Running

A very simple example project is provided - you can test it out here:

__NATIVE:__
- `cd _build/install/default/bin`
- `./test_glfw` (`./test_glfw.exe` on Windows)

__WEB:__
- Start an http-server at `_build/install/default/bin`. (I use `npm install -g http-server`).
- Navigate to `index.html`

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for more details.

## Acknowledgements

- The test texture is provided by [LearnOpenGL](https://learnopengl.com) - an excellent resource for OpenGL!
- This is built on several great libraries, like [GLFW](https://www.glfw.org), [glad](https://github.com/Dav1dde/glad), [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- The test image is from @Arahnoid's [UVChecker-map](https://github.com/Arahnoid/UVChecker-map) repo.
