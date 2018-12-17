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


type glfwFramebufferSizeCallback = (Window.t, int, int) => unit;
type glfwWindowSizeCallback = (Window.t, int, int) => unit;
