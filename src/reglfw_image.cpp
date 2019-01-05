#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>

#include "reglfw_image.h"

extern "C" {
    CAMLprim value
    caml_createImage(value vWidth, value vHeight, value vNumChannels, value vChannelSize) {
      CAMLparam4(vWidth, vHeight, vNumChannels, vChannelSize);

      ReglfwImageInfo *ret = (ReglfwImageInfo *) malloc(sizeof(ReglfwImageInfo));
      ret->width = Int_val(vWidth);
      ret->height = Int_val(vHeight);
      ret->numChannels = Int_val(vNumChannels);
      ret->channelSize = Int_val(vChannelSize);
      ret->data =
        (unsigned char *) malloc(ret->width * ret->height * ret->numChannels * ret->channelSize);

      CAMLreturn((value) ret);
    }

    CAMLprim value
    caml_destroyImage(value vImage) {
      CAMLparam1(vImage);

      ReglfwImageInfo *image = (ReglfwImageInfo *) vImage;
      free(image->data);
      free(image);

      CAMLreturn(Val_unit);
    }

    CAMLprim value
    caml_getImageBuffer(value vImage) {
      CAMLparam1(vImage);
      ReglfwImageInfo *image = (ReglfwImageInfo *) vImage;
      CAMLreturn((value) image->data);
    }

    // Based on https://github.com/Jba03/glReadPixels_example/blob/master/tga.cpp
    CAMLprim value
    caml_saveImage(value vImage, value vPath) {
      CAMLparam2(vImage, vPath);
      ReglfwImageInfo *image = (ReglfwImageInfo *) vImage;

      uint8_t tga_header[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      uint8_t bitsPerPixel = 8 * image->numChannels * image->channelSize;
      // See http://www.paulbourke.net/dataformats/tga/
      uint8_t imageDescriptor =
        image->numChannels > 3 ? 0x28 : 0x20;
      uint16_t header[3] = {
        (uint16_t) image->width,
        (uint16_t) image->height,
        (uint16_t) (((uint16_t) imageDescriptor << 8) | (uint16_t) bitsPerPixel)
      };

      const char *path = String_val(vPath);
      FILE *fd = fopen(path, "wb");
      fwrite(tga_header, 1, 12, fd);
      fwrite(header, 2, 3, fd);
      fwrite(image->data, image->channelSize, image->width * image->height * image->numChannels, fd);
      fclose(fd);

      CAMLreturn(Val_unit);
    }
}
