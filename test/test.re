open Reglfw.Glfw;

let () = {
    let _ = glfwInit();
    let w = glfwCreateWindow(800, 600, "test window");
    glfwMakeContextCurrent(w);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    let vsSource ={|
        void main() {
            gl_Position = vec4(1.0);
        }
    |};

    let shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, vsSource);
    let result = glCompileShader(shader);

    switch (result) {
    | CompilationSuccess => assert(true);
    | _ => assert(false);
    }
}
