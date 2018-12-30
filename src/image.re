/* Image loading / handling */

type t;
type pixelBuffer;

exception ImageLoadException(string);

type successCallback = t => unit;
type failureCallback = string => unit;

external raw_load: (string, successCallback, failureCallback) => unit =
  "caml_stb_image_load";
external debug_print: t => unit = "caml_stb_image_debug_print";

external fromColor: (int, int, int, int) => t = "caml_stb_image_from_color";

type dimensions = {
  width: int,
  height: int,
};

external getDimensions: t => dimensions = "caml_stb_image_dimensions";

let load = imgName => {
  let (promise, resolver) = Lwt.task();

  let success = img => Lwt.wakeup_later(resolver, img);
  let failure = message =>
    Lwt.wakeup_later_exn(resolver, ImageLoadException(message));

  raw_load(imgName, success, failure);
  promise;
};

external create: (~width:int, ~height:int, ~numChannels:int,
                  ~channelSize:int) => t = "caml_createImage";
external destroy: t => unit = "caml_destroyImage";
external getBuffer: t => pixelBuffer = "caml_getImageBuffer";
external save: (t, string) => unit = "caml_saveImage";
