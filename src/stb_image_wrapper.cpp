#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include "stb_image.h"

#include "image.h"

extern "C" {

    CAMLprim value
    caml_stb_image_print_hello(value unit)
    {
        printf("Hello with stb headers!! \n");
        return Val_unit;
    }

    CAMLprim value
    caml_stb_image_load(value vPath, value vSuccess, value vFailure) {
        CAMLparam3(vPath, vSuccess, vFailure);
        int width, height, numChannels;
        unsigned char *data = stbi_load("test.jpg", &width, &height, &numChannels, 0);

        if (!data) {
            caml_callback(vFailure, caml_copy_string("Unable to load image."));
        } else {
            printf("Load result - width: %d height: %d numChannels: %d\n", width, height, numChannels);
            struct ImageInfo* pImageInfo = (ImageInfo *)malloc(sizeof(ImageInfo));
            pImageInfo->width = width;
            pImageInfo->height = height;
            pImageInfo->numChannels = numChannels;
            pImageInfo->data = data;
            caml_callback(vSuccess, (value)pImageInfo);
        }

        CAMLreturn(Val_unit);
    }

    CAMLprim value
    caml_stb_image_debug_print(value vImage) {
        ImageInfo *pImage = (ImageInfo*)vImage;

        printf("Debug - width: %d height: %d numChannels: %d\n", pImage->width, pImage->height, pImage->numChannels);
    }
}
