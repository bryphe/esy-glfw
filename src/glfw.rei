open Reglm;

type window;

let glfwInit: unit => bool;
let glfwCreateWindow: (int, int, string) => window;
let glfwMakeContextCurrent: window => unit;
let glfwWindowShouldClose: window => bool;
let glfwPollEvents: unit => unit;
let glfwTerminate: unit => unit;
let glfwSwapBuffers: window => unit;
let glfwSetWindowPos: (window, int, int) => unit;
let glfwSetWindowSize: (window, int, int) => unit;
let glfwMaximizeWindow: (window) => unit;
let glfwSetWindowTitle: (window, string) => unit;
let glfwShowWindow: (window) => unit;
let glfwHideWindow: (window) => unit;
let glfwSwapInterval: int => unit;
let glfwGetTime: unit => float;
let glfwSetTime: float => unit;

module Monitor {
    type t;

    type position = {
        x: int,
        y: int,
    };
}

module VideoMode {
    type t = {
        width: int,
        height: int,
    };
}

let glfwGetPrimaryMonitor: unit => Monitor.t;
let glfwGetVideoMode: Monitor.t => VideoMode.t;
let glfwGetMonitorPos: Monitor.t => Monitor.position;

type windowHint =
| GLFW_RESIZABLE
| GLFW_VISIBLE
| GLFW_DECORATED
| GLFW_FOCUSED
| GLFW_AUTO_ICONIFY
| GLFW_FLOATING
| GLFW_MAXIMIZED;

type glfwMouseButton =
| GLFW_MOUSE_BUTTON_LEFT
| GLFW_MOUSE_BUTTON_MIDDLE
| GLFW_MOUSE_BUTTON_RIGHT
| GLFW_MOUSE_BUTTON_LAST

type glfwButtonState =
| GLFW_PRESS
| GLFW_RELEASE

type glfwModifierKey =
| GLFW_MOD_SHIFT
| GLFW_MOD_CONTROL
| GLFW_MOD_ALT
| GLFW_MOD_SUPER

let glfwDefaultWindowHints: unit => unit;
let glfwWindowHint: (windowHint, bool) => unit;

type glfwCharCallback = (window, int) => unit;
let glfwSetCharCallback: (window, glfwCharCallback) => unit;

type glfwFramebufferSizeCallback = (window, int, int) => unit;
let glfwSetFramebufferSizeCallback:
  (window, glfwFramebufferSizeCallback) => unit;

type glfwCursorPos = {
    mouseX: float,
    mouseY: float
};
let glfwGetCursorPos: window => glfwCursorPos;

let printFrameBufferSize: window => unit;

type glfwRenderLoopCallback = (float) => bool;

let glfwRenderLoop: (glfwRenderLoopCallback) => unit;

/* GL */

let glClearColor: (float, float, float, float) => unit;
let glClearDepth: float => unit;
let glViewport: (int, int, int, int) => unit;

type shader;
type shaderType =
  | GL_VERTEX_SHADER
  | GL_FRAGMENT_SHADER;

type shaderCompilationResult =
  | CompilationSuccess
  | CompilationFailure(string);

let glCreateShader: shaderType => shader;
let glShaderSource: (shader, string) => unit;
let glCompileShader: shader => shaderCompilationResult;
let glDeleteShader: shader => unit;

type enableOptions =
  | GL_DEPTH_TEST
  | GL_BLEND;

let glEnable: enableOptions => unit;
let glDisable: enableOptions => unit;

type depthFunctions =
  | GL_LEQUAL;

let glDepthFunc: depthFunctions => unit;

type blendFunc =
  | GL_ZERO
  | GL_ONE
  | GL_SRC_ALPHA
  | GL_ONE_MINUS_SRC_ALPHA;

let glBlendFunc: (blendFunc, blendFunc) => unit;

type program;

type shaderLinkResult =
| LinkSuccess
| LinkFailure(string);

let glCreateProgram: unit => program;
let glAttachShader: (program, shader) => unit;
let glLinkProgram: program => shaderLinkResult;
let glUseProgram: program => unit;

type attribLocation;
let glGetAttribLocation: (program, string) => attribLocation;

type uniformLocation;
let glGetUniformLocation: (program, string) => uniformLocation;

let glUniform1f: (uniformLocation, float) => unit;
let glUniform2f: (uniformLocation, float, float) => unit;
let glUniform3f: (uniformLocation, float, float, float) => unit;
let glUniform4f: (uniformLocation, float, float, float, float) => unit;

let glUniform1i: (uniformLocation, int) => unit;
let glUniform2i: (uniformLocation, int, int) => unit;
let glUniform3i: (uniformLocation, int, int, int) => unit;
let glUniform4i: (uniformLocation, int, int, int, int) => unit;

let glUniform2fv: (uniformLocation, Vec2.t) => unit;
let glUniform3fv: (uniformLocation, Vec3.t) => unit;
let glUniform4fv: (uniformLocation, Vec4.t) => unit;

let glUniformMatrix4fv: (uniformLocation, Mat4.t) => unit;

type pixelAlignmentParameter =
  | GL_PACK_ALIGNMENT
  | GL_UNPACK_ALIGNMENT;

let glPixelStorei: (pixelAlignmentParameter, int) => unit;

type textureType =
  | GL_TEXTURE_2D;

type textureParameter =
  | GL_TEXTURE_WRAP_S
  | GL_TEXTURE_WRAP_T
  | GL_TEXTURE_MIN_FILTER
  | GL_TEXTURE_MAG_FILTER;

type textureParameterValue =
  | GL_REPEAT
  | GL_LINEAR;

type texturePixelDataFormat =
  | GL_RGB
  | GL_RGBA;

type glType =
  | GL_FLOAT
  | GL_UNSIGNED_BYTE
  | GL_UNSIGNED_SHORT;

type texture;
let glCreateTexture: unit => texture;
let glBindTexture: (textureType, texture) => unit;
let glTexParameteri:
  (textureType, textureParameter, textureParameterValue) => unit;
let glTexImage2D:
  (textureType, Image.t) => unit;
let glGenerateMipmap: textureType => unit;

type bufferType =
  | GL_ARRAY_BUFFER
  | GL_ELEMENT_ARRAY_BUFFER;

let glVertexAttribPointer: (attribLocation, int, glType, bool) => unit;
let glEnableVertexAttribArray: attribLocation => unit;

type buffer;
let glCreateBuffer: unit => buffer;
let glBindBuffer: (bufferType, buffer) => unit;
let glUnbindBuffer: bufferType => unit;

type drawType =
  | GL_STATIC_DRAW;

let glBufferData:
  (
    bufferType,
    Bigarray.Array1.t('a, 'b, Bigarray.c_layout),
    drawType
  ) =>
  unit;

type drawMode =
  | GL_TRIANGLES
  | GL_TRIANGLE_STRIP;

let glDrawArrays: (drawMode, int, int) => unit;
let glDrawElements: (drawMode, int, glType, int) => unit;
