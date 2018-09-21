open Reglm;

type window;

let glfwInit: unit => bool;
let glfwCreateWindow: (int, int, string) => window;
let glfwMakeContextCurrent: window => unit;
let glfwWindowShouldClose: window => bool;
let glfwPollEvents: unit => unit;
let glfwTerminate: unit => unit;
let glfwSwapBuffers: window => unit;
let printFrameBufferSize: window => unit;

/* GL */

let glClearColor: (float, float, float, float) => unit;
let glClearDepth: float => unit;

type shader;
type shaderType =
  | GL_VERTEX_SHADER
  | GL_FRAGMENT_SHADER;

let glCreateShader: shaderType => shader;
let glShaderSource: (shader, string) => unit;
let glCompileShader: shader => unit;
let glDeleteShader: shader => unit;
/*TODO: Remove*/
let glGetShaderIsCompiled: shader => bool;

type enableOptions =
  | GL_DEPTH_TEST;

let glEnable: enableOptions => unit;

type depthFunctions =
  | GL_LEQUAL;

let glDepthFunc: depthFunctions => unit;

type program;

let glCreateProgram: unit => program;
let glAttachShader: (program, shader) => unit;
let glLinkProgram: program => unit;
let glUseProgram: program => unit;

type attribLocation;
let glGetAttribLocation: (program, string) => attribLocation;

type uniformLocation;
let glGetUniformLocation: (program, string) => uniformLocation;
let glUniformMatrix4fv: (uniformLocation, Mat4.t) => unit;

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

type texturePixelDataFormat =
  | GL_RGB
  | GL_RGBA

type texturePixelDataType =
  | GL_UNSIGNED_BYTE

type texture;
let glCreateTexture: unit => texture;
let glBindTexture: (textureType, texture) => unit;
let glTexParameteri: (textureType, textureParameter, textureParameterValue) => unit;
let glTexImage2D: (textureType, texturePixelDataFormat, texturePixelDataType, Image.t) => unit;
let glGenerateMipmap: (textureType) => unit;

type bufferType =
  | GL_ARRAY_BUFFER;

type glType =
  | GL_FLOAT;

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
    Bigarray.Array1.t(float, Bigarray.float32_elt, Bigarray.c_layout),
    drawType
  ) =>
  unit;

type drawMode =
  | GL_TRIANGLES
  | GL_TRIANGLE_STRIP;

let glDrawArrays: (drawMode, int, int) => unit;
