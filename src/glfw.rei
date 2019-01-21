open Reglm;

module Key = Glfw_key;

include (module type of Glfw_types);

let glfwInit: unit => bool;
let glfwCreateWindow:
  (int, int, ~sharedContext: Window.t=?, string) => Window.t;
let glfwMakeContextCurrent: Window.t => unit;
let glfwWindowShouldClose: Window.t => bool;
let glfwPollEvents: unit => unit;
let glfwTerminate: unit => unit;
let glfwSwapBuffers: Window.t => unit;
let glfwSetWindowPos: (Window.t, int, int) => unit;
let glfwSetWindowSize: (Window.t, int, int) => unit;
let glfwGetFramebufferSize: Window.t => Window.frameBufferSize;
let glfwGetWindowSize: Window.t => Window.windowSize;
let glfwMaximizeWindow: Window.t => unit;
let glfwSetWindowTitle: (Window.t, string) => unit;
let glfwShowWindow: Window.t => unit;
let glfwHideWindow: Window.t => unit;
let glfwDestroyWindow: Window.t => unit;
let glfwSwapInterval: int => unit;
let glfwGetTime: unit => float;
let glfwSetTime: float => unit;

module Modifier: {
  type t;

  let of_int: int => t;

  let isShiftPressed: t => bool;
  let isControlPressed: t => bool;
  let isAltPressed: t => bool;
  let isSuperPressed: t => bool;
};

module MouseButton: {
  type t =
    | GLFW_MOUSE_LEFT
    | GLFW_MOUSE_RIGHT
    | GLFW_MOUSE_MIDDLE
    | GLFW_MOUSE_BUTTON_4
    | GLFW_MOUSE_BUTTON_5
    | GLFW_MOUSE_BUTTON_6
    | GLFW_MOUSE_BUTTON_7
    | GLFW_MOUSE_BUTTON_8;

  let show: t => string;
};

module Monitor: {
  type t;

  type position = {
    x: int,
    y: int,
  };
};

module VideoMode: {
  type t = {
    width: int,
    height: int,
  };
};

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

module ButtonState: {
  type t =
    | GLFW_PRESS
    | GLFW_RELEASE
    | GLFW_REPEAT;

  let show: t => string;
};

let glfwDefaultWindowHints: unit => unit;
let glfwWindowHint: (windowHint, bool) => unit;

type glfwCursorPosCallback = (Window.t, float, float) => unit;
let glfwSetCursorPosCallback: (Window.t, glfwCursorPosCallback) => unit;

type glfwCharCallback = (Window.t, int) => unit;
let glfwSetCharCallback: (Window.t, glfwCharCallback) => unit;

type glfwKeyCallback =
  (Window.t, Key.t, int, ButtonState.t, Modifier.t) => unit;
let glfwSetKeyCallback: (Window.t, glfwKeyCallback) => unit;

type glfwScrollCallback = (Window.t, float, float) => unit;
let glfwSetScrollCallback: (Window.t, glfwScrollCallback) => unit;

type glfwMouseButtonCallback =
  (Window.t, MouseButton.t, ButtonState.t, Modifier.t) => unit;
let glfwSetMouseButtonCallback: (Window.t, glfwMouseButtonCallback) => unit;

let glfwSetFramebufferSizeCallback:
  (Window.t, glfwFramebufferSizeCallback) => unit;

let glfwSetWindowSizeCallback: (Window.t, glfwWindowSizeCallback) => unit;

type glfwCursorPos = {
  mouseX: float,
  mouseY: float,
};
let glfwGetCursorPos: Window.t => glfwCursorPos;

type glfwCursor;
type glfwCursorShape =
  | GLFW_ARROW_CURSOR
  | GLFW_IBEAM_CURSOR
  | GLFW_CROSSHAIR_CURSOR
  | GLFW_HAND_CURSOR
  | GLFW_HRESIZE_CURSOR
  | GLFW_VRESIZE_CURSOR;
let glfwCreateStandardCursor: glfwCursorShape => glfwCursor;
let glfwDestroyCursor: glfwCursor => unit;
let glfwSetCursor: (Window.t, glfwCursor) => unit;

let printFrameBufferSize: Window.t => unit;

type glfwRenderLoopCallback = float => bool;

let glfwRenderLoop: glfwRenderLoopCallback => unit;

/* GL */

let glClearColor: (float, float, float, float) => unit;
let glClearDepth: float => unit;
let glViewport: (int, int, int, int) => unit;
let glScissor: (int, int, int, int) => unit;

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
  | GL_LINEAR
  | GL_CLAMP_TO_EDGE;

type texturePixelDataFormat =
  | GL_RGB
  | GL_RGBA;

type glType =
  | GL_FLOAT
  | GL_UNSIGNED_BYTE
  | GL_UNSIGNED_SHORT
  | GL_UNSIGNED_SHORT_5_6_5
  | GL_UNSIGNED_SHORT_4_4_4_4
  | GL_UNSIGNED_SHORT_5_5_5_1;

type texture;
let glCreateTexture: unit => texture;
let glBindTexture: (textureType, texture) => unit;
let glTexParameteri:
  (textureType, textureParameter, textureParameterValue) => unit;
let glTexImage2D: (textureType, Image.t) => unit;
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
  (bufferType, Bigarray.Array1.t('a, 'b, Bigarray.c_layout), drawType) => unit;

type drawMode =
  | GL_TRIANGLES
  | GL_TRIANGLE_STRIP;

let glDrawArrays: (drawMode, int, int) => unit;
let glDrawElements: (drawMode, int, glType, int) => unit;

let glReadPixels:
  (int, int, int, int, texturePixelDataFormat, glType, 'pixelBuffer) => unit;