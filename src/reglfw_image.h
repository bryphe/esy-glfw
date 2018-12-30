extern "C" {
    struct ReglfwImageInfo {
        int width;
        int height;

        // Number of channels
        int numChannels;

        // Size of individual channel
        // 1 - 1 byte
        // 4 - 4 bytes (ie, float for HDR)
        int channelSize;

        // Actual data - should be width*height*numChannels*channelSize bytes!
        unsigned char* data;
    };

    CAMLprim value
    caml_createImage(value vWidth, value vHeight, value vNumChannels, value vChannelSize);

    CAMLprim value
    caml_destroyImage(value vImage);

    CAMLprim value
    caml_getImageBuffer(value vImage);

    CAMLprim value
    caml_saveImage(value vImage, value path);
}
