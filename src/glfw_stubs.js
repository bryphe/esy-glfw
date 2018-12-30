//Provides: caml_glfwInit
function caml_glfwInit() {
    joo_global_object._time = {
        start: Date.now(),
        offset: 0,
    };

    joo_global_object.window.addEventListener("resize", function () {
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyResize();
        }
    });

    joo_global_object._mouseState = {};
    joo_global_object.window.addEventListener("mousemove", function (e) {
        var activeWindow = joo_global_object._activeWindow;
        joo_global_object._mouseState.x = e.pageX - activeWindow.x;
        joo_global_object._mouseState.y = e.pageY - activeWindow.y;

        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            var win = wins[i];
            if (win.onCursorPos) {
                win.onCursorPos(win, e.pageX - win.x, e.pageY - win.y);
            }
        }
    });

    joo_global_object.window.addEventListener("wheel", function (e) {
        // TODO: account for deltaMode
        var deltaX = e.deltaX;
        var deltaY = e.deltaY;

        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyScroll(deltaX, deltaY);
        }
    });

    joo_global_object.window.addEventListener("keypress", function (keyEvent) {
        if (keyEvent.key && keyEvent.key.length === 1) {
            var codepoint = keyEvent.key.codePointAt(0);
            var wins = joo_global_object._activeWindows;
            for (var i = 0; i < wins.length; i++) {
                wins[i]._notifyChar(codepoint);
            }
        }
    });

    joo_global_object.window.addEventListener("mousedown", function (mouseEvent) {
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyMouseButton(mouseEvent, 0);
        }
    });

    joo_global_object.window.addEventListener("mouseup", function (mouseEvent) {
        var wins = joo_global_object._activeWindows;
        for (var i = 0; i < wins.length; i++) {
            wins[i]._notifyMouseButton(mouseEvent, 1);
        }
    });

    joo_global_object.window.addEventListener("keydown", function (keyEvent) {
        if (keyEvent.key) {
            var wins = joo_global_object._activeWindows;
            for (var i = 0; i < wins.length; i++) {
                var pressMode = keyEvent.repeat ? 2 : 0;
                wins[i]._notifyKey(keyEvent, pressMode);
            }
        }
    });

    joo_global_object.window.addEventListener("keyup", function (keyEvent) {
        if (keyEvent.key) {
            var wins = joo_global_object._activeWindows;
            for (var i = 0; i < wins.length; i++) {
                wins[i]._notifyKey(keyEvent, 1);
            }
        }
    });
};

// Provides: caml_glfwGetCursorPos
// Requires: caml_js_to_array
function caml_glfwGetCursorPos(w) {
    // TODO: Window parameter is currently ignored, but 
    // we should calculate the mouse position relative to it.

    return caml_js_to_array([joo_global_object._mouseState.x, joo_global_object._mouseState.y]);
}

// Provides: caml_glfwCreateStandardCursor
function caml_glfwCreateStandardCursor(shape) {
  switch (shape) {
  case 0: return "default";
  case 1: return "text";
  case 2: return "crosshair";
  case 3: return "pointer";
  case 4: return "ew-resize";
  case 5: return "ns-resize";
  default:
    joo_global_object.console.warn("Unsupported cursor shape.");
    return "default";
  }
}

// Provides: caml_glfwDestroyCursor
function caml_glfwDestroyCursor(cursor) {
  // no op
}

// Provides: caml_glfwSetCursor
function caml_glfwSetCursor(window, cursor) {
  window.canvas.style.cursor = cursor;
}

// Provides: caml_glfwGetTime_byte
function caml_glfwGetTime_byte() {
    return (joo_global_object._time.offset + (Date.now() - joo_global_object._time.start)) / 1000;
}

// Provides: caml_glfwSetTime_byte
function caml_glfwSetTime_byte(t) {
    joo_global_object._time.offset = t * 1000;
    joo_global_object._time.start = Date.now();
}

// Provides: caml_glfwGetPrimaryMonitor
function caml_glfwGetPrimaryMonitor() {
    // No-op
}

// Provides: caml_glfwGetVideoMode
function caml_glfwGetVideoMode() {
    var win = joo_global_object.window;
    return [0, win.innerWidth, win.innerHeight];
};

// Provides: caml_glfwGetMonitorPos
function caml_glfwGetMonitorPos() {
    return [0, 0, 0];
};

// Provides: caml_glfwGetFramebufferSize
function caml_glfwGetFramebufferSize(w) {
    var width = w.canvas.width;
    var height = w.canvas.height;
    return [0, width, height];
}

// Provides: caml_glfwDefaultWindowHints
function caml_glfwDefaultWindowHints(w) {
    joo_global_object.console.warn("glfwDefaultWindowHints not implemented in WebGL");
}

// Provides: caml_glfwShowWindow
function caml_glfwShowWindow(w) {
    joo_global_object.console.warn("glfwShowWindow not implemented in WebGL");
}

// Provides: caml_glfwHideWindow
function caml_glfwHideWindow(w) {
    joo_global_object.console.warn("glfwHideWindow not implemented in WebGL");
}

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
// Requires: caml_js_to_string
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
    var pixelRatio = joo_global_object.window.devicePixelRatio;
    canvas.width = width * pixelRatio;
    canvas.height = height * pixelRatio;

    joo_global_object._activeWindows = joo_global_object._activeWindows || [];

    document.body.appendChild(canvas);
    var w = {
        canvas: canvas,
        title: title,
        isMaximized: false,
        onSetFramebufferSize: null,
        onSetWindowSize: null,
        onKey: null,
        onChar: null,
        onCursorPos: null,
        onMouseButton: null,
        onScroll: null,
        x: 0,
        y: 0,
    };

    var notifyChar = function (codepoint) {
        if (w.onChar) {
            w.onChar(w, codepoint);
        }
    };

    var notifyKey = function (keyEvent, pressMode) {
        if (w.onKey) {

            if (keyEvent.key && keyEvent.key.length == 1) {
                var code = keyEvent.key.toUpperCase().charCodeAt(0);
                var modifier = 0;
                modifier = keyEvent.shiftKey ? modifier + 1 : modifier;
                modifier = keyEvent.ctrlKey ? modifier + 2 : modifier;
                modifier = keyEvent.altKey ? modifier + 4 : modifier;
                modifier = keyEvent.metaKey ? modifier + 8 : modifier

                w.onKey(w, code, keyEvent.location, pressMode, modifier);
            };
        }
    };

    var notifyMouseButton = function(mouseEvent, pressMode) {
        if (w.onMouseButton) {
            var modifier = 0;
            modifier = mouseEvent.shiftKey ? modifier + 1 : modifier;
            modifier = mouseEvent.ctrlKey ? modifier + 2 : modifier;
            modifier = mouseEvent.altKey ? modifier + 4 : modifier;
            modifier = mouseEvent.metaKey ? modifier + 8 : modifier

            var b = 0;
            switch(mouseEvent.button) {
                case 1:
                    b = 2;
                    break;
                case 2:
                    b = 1;
                    break;
                default:
                    b = mouseEvent.button;
            };

            w.onMouseButton(w, b, pressMode, modifier);
        }
    };

    var notifyResize = function () {
        if (w.isMaximized) {
            var pixelRatio = joo_global_object.window.devicePixelRatio;

            canvas.width = canvas.offsetWidth * pixelRatio;
            canvas.height = canvas.offsetHeight * pixelRatio;

            if (w.onSetFramebufferSize) {
                w.onSetFramebufferSize(w, canvas.width, canvas.height);
            }

            if (w.onSetWindowSize) {
                w.onSetWindowSize(w, canvas.offsetWidth, canvas.offsetHeight);
            }
        }
    };

    var notifyScroll = function (x, y) {
        if (w.onScroll) {
            w.onScroll(w, x, y);
        }
    };

    w._notifyResize = notifyResize;
    w._notifyChar = notifyChar;
    w._notifyScroll = notifyScroll;
    w._notifyKey = notifyKey;
    w._notifyMouseButton = notifyMouseButton;

    joo_global_object._activeWindows.push(w);
    return w;
};

// Provides: caml_glfwSetWindowPos
function caml_glfwSetWindowPos(w, x, y) {
    var canvas = w.canvas;
    canvas.style.transform = "translate(" + x.toString() + "px, " + y.toString() + "px)";
    w.x = x;
    w.y = y;
}

// Provides: caml_glfwSetWindowSize
function caml_glfwSetWindowSize(w, width, height) {
    var canvas = w.canvas;
    canvas.style.width = width.toString() + "px";
    canvas.style.height = height.toString() + "px";
    var pixelRatio = joo_global_object.window.devicePixelRatio;
    canvas.width = width * pixelRatio;
    canvas.height = height * pixelRatio;
}

// Provides: caml_glfwSetWindowTitle
function caml_glfwSetWindowTitle(w, title) {
    var t = title.toString();
    w.title = t;
    document.title = t;
}

// Provides: caml_glfwWindowHint
function caml_glfwWindowHint(hint, val) {
    joo_global_object.console.warn("Unsupported API: glfwWindowHint");
}
//
// Provides: caml_glfwSetWindowSizeCallback
function caml_glfwSetWindowSizeCallback(w, callback) {
    w.onSetWindowSize = callback;
}

// Provides: caml_glfwSetFramebufferSizeCallback
function caml_glfwSetFramebufferSizeCallback(w, callback) {
    w.onSetFramebufferSize = callback;
}

// Provides: caml_glfwSetScrollCallback
function caml_glfwSetScrollCallback(w, callback) {
    w.onScroll = callback;
}

// Provides: caml_glfwSetCursorPosCallback
function caml_glfwSetCursorPosCallback(w, callback) {
    w.onCursorPos = callback;
}

// Provides: caml_glfwSetCharCallback
function caml_glfwSetCharCallback(w, callback) {
    w.onChar = callback;
}

// Provides: caml_glfwSetKeyCallback
function caml_glfwSetKeyCallback(w, callback) {
    w.onKey = callback;
}

// Provides: caml_glfwSetMouseButtonCallback
function caml_glfwSetMouseButtonCallback(w, callback) {
    w.onMouseButton = callback;
}

// Provides: caml_glfwMaximizeWindow
function caml_glfwMaximizeWindow(w) {
    if (w && !w.isMaximized) {
        var pixelRatio = joo_global_object.window.devicePixelRatio;
        var canvas = w.canvas;
        canvas.style.width = "100%";
        canvas.style.height = "100%";
        canvas.width = canvas.offsetWidth * pixelRatio;
        canvas.height = canvas.offsetHeight * pixelRatio;
        w.isMaximized = true;
        w._notifyResize();
    }
}

// Provides: caml_glfwMakeContextCurrent
function caml_glfwMakeContextCurrent(win) {
    var context = win.canvas.getContext('webgl');
    var gl = context;
    joo_global_object.window.__glfw__gl__ = context;
    joo_global_object.window._activeWindow = win;

    joo_global_object.variantToTextureType = {
        '0': gl.TEXTURE_2D,
    };

    joo_global_object.variantToEnableOption = {
        '0': gl.DEPTH_TEST,
        '1': gl.BLEND,
        '2': gl.SCISSOR_TEST,
    };

    joo_global_object.variantToBlendFunc = {
        '0': gl.ZERO,
        '1': gl.ONE,
        '2': gl.SRC_ALPHA,
        '3': gl.ONE_MINUS_SRC_ALPHA,
    };

    joo_global_object.variantToTextureParameter = {
        '0': gl.TEXTURE_WRAP_S,
        '1': gl.TEXTURE_WRAP_T,
        '2': gl.TEXTURE_MIN_FILTER,
        '3': gl.TEXTURE_MAG_FILTER,
    };

    joo_global_object.variantToPixelAlignmentParameter = {
        '0': gl.PACK_ALIGNMENT,
        '1': gl.UNPACK_ALIGNMENT,
    };

    joo_global_object.variantToTextureParameterValue = {
        '0': gl.REPEAT,
        '1': gl.LINEAR,
        '2': gl.CLAMP_TO_EDGE,
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

    joo_global_object.gl = context;
}

// Provides: caml_glfwDestroyWindow
function caml_glfwDestroyWindow() {
    // no op
    // should we destroy the canvas window here?
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

// Provides: caml_glfwSwapInterval
function caml_glfwSwapInterval(swapInterval) {
    joo_global_object.console.warn("glfwSwapInterval not supported in WebGL");
}

// Provides: caml_glfwTerminate
function caml_glfwTerminate() {
    // no op
}
