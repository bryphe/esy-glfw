// Provides: caml_stb_image_load
function caml_stb_image_load(imgName, onSuccess, onFailure) {
    var image = new joo_global_object.Image();
    console.log("stubby");
    image.onload = function () {
        console.log("laoded!");
        onSuccess(image);
    };

    image.onerror = function (err) {
        console.log("failed to load");
        onFailure(caml_js_to_string("failed to load image: " + err.toString()));
    };

    image.src = "test.jpg";
}

// Provides: caml_stb_image_debug_print
function caml_stb_image_debug_print(img) {
    console.log("Image width: " + img.width + " Image height: " + img.height);
}
