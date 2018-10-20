#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include "stb_image.h"
#include "reglfw_image.h"

extern "C" {

    CAMLprim value
    caml_stb_image_load(value vPath, value vSuccess, value vFailure) {
        CAMLparam3(vPath, vSuccess, vFailure);
        int width, height, numChannels;
        
        char* path = String_val(vPath);
        printf("Loading image %s\n", path);

        unsigned char *data = stbi_load(path, &width, &height, &numChannels, 0);

        if (!data) {
            caml_callback(vFailure, caml_copy_string("Unable to load image."));
        } else {
            printf("Load result - width: %d height: %d numChannels: %d\n", width, height, numChannels);
            struct ReglfwImageInfo* pImageInfo = (ReglfwImageInfo *)malloc(sizeof(ReglfwImageInfo));
            pImageInfo->width = width;
            pImageInfo->height = height;
            pImageInfo->numChannels = numChannels;
            pImageInfo->channelSize = 1;
            pImageInfo->data = data;
            caml_callback(vSuccess, (value)pImageInfo);
        }

        CAMLreturn(Val_unit);
    }

    CAMLprim value
    caml_stb_image_dimensions(value vImage) {
        CAMLparam1(vImage);
        CAMLlocal1(ret);
        ReglfwImageInfo *pImage = (ReglfwImageInfo*)vImage;

        ret = caml_alloc(2, 0);
        Store_field(ret, 0, Val_int(pImage->width));
        Store_field(ret, 1, Val_int(pImage->height));

        CAMLreturn(ret);
    }

    CAMLprim value
    caml_stb_image_from_color(value vR, value vG, value vB, value vA) {
        unsigned char r = (unsigned char)Int_val(vR);
        unsigned char g = (unsigned char)Int_val(vG);
        unsigned char b = (unsigned char)Int_val(vB);
        unsigned char a = (unsigned char)Int_val(vA);

        unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * 4);
        data[0] = r;
        data[1] = g;
        data[2] = b;
        data[3] = a;

        struct ReglfwImageInfo* pImageInfo = (ReglfwImageInfo *)malloc(sizeof(ReglfwImageInfo));
        pImageInfo->width = 1;
        pImageInfo->height = 1;
        pImageInfo->numChannels = 4;
        pImageInfo->channelSize = 1;
        pImageInfo->data = data;

        return (value)pImageInfo;
    }

    CAMLprim value
    caml_stb_image_debug_print(value vImage) {
        ReglfwImageInfo *pImage = (ReglfwImageInfo*)vImage;

        printf("Debug - width: %d height: %d numChannels: %d\n", pImage->width, pImage->height, pImage->numChannels);
        return Val_unit;
    }
}
