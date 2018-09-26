#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <image.h>

extern "C" {

    CAMLprim value
    caml_test_callback_success(value vSuccess, value vFailure) {
        CAMLparam2(vSuccess, vFailure);

        caml_callback(vSuccess, Val_int(900));
        CAMLreturn(Val_unit);
    }

    CAMLprim value
    caml_test_callback_failure(value vSuccess, value vFailure) {
        CAMLparam2(vSuccess, vFailure);

        caml_callback(vFailure, caml_copy_string("failure!"));
        CAMLreturn(Val_unit);
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
    caml_glfwSetWindowSize(value vWindow, value vWidth, value vHeight) {
        GLFWwindow* w = (GLFWwindow *)vWindow;
        glfwSetWindowSize(w, Int_val(vWidth), Int_val(vHeight));
        return Val_unit;
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
