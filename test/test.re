open Reglfw.Glfw;

let runInGlContext = (test) => {
    let _ = glfwInit();
    let w = glfwCreateWindow(800, 600, "test window");
    glfwMakeContextCurrent(w);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    test();
};

let vertexShaderSource = {|
    void main() {
        gl_Position = vec4(1.0);
    }
|};

let fragmentShaderSource = {|
    void main() {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
|};

/* Compilation Success test */
let () = {
    runInGlContext(() => {
        let shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, vertexShaderSource);
        let result = glCompileShader(shader);

        switch (result) {
        | CompilationSuccess => assert(true);
        | _ => assert(false);
        }
    });
};

/* Compilation Failure test */
let () = {
    runInGlContext(() => {
        let vsSource ={|
            a bad shader
        |};

        let shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, vsSource);
        let result = glCompileShader(shader);

        switch (result) {
        | CompilationSuccess => assert(false);
        | CompilationFailure(_) => assert(true);
        }
    });
};

/* Link success test */
let () = {
    runInGlContext(() => {
        let vertexShader = glCreateShader(GL_VERTEX_SHADER);
        let fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, vertexShaderSource);
        glShaderSource(fragmentShader, fragmentShaderSource);

        let vertexCompileResult = glCompileShader(vertexShader);
        switch (vertexCompileResult) {
        | CompilationSuccess => print_endline("Vertex shader compiled successfully.");
        | CompilationFailure(msg) => print_endline("Vertex shader failed to compile with: " ++ msg);
        };

        let fragmentCompileResult = glCompileShader(fragmentShader);
        switch (fragmentCompileResult) {
        | CompilationSuccess => print_endline("Fragment shader compiled successfully.");
        | CompilationFailure(msg) => print_endline("Fragment shader failed to compile with: " ++ msg);
        };

        let program = glCreateProgram();
        let _ = glAttachShader(program, vertexShader);
        let _ = glAttachShader(program, fragmentShader);

        let result = glLinkProgram(program);

        switch (result) {
        | LinkSuccess => assert(true);
        | LinkFailure(msg) =>
            print_endline("[Unexpected link failure] " ++ msg);
            assert(false);
        };
    });
};

/* Link failure test */
let () = {
    runInGlContext(() => {
        let vertexShader = glCreateShader(GL_VERTEX_SHADER);
        let fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        /* Don't expose a main function, which will hit a linker error */
        let vertexShaderSource = {|
            void notmain() {
                gl_Position = vec4(1.0);
            }
        |};

        let fragmentShaderSource = {|
            void notmain() {
                gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
            };
        |};

        glShaderSource(vertexShader, vertexShaderSource);
        glShaderSource(fragmentShader, fragmentShaderSource);

        let _ = glCompileShader(vertexShader);
        let _ = glCompileShader(fragmentShader);

        let program = glCreateProgram();
        let _ = glAttachShader(program, vertexShader);
        let _ = glAttachShader(program, fragmentShader);

        let result = glLinkProgram(program);

        switch (result) {
        | LinkSuccess => assert(false);
        | LinkFailure(_) => assert(true);
        }
    });
};
