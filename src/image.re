/* Image loading / handling */

external stb_print_hello: unit => unit = "caml_stb_image_print_hello";

type image;
type t = image;

exception ImageLoadException(string);

type successCallback = image => unit;
type failureCallback = string => unit;

external raw_load: (string, successCallback, failureCallback) => unit = "caml_stb_image_load";
external debug_print: image => unit = "caml_stb_image_debug_print";

let load = (imgName) => {
    let (promise, resolver) = Lwt.task();

    let success = (img) => Lwt.wakeup_later(resolver, img);
    let failure = (message) => Lwt.wakeup_later_exn(resolver, ImageLoadException(message));

    raw_load(imgName, success, failure);
    promise
}
