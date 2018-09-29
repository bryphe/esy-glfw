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

  let%lwt img = Image.load("test.jpg");
  Image.debug_print(img);
  let vsSource = {|
        #ifndef GL_ES
        #define lowp
        #endif

        attribute vec3 aVertexPosition;
        attribute vec4 aVertexColor;
        attribute vec2 aVertexTexCoord;

        uniform mat4 transform;

        varying lowp vec4 vColor;
        varying lowp vec2 vTexCoord;

        void main() {
            gl_Position = transform * vec4(aVertexPosition, 1.0);
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
            //gl_FragColor = vColor;
            // gl_FragColor = vec4(vTexCoord, 0.0, 1.0);
            gl_FragColor = texture2D(texture, vTexCoord);
        }
    |};
  print_endline(fsSource);

  let positions = [|
    (-0.5),
    0.5,
    0.0,
    0.5,
    0.5,
    0.0,
    (-0.5),
    (-0.5),
    0.0,
    0.5,
    (-0.5),
    0.0,
  |];
  let colors = [|
    1.0,
    0.0,
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
    1.0,
    1.0,
    1.0,
    1.0,
  |];

  let indices = [|
    0, 1, 2,    1, 2, 3
  |];
  let textures = [|0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0|];

  let vArray = Float32Array.of_array(positions);
  let cArray = Float32Array.of_array(colors);
  let tArray = Float32Array.of_array(textures);
  let iArray = Uint16Array.of_array(indices);

  let shaderProgram = initShaderProgram(vsSource, fsSource);
  let vb = glCreateBuffer();
  let cb = glCreateBuffer();
  let tb = glCreateBuffer();
  let ib = glCreateBuffer();
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, cb);
  glBufferData(GL_ARRAY_BUFFER, cArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, tb);
  glBufferData(GL_ARRAY_BUFFER, tArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iArray, GL_STATIC_DRAW);

  /* Create texture */
  let texture = glCreateTexture();
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE, img);
  glGenerateMipmap(GL_TEXTURE_2D);

  let posAttribute = glGetAttribLocation(shaderProgram, "aVertexPosition");
  let colorAttribute = glGetAttribLocation(shaderProgram, "aVertexColor");
  let textureAttribute =
    glGetAttribLocation(shaderProgram, "aVertexTexCoord");
  let worldUniform = glGetUniformLocation(shaderProgram, "transform");

  let render = () => {
    glClearColor(0.0, 0., 0., 1.);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glUseProgram(shaderProgram);
    let m = Mat4.create();
    let v = Vec3.create();
    Vec3.set(v, 1., 2., 0.5);
    Mat4.fromScaling(m, v);
    glUniformMatrix4fv(worldUniform, m);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, false);
    glEnableVertexAttribArray(posAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, cb);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false);
    glEnableVertexAttribArray(colorAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, tb);
    glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, false);
    glEnableVertexAttribArray(textureAttribute);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    /* glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); */
    glfwSwapBuffers(w);
  };

  glfwSetFramebufferSizeCallback(
    w,
    (_, width, height) => {
      glViewport(0, 0, width, height);
      print_endline(
        "Size changed: "
        ++ string_of_int(width)
        ++ ", "
        ++ string_of_int(height),
      );
      render();
    },
  );

  glfwMaximizeWindow(w);

  while (!glfwWindowShouldClose(w)) {
    render();

    glfwPollEvents();
  };

  print_endline("Done!");
  glfwTerminate();
  Lwt.return();
};
