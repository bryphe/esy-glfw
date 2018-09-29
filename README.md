[![Build status](https://ci.appveyor.com/api/projects/status/4gcoritaowypu9va/branch/master?svg=true)](https://ci.appveyor.com/project/bryphe/reason-glfw/branch/master)
[![Build Status](https://travis-ci.org/bryphe/reason-glfw.svg?branch=master)](https://travis-ci.org/bryphe/reason-glfw) 

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

- The test texture is provided by [LearnOpenGL](https://www.learnopengl) - an excellent resource for OpenGL!
- This is built on several great libraries, like [GLFW](https://www.glfw.org), [glad](https://github.com/Dav1dde/glad), [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
