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

    struct WindowInfo {
        GLFWwindow* pWindow;
        value vSetFramebufferSizeCallback;
    };

    static WindowInfo* sActiveWindows[255];
    static int sActiveWindowCount = 0;

    WindowInfo* getWindowInfoFromWindow(GLFWwindow *w) {
        WindowInfo *pInfo;
        for (int i = 0; i < sActiveWindowCount; i++) {
            if (sActiveWindows[i] && sActiveWindows[i]->pWindow) {
                pInfo = sActiveWindows[i];
            }
        }
        return pInfo;
    }

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

    void framebuffer_size_callback(GLFWwindow *pWin, int iWidth, int iHeight) {
        // Is there a window info?
        WindowInfo * pWinInfo = getWindowInfoFromWindow(pWin);

        if (pWinInfo && pWinInfo->vSetFramebufferSizeCallback != Val_unit) {
            (void) caml_callback3((value)pWinInfo->vSetFramebufferSizeCallback, ((value)(void *)pWinInfo), Val_int(iWidth), Val_int(iHeight));
        }
    }

    CAMLprim value
    caml_glfwCreateWindow(value iWidth, value iHeight, value sTitle)
    {
      CAMLparam3(iWidth, iHeight, sTitle);

      GLFWwindow* wd;           /* window desciptor/handle */
      int w = Int_val(iWidth);
      int h = Int_val(iHeight);
      char *s = String_val(sTitle);

      glfwWindowHint(GLFW_SAMPLES, 4);

      wd = glfwCreateWindow(w, h, s,
                            NULL, NULL);

      struct WindowInfo* pWindowInfo = (WindowInfo *)malloc(sizeof(WindowInfo));
      pWindowInfo->pWindow = wd;
      pWindowInfo->vSetFramebufferSizeCallback = Val_unit;

      glfwSetFramebufferSizeCallback(wd, framebuffer_size_callback);

      sActiveWindows[sActiveWindowCount] = pWindowInfo;
      sActiveWindowCount++;

      CAMLreturn((value)pWindowInfo);
    }

    CAMLprim value
    caml_glfwSetWindowSize(value vWindow, value vWidth, value vHeight) {
        WindowInfo* pWindowInfo = (WindowInfo *)vWindow;
        glfwSetWindowSize(pWindowInfo->pWindow, Int_val(vWidth), Int_val(vHeight));
        return Val_unit;
    }

    CAMLprim value
    caml_glfwMaximizeWindow(value vWindow) {
        WindowInfo *pWindowInfo = (WindowInfo *)vWindow;
        glfwMaximizeWindow(pWindowInfo->pWindow);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwMakeContextCurrent(value window)
    {
        WindowInfo* wd = (WindowInfo *)window;
        glfwMakeContextCurrent(wd->pWindow);
        gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwSetFramebufferSizeCallback(value vWindow, value vCallback) {
        CAMLparam2(vWindow, vCallback);

        WindowInfo *pWinInfo = (WindowInfo *)vWindow;

        if (pWinInfo) {
            // TODO: Recycle existing callback if any!

            // We need to mark the closure as being a global root, so the garbage
            // collector knows it is being used.
            pWinInfo->vSetFramebufferSizeCallback = vCallback;
            caml_register_global_root(&(pWinInfo->vSetFramebufferSizeCallback));
        }

        CAMLreturn(Val_unit);
    }

    CAMLprim value
    caml_printFrameBufferSize(value window)
    {
        WindowInfo* wd = (WindowInfo*)window;
        int fbwidth;
        int fbheight;
        glfwGetFramebufferSize(wd->pWindow, &fbwidth, &fbheight);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwWindowShouldClose(value window)
    {
        WindowInfo *wd = (WindowInfo *)window;
        int val = glfwWindowShouldClose(wd->pWindow);
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
        WindowInfo *wd = (WindowInfo*)window;
        glfwSwapBuffers(wd->pWindow);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwTerminate(value unit)
    {
        glfwTerminate();
        return Val_unit;
    }
}
