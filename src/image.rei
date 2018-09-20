
let stb_print_hello: unit => unit;

type image;
type t = image;

exception ImageLoadException(string);

type successCallback = image => unit;
type failureCallback = string => unit;

let load: (string) => Lwt.t(image);

let debug_print: image => unit;
