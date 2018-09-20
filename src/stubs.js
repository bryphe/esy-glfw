//Provides: caml_print_hello
function caml_print_hello() {
    console.log("Hello!");
}

//Provides: caml_glfwInit
function caml_glfwInit() {
    // no-op
};

// Provides: caml_test_callback_success
function caml_test_callback_success(s, f) {
    s(999);
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

    document.body.appendChild(canvas);
    return {
        canvas: canvas,
        title: title,
    };
};

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

    joo_global_object.variantToTexturePixelDataType = {
        '0': gl.UNSIGNED_BYTE
    }

    joo_global_object.variantToDrawMode = {
        '0': gl.TRIANGLES,
        '1': gl.TRIANGLE_STRIP
    }

    console.log("set context to: " + win.title)
}

function gl() {
    return window.__glfw__gl__
}

// Provides: caml_glClearColor
function caml_glClearColor(r, g, b, a) {
    gl().clearColor(r, g, b, a);
    gl().clear(gl().COLOR_BUFFER_BIT);
}

// Provides: caml_glClearDepth
function caml_glClearDepth(d) {
    gl().clearDepth(d);
    gl().clear(gl().DEPTH_BUFFER_BIT);
}

// Provides: caml_glfwWindowShouldClose
var i = 0;
function caml_glfwWindowShouldClose() {
    i++
    if (i > 1) {
        return true
    } else {
        return false
    }
}

// Provides: caml_glAttachShader
function caml_glAttachShader(program, shader) {
    gl().attachShader(program, shader);
}

// Provides: caml_glBindBuffer
function caml_glBindBuffer(target, buffer) {
    // TODO: target param
    gl().bindBuffer(gl().ARRAY_BUFFER, buffer)
}

// Provides: caml_glBufferData
function caml_glBufferData(target, data, usage) {
    // TODO: Remove size
    gl().bufferData(gl().ARRAY_BUFFER, data.data,  gl().STATIC_DRAW)
}

// Provides: caml_glCompileShader
function caml_glCompileShader(shader) {
    gl().compileShader(shader);
}

// Provides: caml_glCreateBuffer
function caml_glCreateBuffer() {
    return gl().createBuffer();
}

// Provides: caml_glCreateProgram
function caml_glCreateProgram() {
    return gl().createProgram();
}

// Provides: caml_glCreateShader
function caml_glCreateShader(type) {
    console.log("create shader: " + type);
    var glType;
    switch (type) {
        case 0:
            glType = gl().VERTEX_SHADER;
            break;
        case 1:
            glType = gl().FRAGMENT_SHADER;
            break;
        default:
            throw "Unrecognized shader type";
    }
    return gl().createShader(glType);
}

// Provides: caml_glDepthFunc
function caml_glDepthFunc(df) {
    // TODO: Use df parameter
    gl().depthFunc(gl().LEQUAL);
}

// Provides: caml_glDrawArrays
function caml_glDrawArrays(drawMode, first, count) {
    // TODO: Use params
    gl().drawArrays(gl().TRIANGLES, 0, 3);
}

// Provides: caml_glEnable
function caml_glEnable(option) {
    // TODO: Use params
    gl().enable(gl().DEPTH_TEST)
}

// Provides: caml_glGetShaderIsCompiled
function caml_glGetShaderIsCompiled() {
    // TODO: Refactor this
    return true;
}

// Provides: caml_glLinkProgram
function caml_glLinkProgram(program) {
    gl().linkProgram(program);
}

// Provides: caml_glGetAttribLocation
function caml_glGetAttribLocation(program, attribName) {
    return gl().getAttribLocation(program, attribName)
}

// Provides: caml_glGetUniformLocation
function caml_glGetUniformLocation(program, uniformName) {
    return gl().getUniformLocation(program, uniformName)
}

// Provides: caml_glUniformMatrix4fv
function caml_glUniformMatrix4fv(uniformLocation, uniformValue) {
    console.log("TODO: Remove hardcoding caml_glUniformMatrix4fv")
    return gl().uniformMatrix4fv(uniformLocation, false, new Float32Array([2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]));
}

// Provides: caml_glShaderSource
function caml_glShaderSource(shader, src) {
    gl().shaderSource(shader, src);
}

// Provides: caml_glUseProgram
function caml_glUseProgram(program) {
    gl().useProgram(program);
}

// Provides: caml_glCreateTexture
function caml_glCreateTexture() {
    return gl().createTexture();
}

// Provides: caml_glBindTexture
function caml_glBindTexture(vTextureType, vTexture) {
    var textureType = joo_global_object.variantToTextureType[vTextureType];
    console.log("TEXTURE TYPE: " + textureType.toString());
    gl().bindTexture(textureType, vTexture);
}

// Provides: caml_glTexParameteri
function caml_glTexParameteri(vTextureType, vTextureParameter, vTextureParameterValue) {
    var textureType = joo_global_object.variantToTextureType[vTextureType];
    var textureParameter = joo_global_object.variantToTextureParameter[vTextureParameter];
    var textureParameterValue = joo_global_object.variantToTextureParameterValue[vTextureParameterValue];

    console.log("type: " + textureType.toString() + " textureParameter: " + textureParameter.toString() + " tpv: " + textureParameterValue.toString());
    gl().texParameteri(textureType, textureParameter, textureParameterValue);
}

// Provides: caml_glVertexAttribPointer
function caml_glVertexAttribPointer(attributeLocation, numComponents) {
    // TODO: PARAMS!
    gl().vertexAttribPointer(attributeLocation, numComponents, gl().FLOAT, false, 0, 0);
}

// Provides: caml_glEnableVertexAttribArray
function caml_glEnableVertexAttribArray(attributeLocation) {
    gl().enableVertexAttribArray(attributeLocation);
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
