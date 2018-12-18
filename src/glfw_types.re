module Window = {
  type t;

  type windowSize = {
    width: int,
    height: int,
  };

  type frameBufferSize = {
    width: int,
    height: int,
  };
};

type enableOptions =
  | GL_DEPTH_TEST
  | GL_BLEND
  | GL_SCISSOR_TEST;

type glfwFramebufferSizeCallback = (Window.t, int, int) => unit;
type glfwWindowSizeCallback = (Window.t, int, int) => unit;
