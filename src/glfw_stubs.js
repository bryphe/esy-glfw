//Provides: caml_print_hello
function caml_print_hello(stub) {
    console.log("Hello!");
    console.dir(stub);
}

//Provides: caml_glfwInit
function caml_glfwInit() {
    // no-op

    joo_global_object.window.addEventListener("resize", function () {
        console.log("RESIZE");
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            console.log("notifying");
            wins[i]._notifyResize();
        }
    });
};

// Provides: caml_test_callback_success
function caml_test_callback_success(s, f) {
    s(999);
}

// Provides: caml_glfwJavascriptRenderLoop
function caml_glfwJavascriptRenderLoop(loopFunc) {
    function renderLoop(t) {
        var ret = loopFunc(t);

        if (!ret) {
            joo_global_object.window.requestAnimationFrame(renderLoop);
        }
    }

    joo_global_object.window.requestAnimationFrame(renderLoop);
}

// Provides: caml_test_callback_failure
function caml_test_callback_failure(s, f) {
    f(caml_js_to_string("failed"));
}

//Provides: caml_glfwCreateWindow
function caml_glfwCreateWindow(width, height, title) {
    var canvas = document.createElement("canvas");
    canvas.style.position = "absolute";
    canvas.style.top = "0px";
    canvas.style.bottom = "0px";
    canvas.style.width = width.toString() + "px";
    canvas.style.height = height.toString() + "px";
    canvas.width = width;
    canvas.height = height;

    joo_global_object._activeWindows = joo_global_object._activeWindows || [];

    document.body.appendChild(canvas);
    var w = {
        canvas: canvas,
        title: title,
        isMaximized: false,
        onSetFramebufferSize: null,
    };

    var notifyResize = function () {
        if (w.isMaximized) {
            canvas.width = canvas.offsetWidth;
            canvas.height = canvas.offsetHeight;

            if (w.onSetFramebufferSize) {
                w.onSetFramebufferSize(w, canvas.offsetWidth, canvas.offsetHeight);
            }
        }
    };

    w._notifyResize = notifyResize;

    joo_global_object._activeWindows.push(w);
    return w;
};

// Provides: caml_glfwSetWindowSize
function caml_glfwSetWindowSize(w, width, height) {
    var canvas = w.canvas;
    canvas.style.width = width.toString() + "px";
    canvas.style.height = height.toString() + "px";
    canvas.width = width;
    canvas.height = height;
}

// Provides: caml_glfwSetFramebufferSizeCallback
function caml_glfwSetFramebufferSizeCallback(w, callback) {
    w.onSetFramebufferSize = callback;
}

// Provides: caml_glfwMaximizeWindow
function caml_glfwMaximizeWindow(w) {
    if (w && !w.isMaximized) {
        var canvas = w.canvas;
        canvas.style.width = "100%";
        canvas.style.height = "100%";
        canvas.width = canvas.offsetWidth;
        canvas.height = canvas.offsetHeight;
        w.isMaximized = true;
        w._notifyResize();
    }
}

// Provides: caml_glfwMakeContextCurrent
function caml_glfwMakeContextCurrent(win) {
    var context = win.canvas.getContext('webgl');
    var gl = context;
    window.__glfw__gl__ = context;

    joo_global_object.variantToTextureType = {
        '0': gl.TEXTURE_2D,
    };

    joo_global_object.variantToTextureParameter = {
        '0': gl.TEXTURE_WRAP_S,
        '1': gl.TEXTURE_WRAP_T,
        '2': gl.TEXTURE_MIN_FILTER,
        '3': gl.TEXTURE_MAG_FILTER,
    };

    joo_global_object.variantToTextureParameterValue = {
        '0': gl.REPEAT,
        '1': gl.LINEAR
    }

    joo_global_object.variantToTexturePixelDataFormat = {
        '0': gl.RGB,
        '1': gl.RGBA,
    }

    joo_global_object.variantToGlType = {
        '0': gl.FLOAT,
        '1': gl.UNSIGNED_BYTE,
        '2': gl.UNSIGNED_SHORT,
    }

    joo_global_object.variantToBufferType = {
        '0': gl.ARRAY_BUFFER,
        '1': gl.ELEMENT_ARRAY_BUFFER,
    }

    joo_global_object.variantToDrawMode = {
        '0': gl.TRIANGLES,
        '1': gl.TRIANGLE_STRIP
    }

    console.log("set context to: " + win.title)
    joo_global_object.gl = context;
}

// Provides: caml_glfwWindowShouldClose
function caml_glfwWindowShouldClose() {
    return false
}

// Provides: caml_glfwPollEvents
function caml_glfwPollEvents() {
    // no op
}

// Provides: caml_glfwSwapBuffers
function caml_glfwSwapBuffers() {
    // no op
}

// Provides: caml_glfwTerminate
function caml_glfwTerminate() {
    // no op
}
