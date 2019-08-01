open Reglm;

module Key = Glfw_key;

include Glfw_types;

/* GLFW */
external glfwInit: unit => bool = "caml_glfwInit";
external glfwCreateWindow:
  (int, int, ~sharedContext: Window.t=?, string) => Window.t =
  "caml_glfwCreateWindow";

let _lastWindow: ref(option(Window.t)) = ref(None);

[@noalloc]
external _glfwMakeContextCurrent: Window.t => unit =
  "caml_glfwMakeContextCurrent";

let glfwMakeContextCurrent = win => {
  switch (_lastWindow^) {
  | None =>
    _glfwMakeContextCurrent(win);
    _lastWindow := Some(win);
  | Some(w) when w != win =>
    ignore(w);
    _glfwMakeContextCurrent(win);
    _lastWindow := Some(win);
  | Some(_) => ()
  };
};

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
external glfwGetWindowSize: Window.t => Window.windowSize =
  "caml_glfwGetWindowSize";
external glfwGetFramebufferSize: Window.t => Window.frameBufferSize =
  "caml_glfwGetFramebufferSize";
[@noalloc] external glfwShowWindow: Window.t => unit = "caml_glfwShowWindow";
[@noalloc] external glfwHideWindow: Window.t => unit = "caml_glfwHideWindow";
external glfwMaximizeWindow: Window.t => unit = "caml_glfwMaximizeWindow";
external glfwSetWindowTitle: (Window.t, string) => unit =
  "caml_glfwSetWindowTitle";
[@noalloc]
external glfwDestroyWindow: Window.t => unit = "caml_glfwDestroyWindow";
[@noalloc] external glfwSwapInterval: int => unit = "caml_glfwSwapInterval";

[@noalloc]
external glfwGetTime: unit => [@unboxed] float =
  "caml_glfwGetTime_byte" "caml_glfwGetTime";
[@noalloc]
external glfwSetTime: ([@unboxed] float) => unit =
  "caml_glfwSetTime_byte" "caml_glfwSetTime";

[@noalloc]
external glfwSetWindowIcon: (Window.t, string) => unit =
  "caml_glfwSetWindowIcon";

external glfwGetClipboardString: (Window.t) => option(string) = "caml_glfwGetClipboardString";
external glfwSetClipboardString: (Window.t, string) => unit = "caml_glfwSetClipboardString";

[@noalloc]
external glfwGetNativeWindow: Window.t => NativeWindow.t =
  "caml_glfwGetNativeWindow";

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

  let show = v => {
    "shift: "
    ++ string_of_bool(isShiftPressed(v))
    ++ " alt: "
    ++ string_of_bool(isAltPressed(v))
    ++ " ctrl: "
    ++ string_of_bool(isControlPressed(v))
    ++ " super: "
    ++ string_of_bool(isSuperPressed(v));
  };
};

module MouseButton = {
  type t =
    | GLFW_MOUSE_LEFT
    | GLFW_MOUSE_RIGHT
    | GLFW_MOUSE_MIDDLE
    | GLFW_MOUSE_BUTTON_4
    | GLFW_MOUSE_BUTTON_5
    | GLFW_MOUSE_BUTTON_6
    | GLFW_MOUSE_BUTTON_7
    | GLFW_MOUSE_BUTTON_8;

  let show = t =>
    switch (t) {
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

  type dimensions = {
    width: int,
    height: int,
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
external glfwGetMonitorPhysicalSize: Monitor.t => Monitor.dimensions =
  "caml_glfwGetMonitorPhysicalSize";

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

type _glfwCharModsCallback = (Window.t, int, int) => unit;
external _glfwSetCharModsCallback: (Window.t, _glfwCharModsCallback) => unit =
  "caml_glfwSetCharModsCallback";

type glfwCharModsCallback = (Window.t, int, Modifier.t) => unit;

let glfwSetCharModsCallback = (win, callback) =>
  _glfwSetCharModsCallback(win, (w, k, modifier) =>
    callback(w, k, Modifier.of_int(modifier))
  );

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
external glfwSetCursorPosCallback: (Window.t, glfwCursorPosCallback) => unit =
  "caml_glfwSetCursorPosCallback";

/* Internal implementation of glfwMouseButtonCallback, since we need to cast some integers to types */
type _glfwMouseButtonCallback =
  (Window.t, MouseButton.t, ButtonState.t, int) => unit;
external _glfwSetMouseButtonCallback:
  (Window.t, _glfwMouseButtonCallback) => unit =
  "caml_glfwSetMouseButtonCallback";

type glfwMouseButtonCallback =
  (Window.t, MouseButton.t, ButtonState.t, Modifier.t) => unit;
let glfwSetMouseButtonCallback = (win, callback) =>
  _glfwSetMouseButtonCallback(win, (w, m, b, modifier) =>
    callback(w, m, b, Modifier.of_int(modifier))
  );

type glfwScrollCallback = (Window.t, float, float) => unit;
external glfwSetScrollCallback: (Window.t, glfwScrollCallback) => unit =
  "caml_glfwSetScrollCallback";

external glfwSetFramebufferSizeCallback:
  (Window.t, glfwFramebufferSizeCallback) => unit =
  "caml_glfwSetFramebufferSizeCallback";

external glfwSetWindowSizeCallback: (Window.t, glfwWindowSizeCallback) => unit =
  "caml_glfwSetWindowSizeCallback";

external glfwSetWindowPosCallback: (Window.t, glfwWindowPosCallback) => unit =
  "caml_glfwSetWindowPosCallback";

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

type glfwCursor;
type glfwCursorShape =
  | GLFW_ARROW_CURSOR
  | GLFW_IBEAM_CURSOR
  | GLFW_CROSSHAIR_CURSOR
  | GLFW_HAND_CURSOR
  | GLFW_HRESIZE_CURSOR
  | GLFW_VRESIZE_CURSOR;

external glfwCreateStandardCursor: glfwCursorShape => glfwCursor =
  "caml_glfwCreateStandardCursor";
[@noalloc]
external glfwDestroyCursor: glfwCursor => unit = "caml_glfwDestroyCursor";
[@noalloc]
external glfwSetCursor: (Window.t, glfwCursor) => unit = "caml_glfwSetCursor";

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

[@noalloc]
external glViewport: (int, int, int, int) => unit = "caml_glViewport";
[@noalloc] external glScissor: (int, int, int, int) => unit = "caml_glScissor";

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
[@noalloc]
external glGetAttribLocation: (program, string) => attribLocation =
  "caml_glGetAttribLocation";

type uniformLocation;
[@noalloc]
external glGetUniformLocation: (program, string) => uniformLocation =
  "caml_glGetUniformLocation";

[@noalloc]
external glUniform1f: (uniformLocation, [@unboxed] float) => unit = "caml_glUniform1f_byte" "caml_glUniform1f";
[@noalloc]
external glUniform2f: (uniformLocation, [@unboxed] float, [@unboxed] float) => unit =
  "caml_glUniform2f_byte" "caml_glUniform2f";
[@noalloc]
external glUniform3f: (uniformLocation, [@unboxed] float, [@unboxed] float, [@unboxed] float) => unit =
  "caml_glUniform3f_byte" "caml_glUniform3f";
[@noalloc]
external glUniform4f: (uniformLocation, [@unboxed] float, [@unboxed] float, [@unboxed] float, [@unboxed] float) => unit =
  "caml_glUniform4f_byte" "caml_glUniform4f";

[@noalloc]
external glUniform1i: (uniformLocation, int) => unit = "caml_glUniform1i";
[@noalloc]
external glUniform2i: (uniformLocation, int, int) => unit = "caml_glUniform2i";
[@noalloc]
external glUniform3i: (uniformLocation, int, int, int) => unit =
  "caml_glUniform3i";
[@noalloc]
external glUniform4i: (uniformLocation, int, int, int, int) => unit =
  "caml_glUniform4i";

[@noalloc]
external glUniform2fv: (uniformLocation, Vec2.t) => unit = "caml_glUniform2fv";
[@noalloc]
external glUniform3fv: (uniformLocation, Vec3.t) => unit = "caml_glUniform3fv";
[@noalloc]
external glUniform4fv: (uniformLocation, Vec4.t) => unit = "caml_glUniform4fv";

[@noalloc]
external glUniformMatrix4fv: (uniformLocation, Mat4.t) => unit =
  "caml_glUniformMatrix4fv";

type pixelAlignmentParameter =
  | GL_PACK_ALIGNMENT
  | GL_UNPACK_ALIGNMENT;

[@noalloc]
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

type format =
  | GL_ALPHA
  | GL_RGB
  | GL_RGBA;

type glType =
  | GL_FLOAT
  | GL_UNSIGNED_BYTE
  | GL_UNSIGNED_SHORT
  | GL_UNSIGNED_SHORT_5_6_5
  | GL_UNSIGNED_SHORT_4_4_4_4
  | GL_UNSIGNED_SHORT_5_5_5_1;

[@noallooc] external glCreateTexture: unit => texture = "caml_glCreateTexture";
[@noalloc]
external glBindTexture: (textureType, texture) => unit = "caml_glBindTexture";
[@noalloc]
external glTexParameteri:
  (textureType, textureParameter, textureParameterValue) => unit =
  "caml_glTexParameteri";

external glTexImage2D:
  (
    textureType,
    int,
    format,
    format,
    glType,
    Bigarray.Array2.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout)
  ) =>
  unit =
  "caml_glTexImage2D_bytecode" "caml_glTexImage2D_native";

[@noalloc]
external glGenerateMipmap: textureType => unit = "caml_glGenerateMipmap";

type bufferType =
  | GL_ARRAY_BUFFER
  | GL_ELEMENT_ARRAY_BUFFER;

type buffer;
[@noalloc] external glCreateBuffer: unit => buffer = "caml_glCreateBuffer";
[@noalloc]
external glBindBuffer: (bufferType, buffer) => unit = "caml_glBindBuffer";
[@noalloc] external glUnbindBuffer: bufferType => unit = "caml_glUnbindBuffer";

type drawType =
  | GL_STATIC_DRAW;

[@noalloc]
external glBufferData:
  (bufferType, Bigarray.Array1.t('a, 'b, Bigarray.c_layout), drawType) => unit =
  "caml_glBufferData";

[@noalloc]
external glVertexAttribPointer: (attribLocation, int, glType, bool) => unit =
  "caml_glVertexAttribPointer";
[@noalloc]
external glEnableVertexAttribArray: attribLocation => unit =
  "caml_glEnableVertexAttribArray";

type drawMode =
  | GL_TRIANGLES
  | GL_TRIANGLE_STRIP;

[@noalloc]
external glDrawArrays: (drawMode, int, int) => unit = "caml_glDrawArrays";
[@noalloc]
external glDrawElements: (drawMode, int, glType, int) => unit =
  "caml_glDrawElements";

external printFrameBufferSize: Window.t => unit = "caml_printFrameBufferSize";

external glReadPixels:
  (
    int,
    int,
    format,
    glType,
    Bigarray.Array2.t(int, Bigarray.int8_unsigned_elt, Bigarray.c_layout)
  ) =>
  unit =
  "caml_glReadPixels";
