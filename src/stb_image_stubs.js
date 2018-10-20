// Provides: caml_stb_image_load
// Requires: caml_js_to_string
function caml_stb_image_load(imgName, onSuccess, onFailure) {
    var image = new joo_global_object.Image();
    image.onload = function () {
        onSuccess(image);
    };

    image.onerror = function (err) {
        joo_global_object.console.log("failed to load");
        onFailure(caml_js_to_string("failed to load image: " + err.toString()));
    };

    image.src = "test.jpg";
}

// Provides: caml_stb_image_debug_print
function caml_stb_image_debug_print(img) {
    joo_global_object.console.log("Image width: " + img.width + " Image height: " + img.height);
}

// Provides: caml_stb_image_dimensions
function caml_stb_image_dimensions(img) {
    return [0, img.width, img.height];
}

// Provides: caml_stb_image_from_color
function caml_stb_image_from_color(r, g, b, a) {
    var pixel = new joo_global_object.Uint8Array([r, g, b, a]);
    return pixel;
}
