#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <image.h>

extern "C" {

    void warn(const char *message) {
        printf("[WARNING]: %s\n", message);
    }

    GLenum variantToTextureType(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_TEXTURE_2D;
            default:
                warn("Unexpected texture type!");
                return 0;
        }
    }

    GLenum variantToTextureParameter(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_TEXTURE_WRAP_S;
            case 1:
                return GL_TEXTURE_WRAP_T;
            case 2:
                return GL_TEXTURE_MIN_FILTER;
            case 3:
                return GL_TEXTURE_MAG_FILTER;
            default:
                warn("Unexpected texture parameter!");
                return 0;
        }
    }

    GLenum variantToTextureParameterValue(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_REPEAT;
            case 1:
                return GL_LINEAR;
            default:
                warn("Unexpected texture parameter value!");
                return 0;
        }
    }

    GLenum variantToTexturePixelDataFormat(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_RGB;
            case 1:
                return GL_RGBA;
            default:
                warn ("Unexpected texture pixel data format!");
                return 0;
        }
    }

    GLenum variantToTexturePixelDataType(value vVal) {
        return GL_UNSIGNED_BYTE;
    }

    CAMLprim value
    caml_print_hello(value unit)
    {
        printf("Hello \n");
        return Val_unit;
    }

    CAMLprim value
    caml_glfwInit(value unit)
    {
        int ret = glfwInit();
        return Val_bool(ret);
    }

    CAMLprim value
    caml_glfwCreateWindow(value iWidth, value iHeight, value sTitle)
    {
      GLFWwindow* wd;           /* window desciptor/handle */
      int w = Int_val(iWidth);
      int h = Int_val(iHeight);
      char *s = String_val(sTitle);

      wd = glfwCreateWindow(w, h, s,
                            NULL, NULL);
      return (value) wd;
    }

    CAMLprim value
    caml_glfwMakeContextCurrent(value window)
    {
        GLFWwindow* wd = (GLFWwindow *)window;
        glfwMakeContextCurrent(wd);
        gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
        return Val_unit;
    }

    CAMLprim value
    caml_printFrameBufferSize(value window)
    {

        GLFWwindow *wd = (GLFWwindow*)window;
        int fbwidth;
        int fbheight;
        glfwGetFramebufferSize(wd, &fbwidth, &fbheight);

        printf("size2: %d %d\n", fbwidth, fbheight);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwWindowShouldClose(value window)
    {
        GLFWwindow *wd = (GLFWwindow*)window;
        int val = glfwWindowShouldClose(wd);
        return Val_bool(val);
    }

    CAMLprim value
    caml_glfwPollEvents(value unit) 
    {
        glfwPollEvents();
        return Val_unit;
    }

    CAMLprim value
    caml_glClearColor(value vr, value vg, value vb, value va) {
        float r = Double_val(vr);
        float g = Double_val(vg);
        float b = Double_val(vb);
        float a = Double_val(va);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
        return Val_unit;
    }

    CAMLprim value
    caml_glClearDepth(value vd) {
        float d = Double_val(vd);
        glClearDepthf(d);
        glClear(GL_DEPTH_BUFFER_BIT);
        return Val_unit;
    }

    CAMLprim value
    caml_glEnable(value vEnableOptions) {
        glEnable(GL_DEPTH_TEST);
        return Val_unit;
    }

    CAMLprim value
    caml_glDepthFunc(value vDepthFunc) {
        glDepthFunc(GL_LEQUAL);
        return Val_unit;
    }

    CAMLprim value
    caml_glCreateShader(value v) {
        int shaderType;
        switch (Int_val(v)) {
            case 0:
                printf("vertex shader");
                shaderType = GL_VERTEX_SHADER;
                break;
            default:
            case 1:
                printf("fragment shader");
                shaderType = GL_FRAGMENT_SHADER;
                break;
        }

        return (value) glCreateShader(shaderType);
    }

    CAMLprim value
    caml_glShaderSource(value vShader, value vSource) {
        GLuint shader = (GLuint)vShader;
        char *s;
        s = String_val(vSource);
        glShaderSource(shader, 1, &s, NULL);
        return Val_unit;
    }

    CAMLprim value
    caml_glCompileShader(value vShader) {
        GLuint shader = (GLuint)vShader;
        glCompileShader(shader);
        return Val_unit;
    }

    CAMLprim value
    caml_glDeleteShader(value vShader) {
        GLuint shader = (GLuint)vShader;
        glDeleteShader(shader);
    }

    CAMLprim value
    caml_glGetShaderIsCompiled(value vShader) {
        GLuint shader = (GLuint)vShader;

        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

        if (result == GL_TRUE) {
            printf("Compiled! %d\n", shader);
            return Val_true;
        } else {
            printf("Not compiled! %d\n", shader);
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("Output: %s\n", infoLog);
            return Val_false;
        }
    }

    CAMLprim value
    caml_glCreateProgram(value unit) {
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        return (value)shaderProgram;
    }

    CAMLprim value
    caml_glAttachShader(value vProgram, value vShader) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        unsigned int shader = (unsigned int)vShader;
        glAttachShader(shaderProgram, shader);
        return Val_unit;
    }

    CAMLprim value
    caml_glLinkProgram(value vProgram) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        glLinkProgram(shaderProgram);

        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        char infoLog[512];
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("link failed: %s\n", infoLog);
        } else {
            printf("link success!\n");
        }
        return Val_unit;
    }

    CAMLprim value
    caml_glGetAttribLocation(value vProgram, value vAttributeName) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        char *s;
        s = String_val(vAttributeName);
        int val = glGetAttribLocation(shaderProgram, s);
        printf("- attribute: %s - loc: %d\n", s, val);
        return (value)val;
    }

    CAMLprim value
    caml_glGetUniformLocation(value vProgram, value vAttributeName) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        char *s;
        s = String_val(vAttributeName);

        int val = glGetUniformLocation(shaderProgram, s);
        printf(" -uniform: %s - loc: %d\n", s, val);
        return (value)val;
    }

    // TODO: Real matrix
    CAMLprim value
    caml_glUniformMatrix4fv(value vUniformLocation) {
        float mat[] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        int uloc = (int)vUniformLocation;

        glUniformMatrix4fv(uloc, 1, GL_FALSE, &mat[0]);
        return Val_unit;
    }

    CAMLprim value
    caml_glCreateTexture(value vUnit) {
        unsigned int texture;
        glGenTextures(1, &texture);
        return (value)texture;
    }

    CAMLprim value
    caml_glBindTexture(value vTextureType, value vTexture) {
        unsigned int texture = (unsigned int)vTexture;
        glBindTexture(variantToTextureType(vTextureType), texture);
        return Val_unit;
    }

    CAMLprim value
    caml_glTexImage2D(value vTextureType, value vTexturePixelDataFormat, value vTexturePixelDataType, value vImage) {
        ImageInfo *pImage = (ImageInfo *)vImage;
        glTexImage2D(
                variantToTextureType(vTextureType), 
                0,
                GL_RGB,
                pImage->width,
                pImage->height,
                0,
                variantToTexturePixelDataFormat(vTexturePixelDataFormat), 
                variantToTexturePixelDataType(vTexturePixelDataType), 
                pImage->data);
        return Val_unit;
    }

    CAMLprim value
    caml_glTexParameteri(value vTextureType, value vTextureParameter, value vTextureParameterValue) {
        glTexParameteri(
                variantToTextureType(vTextureType),
                variantToTextureParameter(vTextureParameter),
                variantToTextureParameterValue(vTextureParameterValue));
        return Val_unit;
    }

    CAMLprim value
    caml_glGenerateMipmap(value vTextureType) {
        glGenerateMipmap(variantToTextureType(vTextureType));
        return Val_unit;
    }

    CAMLprim value
    caml_glUseProgram(value vProgram) {
        unsigned int program = (unsigned int)vProgram;
        glUseProgram(program);
        return Val_unit;
    }

    CAMLprim value
    caml_glCreateBuffer(value unit) {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        printf("glCreateBuffer: %d \n", VBO);
        return (value)VBO;
    }

    CAMLprim value
    caml_glBindBuffer(value vBufferType, value vBuffer) {
        unsigned int VBO = (unsigned int)vBuffer;
        /* printf("bind buffer: %d\n", VBO); */

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        /* printf("glBindBuffer: %d\n", VBO); */
        return Val_unit;
    }

    CAMLprim value
    caml_glBufferData(value vBufferType, value vData, value drawType) {
        /* printf("glBufferData - show floats TODO\n"); */
        int size = Caml_ba_array_val(vData)->dim[0];
        float* elements = (float*)Caml_ba_data_val(vData);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), elements, GL_STATIC_DRAW);
        return Val_unit;
    }

    CAMLprim value
    caml_glDrawArrays(value vDrawMode, value vFirst, value vCount) {
        // TODO: Use param
        unsigned int first = Int_val(vFirst);
        unsigned int count = Int_val(vCount);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        return Val_unit;
    }

    CAMLprim value
    caml_glEnableVertexAttribArray(value vAttributeLocation) {
        int attributeLocation = (int)(vAttributeLocation);
        glEnableVertexAttribArray(attributeLocation);
        return Val_unit;
    }

    CAMLprim value
    caml_glVertexAttribPointer(value vAttrib, value vNumComponents) {
        // TODO: Params!
        int attributeLocation = (int)(vAttrib);
        int numComponents = Int_val(vNumComponents);
        glVertexAttribPointer(attributeLocation, numComponents, GL_FLOAT, GL_FALSE, 0, (void*)0);
        return Val_unit;
    }

    CAMLprim value
    caml_glUnbindBuffer(value unit) {
        printf("TODO: glUnbindBuffer\n");
        return Val_unit;
    }

    CAMLprim value
    caml_glfwSwapBuffers(value window)
    {
        GLFWwindow *wd = (GLFWwindow*)window;
        glfwSwapBuffers(wd);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwTerminate(value unit)
    {
        glfwTerminate();
        return Val_unit;
    }
}
