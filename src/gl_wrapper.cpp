#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <reglfw_image.h>

// From: https://stackoverflow.com/questions/23177229/how-to-cast-int-to-const-glvoid
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

extern "C" {

    void warn(const char *message) {
        printf("[WARNING]: %s\n", message);
    }

    GLenum variantToBlendFunc(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_ZERO;
            case 1:
                return GL_ONE;
            case 2:
                return GL_SRC_ALPHA;
            case 3:
                return GL_ONE_MINUS_SRC_ALPHA;
            default:
                warn("Unexpected option for glBlendFunc");
                return 0;
        }
    }

    GLenum variantToEnableOption(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_DEPTH_TEST;
            case 1:
                return GL_BLEND;
            case 2:
                return GL_SCISSOR_TEST;
            default:
                warn("Unexpected option for glEnable");
                return 0;
        }
    }

    GLenum variantToType(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_FLOAT;
            case 1:
                return GL_UNSIGNED_BYTE;
            case 2:
                return GL_UNSIGNED_SHORT;
            case 3:
                return GL_UNSIGNED_SHORT_5_6_5;
            case 4:
                return GL_UNSIGNED_SHORT_4_4_4_4;
            case 5:
                return GL_UNSIGNED_SHORT_5_5_5_1;
            default:
                warn("Unexpected GL type!");
                return 0;
        }
    }

    GLenum variantToBufferType(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_ARRAY_BUFFER;
            case 1:
                return GL_ELEMENT_ARRAY_BUFFER;
            default:
                warn("Unexpected buffer type!");
                return 0;
        }
    }

    GLenum variantToPixelAlignmentParameter(value vVal) {
        switch (Int_val(vVal)) {
            case 0:
                return GL_PACK_ALIGNMENT;
            case 1:
                return GL_UNPACK_ALIGNMENT;
            default:
                warn ("Unexpected pixel alignment parameter type!");
                return GL_PACK_ALIGNMENT;
        }
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
            case 2:
                return GL_CLAMP_TO_EDGE;
            default:
                warn("Unexpected texture parameter value!");
                return 0;
        }
    }

    GLenum variantToDrawMode(value vDrawMode) {
        switch (Int_val(vDrawMode)) {
            case 0:
                return GL_TRIANGLES;
            case 1:
            default:
                return GL_TRIANGLE_STRIP;
        }
    }

    GLenum variantToFormat(value vFormat) {
        switch(Int_val(vFormat)) {
            case 0:
                return GL_ALPHA;
            case 1:
                return GL_RGB;
            case 2:
                return GL_RGBA;
            default:
                warn("Unsupported pixel format!");
                return 0;
        }
    }

    GLsizei formatToNumChannels(GLenum format) {
        switch(format) {
            case GL_ALPHA:
                return 1;
            case GL_RGB:
                return 3;
            case GL_RGBA:
                return 4;
            default:
                warn("Unsupported pixel format!");
                return 4;
        }
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
    caml_glViewport(value vX, value vY, value vWidth, value vHeight) {
        int x = Int_val(vX);
        int y = Int_val(vY);
        int width = Int_val(vWidth);
        int height = Int_val(vHeight);
        glViewport(x, y, width, height);
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
        glEnable(variantToEnableOption(vEnableOptions));
        return Val_unit;
    }

    CAMLprim value
    caml_glDisable(value vEnableOptions) {
        glDisable(variantToEnableOption(vEnableOptions));
        return Val_unit;
    }

    CAMLprim value
    caml_glScissor(value vX, value vY, value vWidth, value vHeight) {
        int x = Int_val(vX);
        int y = Int_val(vY);
        int width = Int_val(vWidth);
        int height = Int_val(vHeight);
        glScissor(x, y, width, height);
        return Val_unit;
    }

    CAMLprim value
    caml_glDepthFunc(value vDepthFunc) {
        glDepthFunc(GL_LEQUAL);
        return Val_unit;
    }

    CAMLprim value
    caml_glBlendFunc(value vSrcFunc, value vDestFunc) {
        glBlendFunc(variantToBlendFunc(vSrcFunc), variantToBlendFunc(vDestFunc));
        return Val_unit;
    }

    CAMLprim value
    caml_glCreateShader(value v) {
        int shaderType;
        switch (Int_val(v)) {
            case 0:
                shaderType = GL_VERTEX_SHADER;
                break;
            default:
            case 1:
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
        CAMLparam1(vShader);
        GLuint shader = (GLuint)vShader;
        glCompileShader(shader);

        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

        if (result == GL_TRUE) {
            CAMLreturn(Val_int(0));
        } else {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);

            CAMLlocal2(failure, log);
            failure = caml_alloc(1, 0);
            log = caml_copy_string(infoLog);
            Store_field(failure, 0, log);
            CAMLreturn(failure);
        }
    }

    CAMLprim value
    caml_glDeleteShader(value vShader) {
        GLuint shader = (GLuint)vShader;
        glDeleteShader(shader);
        return Val_unit;
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
        CAMLparam1(vProgram);
        unsigned int shaderProgram = (unsigned int)vProgram;
        glLinkProgram(shaderProgram);

        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        if (success == GL_TRUE) {
            CAMLreturn(Val_int(0));
        } else {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

            CAMLlocal2(failure, log);
            failure = caml_alloc(1, 0);
            log = caml_copy_string(infoLog);
            Store_field(failure, 0, log);
            CAMLreturn(failure);
        }
    }

    CAMLprim value
    caml_glGetAttribLocation(value vProgram, value vAttributeName) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        char *s;
        s = String_val(vAttributeName);
        int val = glGetAttribLocation(shaderProgram, s);
        return (value)val;
    }

    CAMLprim value
    caml_glGetUniformLocation(value vProgram, value vUniformName) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        char *s;
        s = String_val(vUniformName);

        int val = glGetUniformLocation(shaderProgram, s);
        return (value)val;
    }

    CAMLprim value
    caml_glUniform1f(value vUniformLocation, double f) {
        int iUniformLocation = (int)vUniformLocation;

        glUniform1f(iUniformLocation, f);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform1f_byte(value vUniformLocation, value v0) {
        return caml_glUniform1f(vUniformLocation, Double_val(v0));
    }

    CAMLprim value
    caml_glUniform2f(value vUniformLocation, double f0, double f1) {
        int iUniformLocation = (int)vUniformLocation;

        glUniform2f(iUniformLocation, f0, f1);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform2f_byte(value vUniformLocation, value v0, value v1) {
        return caml_glUniform2f(vUniformLocation, Double_val(v0), Double_val(v1));
    }

    CAMLprim value
    caml_glUniform3f(value vUniformLocation, double f0, double f1, double f2) {
        int iUniformLocation = (int)vUniformLocation;

        glUniform3f(iUniformLocation, f0, f1, f2);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform3f_byte(value vUniformLocation, value v0, value v1, value v2) {
        return caml_glUniform3f(vUniformLocation, Double_val(v0), Double_val(v1), Double_val(v2));
    }

    CAMLprim value
    caml_glUniform4f(value vUniformLocation, double f0, double f1, double f2, double f3) {
        int iUniformLocation = (int)vUniformLocation;

        glUniform4f(iUniformLocation, f0, f1, f2, f3);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform4f_byte(value vUniformLocation, value v0, value v1, value v2, value v3) {
        return caml_glUniform4f(vUniformLocation, Double_val(v0), Double_val(v1), Double_val(v2), Double_val(v3));
    }

    CAMLprim value
    caml_glUniform1i(value vUniformLocation, value v0) {
        int i0 = Int_val(v0);
        int iUniformLocation = (int)vUniformLocation;

        glUniform1i(iUniformLocation, i0);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform2i(value vUniformLocation, value v0, value v1) {
        int i0 = Int_val(v0);
        int i1 = Int_val(v1);
        int iUniformLocation = (int)vUniformLocation;

        glUniform2i(iUniformLocation, i0, i1);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform3i(value vUniformLocation, value v0, value v1, value v2) {
        int i0 = Int_val(v0);
        int i1 = Int_val(v1);
        int i2 = Int_val(v2);
        int iUniformLocation = (int)vUniformLocation;

        glUniform3i(iUniformLocation, i0, i1, i2);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform4i(value vUniformLocation, value v0, value v1, value v2, value v3) {
        int i0 = Int_val(v0);
        int i1 = Int_val(v1);
        int i2 = Int_val(v2);
        int i3 = Int_val(v3);
        int iUniformLocation = (int)vUniformLocation;

        glUniform4i(iUniformLocation, i0, i1, i2, i3);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform2fv(value vUniformLocation, value vVec2) {
        float *vec2 = (float *)(Data_custom_val(vVec2));
        int uloc = (int)vUniformLocation;

        glUniform2fv(uloc, 1, vec2);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform3fv(value vUniformLocation, value vVec3) {
        float *vec3 = (float *)(Data_custom_val(vVec3));
        int uloc = (int)vUniformLocation;

        glUniform3fv(uloc, 1, vec3);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniform4fv(value vUniformLocation, value vVec4) {
        float *vec4 = (float *)(Data_custom_val(vVec4));
        int uloc = (int)vUniformLocation;

        glUniform4fv(uloc, 1, vec4);
        return Val_unit;
    }

    CAMLprim value
    caml_glUniformMatrix4fv(value vUniformLocation, value vMat4) {
        float *mat4 = (float *)(Data_custom_val(vMat4));
        int uloc = (int)vUniformLocation;

        glUniformMatrix4fv(uloc, 1, GL_FALSE, mat4);
        return Val_unit;
    }

    CAMLprim value
    caml_glPixelStorei(value vPixelAlignmentParameter, value vParam) {
        glPixelStorei(variantToPixelAlignmentParameter(vPixelAlignmentParameter), Int_val(vParam));
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
    caml_glTexImage2D_native(
            value vTextureType,
            value vLevel,
            value vInternalFormat,
            value vFormat,
            value vType,
            value vPixels) {
        CAMLparam5(vTextureType, vLevel, vInternalFormat, vFormat, vType);
        CAMLxparam1(vPixels);
        CAMLlocal1(vData);

        GLenum format = variantToFormat(vFormat);
        GLsizei numChannels = formatToNumChannels(format);
        GLsizei width = Caml_ba_array_val(vPixels)->dim[1] / numChannels;
        GLsizei height = Caml_ba_array_val(vPixels)->dim[0];
        vData = (value)Caml_ba_data_val(vPixels);
        GLvoid *pPixels = (GLvoid *)vData;

        glTexImage2D(
                variantToTextureType(vTextureType),
                Int_val(vLevel),
                format,
                width,
                height,
                0,
                variantToFormat(vFormat),
                variantToType(vType),
                pPixels);

        CAMLreturn(Val_unit);
    }

    CAMLprim value
    caml_glTexImage2D_bytecode(value *argv, int argn)
    {
        return caml_glTexImage2D_native(
                argv[0],
                argv[1],
                argv[2],
                argv[3],
                argv[4],
                argv[5]);
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
        return (value)VBO;
    }

    CAMLprim value
    caml_glBindBuffer(value vBufferType, value vBuffer) {
        unsigned int VBO = (unsigned int)vBuffer;
        glBindBuffer(variantToBufferType(vBufferType), VBO);

        return Val_unit;
    }

    CAMLprim value
    caml_glBufferData(value vBufferType, value vData, value drawType) {
        int size = Caml_ba_array_val(vData)->dim[0];

        if ((Caml_ba_array_val(vData)->flags & CAML_BA_UINT16) == CAML_BA_UINT16) {
            unsigned short* elements = (unsigned short*)Caml_ba_data_val(vData);
            glBufferData(variantToBufferType(vBufferType), size * sizeof(unsigned short), elements, GL_STATIC_DRAW);
        } else if ((Caml_ba_array_val(vData)->flags & CAML_BA_FLOAT32) == CAML_BA_FLOAT32) {
            float* elements = (float*)Caml_ba_data_val(vData);
            glBufferData(variantToBufferType(vBufferType), size * sizeof(float), elements, GL_STATIC_DRAW);
        } else {
            warn("Unexpected Bigarray type!");
        }
        return Val_unit;
    }

    CAMLprim value
    caml_glDrawArrays(value vDrawMode, value vFirst, value vCount) {
        unsigned int first = Int_val(vFirst);
        unsigned int count = Int_val(vCount);
        glDrawArrays(variantToDrawMode(vDrawMode), first, count);
        return Val_unit;
    }

    CAMLprim value
    caml_glDrawElements(value vDrawMode, value vCount, value vGlType, value vFirst) {
       GLenum drawMode = variantToDrawMode(vDrawMode);
       GLenum dataType = variantToType(vGlType);
       unsigned int count = Int_val(vCount);
       unsigned int first = Int_val(vFirst);
       glDrawElements(drawMode, vCount, dataType, BUFFER_OFFSET(first));
       return Val_unit;
    }

    CAMLprim value
    caml_glEnableVertexAttribArray(value vIndex) {
        int index = (int)(vIndex);
        glEnableVertexAttribArray(index);
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
    caml_glReadPixels(value vX, value vY, value vFormat, value vType, value vPixels) {
        CAMLparam5(vX, vY, vFormat, vType, vPixels);

        GLint x = (GLint) Int_val(vX);
        GLint y = (GLint) Int_val(vY);

        GLenum format = variantToFormat(vFormat);
        GLenum type = variantToType(vType);

        int numChannels = format == GL_RGBA ? 4 : 3;

        GLsizei width = Caml_ba_array_val(vPixels)->dim[1] / numChannels;
        GLsizei height = Caml_ba_array_val(vPixels)->dim[0];
        GLvoid *pPixels = (GLvoid *)Caml_ba_data_val(vPixels);

        glReadPixels(x, y, width, height, format, type, pPixels);

        // If we're on a little-endian system, the R/B channels are swapped
        // So let's determine endianness...
        unsigned int marker = 0x12345678;
        if (*(char *) &marker == 0x78 && type == GL_UNSIGNED_BYTE) {
            // We are little-endian. Onto the swap...
            for (int i = 0; i < width * height * numChannels; i += numChannels) {
            uint8_t tmp = *((uint8_t *) pPixels + i);
            *((uint8_t *) pPixels + i) = *((uint8_t *) pPixels + i + 2);
            *((uint8_t *) pPixels + i + 2) = tmp;
            }
        }

        CAMLreturn(Val_unit);
    }
}
