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
    // This case will almost certainly never happen, since numChannels & channelSize
    // are both ints and we almost always have >=3 channels
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

// Provides: caml_getImageBuffer
function caml_getImageBuffer(image) {
  return image.data;
}

var caml_saveImage_anchor;

// Provides: caml_saveImage
function caml_saveImage(image, path) {
  var buffer = new ArrayBuffer(18 + image.data.byteLength);
  var bufferView = new DataView(buffer);

  var tga_header = [0, 0, 2, 0, 0, 0,
                    0, 0, 0, 0, 0, 0];
  var bitsPerPixel = 8 * image.numChannels * image.channelSize;
  var imageDescriptor = image.numChannels > 3 ? 0x28 : 0x20;
  var header = [image.width, image.height, (imageDescriptor << 8) | bitsPerPixel];

  // Copy in all the data to the new buffer
  var bufferIndex = 0;
  for (var i = 0; i < tga_header.length; i++) {
    bufferView.setUint8(bufferIndex, tga_header[i]);
    bufferIndex++;
  }
  for (var i = 0; i < header.length; i++) {
    // The true parameter tells this to use little endianness, which
    // is required for the TGA file
    bufferView.setUint16(bufferIndex, header[i], true);
    bufferIndex += 2;
  }
  if (image.data instanceof Uint8Array) {
    for (var i = 0; i < image.data.length; i++) {
      bufferView.setUint8(bufferIndex, image.data[i]);
      bufferIndex++;
    }
  } else if (image.data instanceof Uint16Array) {
    for (var i = 0; i < image.data.length; i++) {
      bufferView.setUint16(bufferIndex, image.data[i], true);
      bufferIndex += 2;
    }
  } else {
    throw "TGA image saving only supports Uint8Array and Uint16Array buffers!";
  }

  // Based on https://gist.github.com/liabru/11263260
  // and https://ourcodeworld.com/articles/read/189/how-to-create-a-file-and-generate-a-download-with-javascript-in-the-browser-without-a-server
  function download(filename, text) {
    var blob = new Blob([text], {type: "application/octet-binary"});

    if (!caml_saveImage_anchor) {
      caml_saveImage_anchor = document.createElement('a');
      document.body.appendChild(caml_saveImage_anchor);
      caml_saveImage_anchor.style.display = 'none';
    } else {
      // We need to revoke ObjectURLs because they don't get collected by the
      // browser GC.
      var objUrl = caml_saveImage_anchor.getAttribute('href');
      URL.revokeObjectURL(objUrl);
    }

    caml_saveImage_anchor.setAttribute('href', URL.createObjectURL(blob));
    caml_saveImage_anchor.setAttribute('download', filename);

    caml_saveImage_anchor.click();
  }

  download(path, bufferView);
}
