open EsyGlfw;
open EsyGlfw.Glfw;

let loadShader = (shaderType, source) => {
  let shader = glCreateShader(shaderType);
  let () = glShaderSource(shader, source);
  let () = glCompileShader(shader);
  let _ = glGetShaderIsCompiled(shader);
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

let () = {
  print_hello();
  let _ = glfwInit();
  let w = glfwCreateWindow(800, 600, "test");
  glfwMakeContextCurrent(w);

  let img = Image.load ("test.jpg");
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

  let positions = [|(-0.5), (-0.5), 0.0, 0.5, (-0.5), 0.0, 0.0, 0.5, 0.0|];
  let colors = [|1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0|];
  let textures = [|0.0, 0.0, 1.0, 0.0, 0.0, 1.0|];
  let vArray =
    Bigarray.Array1.of_array(Bigarray.Float32, Bigarray.C_layout, positions);
  let cArray =
    Bigarray.Array1.of_array(Bigarray.Float32, Bigarray.C_layout, colors);
  let tArray =
    Bigarray.Array1.of_array(Bigarray.Float32, Bigarray.C_layout, textures);
  let shaderProgram = initShaderProgram(vsSource, fsSource);
  let vb = glCreateBuffer();
  let cb = glCreateBuffer();
  let tb = glCreateBuffer();
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, cb);
  glBufferData(GL_ARRAY_BUFFER, cArray, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, tb);
  glBufferData(GL_ARRAY_BUFFER, tArray, GL_STATIC_DRAW);

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
  let textureAttribute = glGetAttribLocation(shaderProgram, "aVertexTexCoord");
  let worldUniform = glGetUniformLocation(shaderProgram, "transform");
  while (!glfwWindowShouldClose(w)) {
    glClearColor(0.0, 0., 0., 1.);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(worldUniform);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, false);
    glEnableVertexAttribArray(posAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, cb);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false);
    glEnableVertexAttribArray(colorAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, tb);
    glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, false);
    glEnableVertexAttribArray(textureAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(w);
    glfwPollEvents();
  };
  print_endline("Done!");
  glfwTerminate();
};
