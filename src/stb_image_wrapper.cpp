#include <stdio.h>

#include <caml/mlvalues.h>

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
    caml_stb_image_load(value vPath) {
        int width, height, numChannels;
        unsigned char *data = stbi_load("test.jpg", &width, &height, &numChannels, 0);

        printf("Load result - width: %d height: %d numChannels: %d\n", width, height, numChannels);
        struct ImageInfo* pImageInfo = (ImageInfo *)malloc(sizeof(ImageInfo));
        pImageInfo->width = width;
        pImageInfo->height = height;
        pImageInfo->numChannels = numChannels;
        pImageInfo->data = data;
        return (value)pImageInfo;
    }

    CAMLprim value
    caml_stb_image_debug_print(value vImage) {
        ImageInfo *pImage = (ImageInfo*)vImage;

        printf("Debug - width: %d height: %d numChannels: %d\n", pImage->width, pImage->height, pImage->numChannels);
    }
}
