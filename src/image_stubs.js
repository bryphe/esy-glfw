// Provides: caml_createImage
function caml_createImage(width, height, numChannels, channelSize) {
    var data;

    /* HACK: WebGL readPixels() requires us to have the correct kind of ArrayBufferView.
       That means that we need to guess what type of array buffer view the user is going
       to need. Here's what we can guess: if the pixel size is 3 bytes it's definitely
       GL_UNSIGNED_BYTE. If it's 2 bytes, it's definitely some form of GL_UNSIGNED_SHORT.
       If it's 4 bytes, the user either wants a GL_FLOAT or GL_UNSIGNED_BYTE buffer.
       There's really no great way to know what they want here, so the best we can do is
       pick one and document that. For now, we'll go with GL_UNSIGNED_BYTE. Maybe we
       should just remove the GL_FLOAT option entirely? */
     https://developer.mozilla.org/en-US/docs/Web/API/WebGLRenderingContext/readPixels */
    switch (numChannels * channelSize) {
    case 2:
      data = new Uint16Array(width * height);
      break;
    case 3: case 4:
      data = new Uint8Array(width * height * numChannels * channelSize);
      break;
    default:
      throw "Number of channels/channel size is unsupported.";
    }

    return {
      width: width,
      height: height,
      numChannels: numChannels,
      channelSize: channelSize,
      data: data
    };
}

// Provides: caml_destroyImage
function caml_destroyImage(image) {
  // no op
}

// Provides: caml_getBuffer
function caml_getBuffer(image) {
  return image.data;
}

// Provides: caml_saveImage
function caml_saveImage(image, path) {
  // TODO: Implement this
}
