#include <stdio.h>

#include <caml/mlvalues.h>

#define STB_IMAGE_IMPLEMENATION
#include "stb_image.h"

extern "C" {
    CAMLprim value
    caml_stb_image_print_hello(value unit)
    {
        printf("Hello \n");
        return Val_unit;
    }
}
