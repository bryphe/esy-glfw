open Reglm;

module Key = Glfw_key;

open Glfw_types;

/* GLFW */
external glfwInit: unit => bool = "caml_glfwInit";
external glfwCreateWindow: (int, int, string) => Window.t =
  "caml_glfwCreateWindow";
external glfwMakeContextCurrent: Window.t => unit =
  "caml_glfwMakeContextCurrent";
external glfwWindowShouldClose: Window.t => bool =
  "caml_glfwWindowShouldClose";
external glfwPollEvents: unit => unit = "caml_glfwPollEvents";
external glfwTerminate: unit => unit = "caml_glfwTerminate";
external glfwSwapBuffers: Window.t => unit = "caml_glfwSwapBuffers";
external glfwSetWindowSize: (Window.t, int, int) => unit =
  "caml_glfwSetWindowSize";
[@noalloc]
external glfwSetWindowPos: (Window.t, int, int) => unit =
  "caml_glfwSetWindowPos";
external glfwGetFramebufferSize: Window.t => Window.frameBufferSize =
  "caml_glfwGetFramebufferSize";
[@noalloc] external glfwShowWindow: Window.t => unit = "caml_glfwShowWindow";
[@noalloc] external glfwHideWindow: Window.t => unit = "caml_glfwHideWindow";
external glfwMaximizeWindow: Window.t => unit = "caml_glfwMaximizeWindow";
external glfwSetWindowTitle: (Window.t, string) => unit =
  "caml_glfwSetWindowTitle";
[@noalloc] external glfwSwapInterval: int => unit = "caml_glfwSwapInterval";

[@noalloc]
external glfwGetTime: unit => [@unboxed] float =
  "caml_glfwGetTime_byte" "caml_glfwGetTime";
[@noalloc]
external glfwSetTime: ([@unboxed] float) => unit =
  "caml_glfwSetTime_byte" "caml_glfwSetTime";

module Modifier = {
  type t = int;

  let _mod_shift = 0x0001;
  let _mod_control = 0x0002;
  let _mod_alt = 0x0004;
  let _mod_super = 0x0008;

  let of_int = (x: int) => x;

  let isShiftPressed = (m: t) => m land _mod_shift == _mod_shift;

  let isControlPressed = (m: t) => m land _mod_control == _mod_control;

  let isAltPressed = (m: t) => m land _mod_alt == _mod_alt;

  let isSuperPressed = (m: t) => m land _mod_super == _mod_super;
};

module MouseButton {
    type t =
    | GLFW_MOUSE_LEFT
    | GLFW_MOUSE_RIGHT
    | GLFW_MOUSE_MIDDLE
    | GLFW_MOUSE_BUTTON_4
    | GLFW_MOUSE_BUTTON_5
    | GLFW_MOUSE_BUTTON_6
    | GLFW_MOUSE_BUTTON_7
    | GLFW_MOUSE_BUTTON_8

    let show = (t) => switch(t) {
    | GLFW_MOUSE_LEFT => "Left"
    | GLFW_MOUSE_RIGHT => "Right"
    | GLFW_MOUSE_MIDDLE => "Middle"
    | GLFW_MOUSE_BUTTON_4 => "Button4"
    | GLFW_MOUSE_BUTTON_5 => "Button5"
    | GLFW_MOUSE_BUTTON_6 => "Button6"
    | GLFW_MOUSE_BUTTON_7 => "Button7"
    | GLFW_MOUSE_BUTTON_8 => "Button8"
    };
};

module Monitor = {
  type t;

  type position = {
    x: int,
    y: int,
  };
};

module VideoMode = {
  type t = {
    width: int,
    height: int,
  };
};

[@noalloc]
external glfwGetPrimaryMonitor: unit => Monitor.t =
  "caml_glfwGetPrimaryMonitor";
external glfwGetVideoMode: Monitor.t => VideoMode.t = "caml_glfwGetVideoMode";
external glfwGetMonitorPos: Monitor.t => Monitor.position =
  "caml_glfwGetMonitorPos";

type windowHint =
  | GLFW_RESIZABLE
  | GLFW_VISIBLE
  | GLFW_DECORATED
  | GLFW_FOCUSED
  | GLFW_AUTO_ICONIFY
  | GLFW_FLOATING
  | GLFW_MAXIMIZED;

module ButtonState = {
  type t =
    | GLFW_PRESS
    | GLFW_RELEASE
    | GLFW_REPEAT;

  let show = bs =>
    switch (bs) {
    | GLFW_PRESS => "press"
    | GLFW_RELEASE => "release"
    | GLFW_REPEAT => "repeat"
    };
};

[@noalloc]
external glfwDefaultWindowHints: unit => unit = "caml_glfwDefaultWindowHints";
[@noalloc]
external glfwWindowHint: (windowHint, bool) => unit = "caml_glfwWindowHint";

type glfwRenderLoopCallback = float => bool;

external glfwJavascriptRenderLoop: glfwRenderLoopCallback => unit =
  "caml_glfwJavascriptRenderLoop";

let glfwNativeRenderLoop = callback => {
  let shouldClose = ref(false);

  while (shouldClose^ == false) {
    shouldClose := callback(0.);
  };
};

let glfwRenderLoop = callback =>
  switch (Sys.backend_type) {
  | Native => glfwNativeRenderLoop(callback)
  | Bytecode => glfwNativeRenderLoop(callback)
  | _ => glfwJavascriptRenderLoop(callback)
  };

type glfwCharCallback = (Window.t, int) => unit;
external glfwSetCharCallback: (Window.t, glfwCharCallback) => unit =
  "caml_glfwSetCharCallback";

/* Internal implementation of glfwKeyCallback, since we need to cast some of
   integers to types */
type _glfwKeyCallback = (Window.t, int, int, ButtonState.t, int) => unit;
external _glfwSetKeyCallback: (Window.t, _glfwKeyCallback) => unit =
  "caml_glfwSetKeyCallback";

type glfwKeyCallback =
  (Window.t, Key.t, int, ButtonState.t, Modifier.t) => unit;

let glfwSetKeyCallback = (win, callback) =>
  _glfwSetKeyCallback(win, (w, k, scancode, buttonState, modifier) =>
    callback(
      w,
      Key.of_int(k),
      scancode,
      buttonState,
      Modifier.of_int(modifier),
    )
  );

type glfwCursorPosCallback = (Window.t, float, float) => unit;
external glfwSetCursorPosCallback: (Window.t, glfwCursorPosCallback) => unit = "caml_glfwSetCursorPosCallback";

/* Internal implementation of glfwMouseButtonCallback, since we need to cast some integers to types */
type _glfwMouseButtonCallback = (Window.t, MouseButton.t, ButtonState.t, int) => unit;
external _glfwSetMouseButtonCallback: (Window.t, _glfwMouseButtonCallback) => unit = "caml_glfwSetMouseButtonCallback";

type glfwMouseButtonCallback = (Window.t, MouseButton.t, ButtonState.t, Modifier.t) => unit;
let glfwSetMouseButtonCallback = (win, callback) => {
    _glfwSetMouseButtonCallback(win, (w, m, b, modifier) => {
        callback(w, m, b, Modifier.of_int(modifier));
    });
};

type glfwScrollCallback = (Window.t, float, float) => unit;
external glfwSetScrollCallback: (Window.t, glfwScrollCallback) => unit = "caml_glfwSetScrollCallback";

external glfwSetFramebufferSizeCallback:
  (Window.t, glfwFramebufferSizeCallback) => unit =
  "caml_glfwSetFramebufferSizeCallback";

external glfwSetWindowSizeCallback:
  (Window.t, glfwWindowSizeCallback) => unit =
  "caml_glfwSetWindowSizeCallback";

type glfwCursorPos = {
  mouseX: float,
  mouseY: float,
};
external caml_glfwGetCursorPos: Window.t => array(float) =
  "caml_glfwGetCursorPos";

let glfwGetCursorPos = w => {
  let pos = caml_glfwGetCursorPos(w);

  let v = {mouseX: pos[0], mouseY: pos[1]};
  v;
};

/* GL */
type shader;
type shaderType =
  | GL_VERTEX_SHADER
  | GL_FRAGMENT_SHADER;

type shaderCompilationResult =
  | CompilationSuccess
  | CompilationFailure(string);

external glClearColor: (float, float, float, float) => unit =
  "caml_glClearColor";
external glClearDepth: float => unit = "caml_glClearDepth";
external glCreateShader: shaderType => shader = "caml_glCreateShader";
external glShaderSource: (shader, string) => unit = "caml_glShaderSource";

external glViewport: (int, int, int, int) => unit = "caml_glViewport";

type enableOptions =
  | GL_DEPTH_TEST
  | GL_BLEND;

[@noalloc] external glEnable: enableOptions => unit = "caml_glEnable";
[@noalloc] external glDisable: enableOptions => unit = "caml_glDisable";

type depthFunctions =
  | GL_LEQUAL;

external glDepthFunc: depthFunctions => unit = "caml_glDepthFunc";

type blendFunc =
  | GL_ZERO
  | GL_ONE
  | GL_SRC_ALPHA
  | GL_ONE_MINUS_SRC_ALPHA;

[@noalloc]
external glBlendFunc: (blendFunc, blendFunc) => unit = "caml_glBlendFunc";

/* TODO: Add compile result return */
external glCompileShader: shader => shaderCompilationResult =
  "caml_glCompileShader";
external glDeleteShader: shader => unit = "caml_glDeleteShader";

type program;

external glCreateProgram: unit => program = "caml_glCreateProgram";
external glAttachShader: (program, shader) => unit = "caml_glAttachShader";
external glUseProgram: program => unit = "caml_glUseProgram";

type shaderLinkResult =
  | LinkSuccess
  | LinkFailure(string);

external glLinkProgram: program => shaderLinkResult = "caml_glLinkProgram";

type attribLocation;
external glGetAttribLocation: (program, string) => attribLocation =
  "caml_glGetAttribLocation";

type uniformLocation;
external glGetUniformLocation: (program, string) => uniformLocation =
  "caml_glGetUniformLocation";

external glUniform1f: (uniformLocation, float) => unit = "caml_glUniform1f";
external glUniform2f: (uniformLocation, float, float) => unit =
  "caml_glUniform2f";
external glUniform3f: (uniformLocation, float, float, float) => unit =
  "caml_glUniform3f";
external glUniform4f: (uniformLocation, float, float, float, float) => unit =
  "caml_glUniform4f";

external glUniform1i: (uniformLocation, int) => unit = "caml_glUniform1i";
external glUniform2i: (uniformLocation, int, int) => unit = "caml_glUniform2i";
external glUniform3i: (uniformLocation, int, int, int) => unit =
  "caml_glUniform3i";
external glUniform4i: (uniformLocation, int, int, int, int) => unit =
  "caml_glUniform4i";

external glUniform2fv: (uniformLocation, Vec2.t) => unit = "caml_glUniform2fv";
external glUniform3fv: (uniformLocation, Vec3.t) => unit = "caml_glUniform3fv";
external glUniform4fv: (uniformLocation, Vec4.t) => unit = "caml_glUniform4fv";

external glUniformMatrix4fv: (uniformLocation, Mat4.t) => unit =
  "caml_glUniformMatrix4fv";

type pixelAlignmentParameter =
  | GL_PACK_ALIGNMENT
  | GL_UNPACK_ALIGNMENT;

external glPixelStorei: (pixelAlignmentParameter, int) => unit =
  "caml_glPixelStorei";

type texture;
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
  | GL_UNSIGNED_SHORT;

external glCreateTexture: unit => texture = "caml_glCreateTexture";
external glBindTexture: (textureType, texture) => unit = "caml_glBindTexture";
external glTexParameteri:
  (textureType, textureParameter, textureParameterValue) => unit =
  "caml_glTexParameteri";
external glTexImage2D: (textureType, Image.t) => unit = "caml_glTexImage2D";
external glGenerateMipmap: textureType => unit = "caml_glGenerateMipmap";

type bufferType =
  | GL_ARRAY_BUFFER
  | GL_ELEMENT_ARRAY_BUFFER;

type buffer;
external glCreateBuffer: unit => buffer = "caml_glCreateBuffer";
external glBindBuffer: (bufferType, buffer) => unit = "caml_glBindBuffer";
external glUnbindBuffer: bufferType => unit = "caml_glUnbindBuffer";

type drawType =
  | GL_STATIC_DRAW;

external glBufferData:
  (bufferType, Bigarray.Array1.t('a, 'b, Bigarray.c_layout), drawType) => unit =
  "caml_glBufferData";

external glVertexAttribPointer: (attribLocation, int, glType, bool) => unit =
  "caml_glVertexAttribPointer";
external glEnableVertexAttribArray: attribLocation => unit =
  "caml_glEnableVertexAttribArray";

type drawMode =
  | GL_TRIANGLES
  | GL_TRIANGLE_STRIP;

external glDrawArrays: (drawMode, int, int) => unit = "caml_glDrawArrays";
external glDrawElements: (drawMode, int, glType, int) => unit =
  "caml_glDrawElements";

external printFrameBufferSize: Window.t => unit = "caml_printFrameBufferSize";
