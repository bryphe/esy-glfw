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
external glfwMaximizeWindow: (window) => unit = "caml_glfwMaximizeWindow";
external glfwSetWindowTitle: (window, string) => unit = "caml_glfwSetWindowTitle";

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
  | GL_DEPTH_TEST;

external glEnable: enableOptions => unit = "caml_glEnable";

type depthFunctions =
  | GL_LEQUAL;

external glDepthFunc: depthFunctions => unit = "caml_glDepthFunc";

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
external glUniformMatrix4fv: (uniformLocation, Mat4.t) => /* todo */ unit =
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
  (textureType, glType, Image.t) => unit =
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
