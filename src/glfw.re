open Reglm;

type window;

/* GLFW */
external glfwInit: unit => bool = "caml_glfwInit";
external glfwCreateWindow: (int, int, string) => window =
  "caml_glfwCreateWindow";
external glfwMakeContextCurrent: window => unit =
  "caml_glfwMakeContextCurrent";
external glfwWindowShouldClose: window => bool = "caml_glfwWindowShouldClose";
external glfwPollEvents: unit => unit = "caml_glfwPollEvents";
external glfwTerminate: unit => unit = "caml_glfwTerminate";
external glfwSwapBuffers: window => unit = "caml_glfwSwapBuffers";
external glfwSetWindowSize: (window, int, int) => unit =
  "caml_glfwSetWindowSize";
[@noalloc] external glfwSetWindowPos: (window, int, int) => unit =
  "caml_glfwSetWindowPos";
[@noalloc] external glfwShowWindow: (window) => unit = "caml_glfwShowWindow";
[@noalloc] external glfwHideWindow: (window) => unit = "caml_glfwHideWindow";
external glfwMaximizeWindow: (window) => unit = "caml_glfwMaximizeWindow";
external glfwSetWindowTitle: (window, string) => unit = "caml_glfwSetWindowTitle";
[@noalloc] external glfwSwapInterval: int => unit = "caml_glfwSwapInterval";

[@noalloc] external glfwGetTime: unit => ([@unboxed] float) = "caml_glfwGetTime_byte" "caml_glfwGetTime";
[@noalloc] external glfwSetTime: ([@unboxed] float) => unit = "caml_glfwSetTime_byte" "caml_glfwSetTime";

type windowHint =
| GLFW_RESIZABLE
| GLFW_VISIBLE
| GLFW_DECORATED
| GLFW_FOCUSED
| GLFW_AUTO_ICONIFY
| GLFW_FLOATING
| GLFW_MAXIMIZED;

[@noalloc] external glfwDefaultWindowHints: unit => unit = "caml_glfwDefaultWindowHints";
[@noalloc] external glfwWindowHint: (windowHint, bool) => unit = "caml_glfwWindowHint";

type glfwRenderLoopCallback = float => bool;

external glfwJavascriptRenderLoop: (glfwRenderLoopCallback) => unit = "caml_glfwJavascriptRenderLoop";

let glfwNativeRenderLoop = (callback) => {
    let shouldClose = ref(false);

    while(shouldClose^ == false) {
        shouldClose := callback(0.);
    }
};

let glfwRenderLoop = (callback) => {
    switch (Sys.backend_type) {
    | Native => glfwNativeRenderLoop(callback)
    | Bytecode => glfwNativeRenderLoop(callback);
    | _ => glfwJavascriptRenderLoop(callback);
    };
}

type glfwFramebufferSizeCallback = (window, int, int) => unit;
external glfwSetFramebufferSizeCallback:
  (window, glfwFramebufferSizeCallback) => unit =
  "caml_glfwSetFramebufferSizeCallback";

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

[@noalloc] external glBlendFunc: (blendFunc, blendFunc) => unit = "caml_glBlendFunc";

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
external glUniform2f: (uniformLocation, float, float) => unit = "caml_glUniform2f";
external glUniform3f: (uniformLocation, float, float, float) => unit = "caml_glUniform3f";
external glUniform4f: (uniformLocation, float, float, float, float) => unit = "caml_glUniform4f";

external glUniform1i: (uniformLocation, int) => unit = "caml_glUniform1i";
external glUniform2i: (uniformLocation, int, int) => unit = "caml_glUniform2i";
external glUniform3i: (uniformLocation, int, int, int) => unit = "caml_glUniform3i";
external glUniform4i: (uniformLocation, int, int, int, int) => unit = "caml_glUniform4i";

external glUniform2fv: (uniformLocation, Vec2.t) => unit = "caml_glUniform2fv";
external glUniform3fv: (uniformLocation, Vec3.t) => unit = "caml_glUniform3fv";
external glUniform4fv: (uniformLocation, Vec4.t) => unit = "caml_glUniform4fv";

external glUniformMatrix4fv: (uniformLocation, Mat4.t) => unit =
  "caml_glUniformMatrix4fv";

type pixelAlignmentParameter =
  | GL_PACK_ALIGNMENT
  | GL_UNPACK_ALIGNMENT;

external glPixelStorei:  (pixelAlignmentParameter, int) => unit = "caml_glPixelStorei";

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
  | GL_LINEAR;

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
external glTexImage2D:
  (textureType, Image.t) => unit =
  "caml_glTexImage2D";
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
  (
    bufferType,
    Bigarray.Array1.t('a, 'b, Bigarray.c_layout),
    drawType
  ) =>
  unit =
  "caml_glBufferData";

external glVertexAttribPointer: (attribLocation, int, glType, bool) => unit =
  "caml_glVertexAttribPointer";
external glEnableVertexAttribArray: attribLocation => unit =
  "caml_glEnableVertexAttribArray";

type drawMode =
  | GL_TRIANGLES
  | GL_TRIANGLE_STRIP;

external glDrawArrays: (drawMode, int, int) => unit = "caml_glDrawArrays";
external glDrawElements: (drawMode, int, glType, int) => unit = "caml_glDrawElements";

external printFrameBufferSize: window => unit = "caml_printFrameBufferSize";
