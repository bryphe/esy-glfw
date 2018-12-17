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
  let result = glLinkProgram(shaderProgram);
  switch (result) {
  | LinkSuccess => print_endline("Shader linked successfully.")
  | LinkFailure(v) => print_endline("Failed to link shader: " ++ v)
  };
  shaderProgram;
};

let run = () => {
  let _ = glfwInit();
  let w = glfwCreateWindow(100, 50, "test");
  glfwMakeContextCurrent(w);

  let monitor = glfwGetPrimaryMonitor();
  let vidMode = glfwGetVideoMode(monitor);

  glfwSetWindowPos(w, (vidMode.width - 800) / 2, (vidMode.height - 600) / 2);
  glfwSetWindowSize(w, 800, 600);
  glfwSetWindowTitle(w, "reason-glfw example");

  glViewport(0, 0, 800, 600);

  /* glfwSwapInterval sets the 'swap interval' - also known as vsync / vertical synchronization */
  /* A setting of 0 disables vsync, unless it is overridden by a driver or a user setting */
  glfwSwapInterval(0);

  let width = ref(800);
  let height = ref(600);

  /* Update pack alignment to allow single-channel images */
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  let%lwt img = Image.load("UVCheckerMap02-512.png");
  /* let img = Image.fromColor(255, 0, 0, 255); */
  let dimensions = Image.getDimensions(img);
  print_endline(
    "- width: "
    ++ string_of_int(dimensions.width)
    ++ " - height: "
    ++ string_of_int(dimensions.height),
  );

  let texture = glCreateTexture();
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, img);

  let frameBufferSize = glfwGetFramebufferSize(w);
  print_endline(
    "framebuffersize: "
    ++ string_of_int(frameBufferSize.width)
    ++ "x"
    ++ string_of_int(frameBufferSize.height),
  );

  let vsSource = {|
        #ifndef GL_ES
        #define lowp
        #else
        precision mediump float;
        #endif

        attribute vec3 aVertexPosition;
        attribute vec4 aVertexColor;

        uniform mat4 uProjectionMatrix;
        uniform mat4 uViewMatrix;
        uniform mat4 uWorldMatrix;

        varying lowp vec4 vColor;
        varying lowp vec3 vPos;

        void main() {
            gl_Position = uProjectionMatrix * uViewMatrix * uWorldMatrix * vec4(aVertexPosition, 1.0);
            vColor = aVertexColor;
            vPos = aVertexPosition;
        }
    |};
  print_endline(vsSource);
  let fsSource = {|
        #ifndef GL_ES
        #define lowp
        #else
        precision mediump float;
        #endif

        varying lowp vec4 vColor;
        varying lowp vec3 vPos;

        uniform sampler2D uSampler;

        void main() {
            vec4 t1 = texture2D(uSampler, vPos.yz);
            vec4 t2 = texture2D(uSampler, vPos.xz);
            vec4 t3 = texture2D(uSampler, vPos.xy);
            vec4 t = t1 + t2 + t3;
            gl_FragColor = vec4(vColor.r * t.r, vColor.g * t.g, vColor.b * t.b, 0.5);
            // gl_FragColor = vec4(vTexCoord, 0.0, 1.0);
            //gl_FragColor = texture2D(texture, vTexCoord);
        }
    |};
  print_endline(fsSource);

  /* Populate buffers for the cube geometry */
  let vArray = Float32Array.of_array(Cube.positions);
  let cArray = Float32Array.of_array(Cube.colors);
  let iArray = Uint16Array.of_array(Cube.indices);

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
    /* type glfwMouseButtonCallback = (window, glfwMouseButton, glfwButtonState, array(glfwModifierKey)) => unit; */
    /* let glfwSetMouseButtonCallback = (window, glfwMouseButtonCallback) => unit; */

    /* let glfwGetMouseButton = (window, glfwMouseButton) => glfwButtonState; */

    /* let glfwScrollCallback = (window, float, float) => unit; */
    /* type glfwSetScrollCallback = (window, glfwScrollCallback) => unit; */

    glClearColor(0.0, 0., 0., 1.);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);

    glUseProgram(shaderProgram);
    let m = Mat4.create();
    let v = Vec3.create(0., 0., -10.);
    Mat4.fromTranslation(m, v);

    let rot = Mat4.create();
    Mat4.rotate(rot, Angle.from_radians(delta^), Vec3.create(0., 0., 1.));

    let yRot = Mat4.create();
    Mat4.rotate(
      rot,
      Angle.from_radians(delta^ *. 0.7),
      Vec3.create(0., 1., 0.),
    );

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

  glfwSetScrollCallback(w, (_w, deltaX, deltaY) => {
    print_endline("SCROLL: " ++ string_of_float(deltaX) ++ ", " ++ string_of_float(deltaY));
  });

  glfwSetCursorPosCallback(w, (_w, x, y) => {
    print_endline("CURSOR: " ++ string_of_float(x) ++ ", " ++ string_of_float(y));
  })

  glfwSetKeyCallback(
    w,
    (_w, _key, _scancode, buttonState, m) => {
      let controlPressed = string_of_bool(Modifier.isControlPressed(m));
      let shiftPressed = string_of_bool(Modifier.isShiftPressed(m));
      /* let altPressed = Modifier.isAltPressed(m); */

      print_endline(
        "KEY: "
        ++ string_of_int(Obj.magic(_key))
        ++ "| ctrl: "
        ++ controlPressed
        ++ " | shift: "
        ++ shiftPressed
        ++ "| state: "
        ++ ButtonState.show(buttonState),
      );
    },
  );

  glfwSetCharCallback(w, (_w, codepoint) =>
    print_endline(
      "CHAR: "
      ++ string_of_int(codepoint)
      ++ " | "
      ++ String.make(1, Uchar.to_char(Uchar.of_int(codepoint))),
    )
  );

  glfwSetMouseButtonCallback(w, (_w, button, buttonState, m) => {
      let controlPressed = string_of_bool(Modifier.isControlPressed(m));
      let shiftPressed = string_of_bool(Modifier.isShiftPressed(m));
      print_endline(
        "Mouse button: "
        ++ MouseButton.show(button)
        ++ "| ctrl: "
        ++ controlPressed
        ++ " | shift: "
        ++ shiftPressed
        ++ "| state: "
        ++ ButtonState.show(buttonState),
      );
  })

  glfwSetFramebufferSizeCallback(
    w,
    (_, w, h) => {
      glViewport(0, 0, w, h);
      print_endline(
        "Framebuffer size changed: " ++ string_of_int(w) ++ ", " ++ string_of_int(h),
      );
      width := w;
      height := h;
      render();
    },
  );

  glfwSetWindowSizeCallback(
    w,
    (_, w, h) => {
      print_endline(
        "Window size changed: " ++ string_of_int(w) ++ ", " ++ string_of_int(h),
      );
      width := w;
      height := h;
      render();
    },
  );


  /* glfwMaximizeWindow(w); */

  glfwRenderLoop(_t => {
    render();

    /* Run the GC so we can catch any GC-related crashes early! */
    Gc.full_major();

    glfwPollEvents();
    glfwWindowShouldClose(w);
  });

  print_endline("Done!");
  glfwTerminate();
  Lwt.return();
};
