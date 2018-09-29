open Reglfw;
open Reglfw.Glfw;
open Reglm;

let loadShader = (shaderType, source) => {
  let shader = glCreateShader(shaderType);
  let () = glShaderSource(shader, source);
  let result = glCompileShader(shader);
  switch (result) {
  | CompilationSuccess => print_endline("Shader compiled successfully.")
  | CompilationFailure(v) => print_endline("Failed to compile shader: " ++ v)
  };
  shader;
};

let initShaderProgram = (vsSource, fsSource) => {
  let vsShader = loadShader(GL_VERTEX_SHADER, vsSource);
  let fsShader = loadShader(GL_FRAGMENT_SHADER, fsSource);
  let shaderProgram = glCreateProgram();
  let () = glAttachShader(shaderProgram, vsShader);
  let _ = glAttachShader(shaderProgram, fsShader);
  let _ = glLinkProgram(shaderProgram);
  shaderProgram;
};

let run = () => {
  let _ = glfwInit();
  let w = glfwCreateWindow(100, 50, "test");
  glfwMakeContextCurrent(w);

  glfwSetWindowSize(w, 800, 600);

  glViewport(0, 0, 800, 600);

  let width = ref(800);
  let height = ref(600);

  let%lwt img = Image.load("test.jpg");
  Image.debug_print(img);
  let vsSource = {|
        #ifndef GL_ES
        #define lowp
        #endif

        attribute vec3 aVertexPosition;
        attribute vec4 aVertexColor;
        attribute vec2 aVertexTexCoord;

        uniform mat4 uProjectionMatrix;
        uniform mat4 uViewMatrix;
        uniform mat4 uWorldMatrix;

        varying lowp vec4 vColor;
        varying lowp vec2 vTexCoord;

        void main() {
            gl_Position = uProjectionMatrix * uViewMatrix * uWorldMatrix * vec4(aVertexPosition, 1.0);
            vColor = aVertexColor;
            vTexCoord = aVertexTexCoord;
        }
    |};
  print_endline(vsSource);
  let fsSource = {|
        #ifndef GL_ES
        #define lowp
        #endif

        varying lowp vec4 vColor;
        varying lowp vec2 vTexCoord;

        uniform sampler2D texture;

        void main() {
            gl_FragColor = vColor;
            // gl_FragColor = vec4(vTexCoord, 0.0, 1.0);
            //gl_FragColor = texture2D(texture, vTexCoord);
        }
    |};
  print_endline(fsSource);

  let positions = [|
    (-1.0),
    (-1.0),
    1.0,
    1.0,
    (-1.0),
    1.0,
    1.0,
    1.0,
    1.0,
    (-1.0),
    1.0,
    1.0,
    (-1.0),
    (-1.0),
    (-1.0),
    (-1.0),
    1.0,
    (-1.0),
    1.0,
    1.0,
    (-1.0),
    1.0,
    (-1.0),
    (-1.0),
    (-1.0),
    1.0,
    (-1.0),
    (-1.0),
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    (-1.0),
    (-1.0),
    (-1.0),
    (-1.0),
    1.0,
    (-1.0),
    (-1.0),
    1.0,
    (-1.0),
    1.0,
    (-1.0),
    (-1.0),
    1.0,
    1.0,
    (-1.0),
    (-1.0),
    1.0,
    1.0,
    (-1.0),
    1.0,
    1.0,
    1.0,
    1.0,
    (-1.0),
    1.0,
    (-1.0),
    (-1.0),
    (-1.0),
    (-1.0),
    (-1.0),
    1.0,
    (-1.0),
    1.0,
    1.0,
    (-1.0),
    1.0,
    (-1.0),
  |];
  let colors = [|
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
  |];
  let indices = [|
    0,
    1,
    2,
    0,
    2,
    3,
    4,
    5,
    6,
    4,
    6,
    7,
    8,
    9,
    10,
    8,
    10,
    11,
    12,
    13,
    14,
    12,
    14,
    15,
    16,
    17,
    18,
    16,
    18,
    19,
    20,
    21,
    22,
    20,
    22,
    23,
  |];
  let vArray = Float32Array.of_array(positions);
  let cArray = Float32Array.of_array(colors);
  let iArray = Uint16Array.of_array(indices);
  let shaderProgram = initShaderProgram(vsSource, fsSource);
  let vb = glCreateBuffer();
  let cb = glCreateBuffer();
  let ib = glCreateBuffer();
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, cb);
  glBufferData(GL_ARRAY_BUFFER, cArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iArray, GL_STATIC_DRAW);

  let posAttribute = glGetAttribLocation(shaderProgram, "aVertexPosition");
  let colorAttribute = glGetAttribLocation(shaderProgram, "aVertexColor");
  let worldUniform = glGetUniformLocation(shaderProgram, "uWorldMatrix");
  let viewUniform = glGetUniformLocation(shaderProgram, "uViewMatrix");
  let projectionUniform =
    glGetUniformLocation(shaderProgram, "uProjectionMatrix");

  let prevTime = ref(Unix.gettimeofday());

  let delta = ref(0.);
  let render = () => {
    let time = Unix.gettimeofday();
    delta := delta^ +. time -. prevTime^;
    prevTime := time;

    glClearColor(0.0, 0., 0., 1.);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glUseProgram(shaderProgram);
    let m = Mat4.create();
    let v = Vec3.create(0., 0., -10.);
    Mat4.fromTranslation(m, v);

    let rot = Mat4.create();
    Mat4.rotate(rot, delta^, Vec3.create(0., 0., 1.));

    let yRot = Mat4.create();
    Mat4.rotate(rot, delta^ *. 0.7, Vec3.create(0., 1., 0.));

    Mat4.multiply(rot, m, rot);
    Mat4.multiply(rot, yRot, rot);

    glUniformMatrix4fv(worldUniform, rot);

    glUniformMatrix4fv(viewUniform, Mat4.create());

    let projectionMatrix = Mat4.create();
    Mat4.perspective(
      projectionMatrix,
      45. *. (4.0 *. atan(1.0)) /. 180.,
      float_of_int(width^) /. float_of_int(height^),
      0.1,
      100.,
    );

    glUniformMatrix4fv(projectionUniform, projectionMatrix);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, false);
    glEnableVertexAttribArray(posAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, cb);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false);
    glEnableVertexAttribArray(colorAttribute);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    glfwSwapBuffers(w);
  };

  glfwSetFramebufferSizeCallback(
    w,
    (_, w, h) => {
      glViewport(0, 0, w, h);
      print_endline(
        "Size changed: " ++ string_of_int(w) ++ ", " ++ string_of_int(h),
      );
      width := w;
      height := h;
      render();
    },
  );

  glfwMaximizeWindow(w);

  glfwRenderLoop((_t) => {
    render();

    glfwPollEvents();
    glfwWindowShouldClose(w);
  })

  print_endline("Done!");
  glfwTerminate();
  Lwt.return();
};
