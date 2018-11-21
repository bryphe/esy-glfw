type t;

exception ImageLoadException(string);

type successCallback = t => unit;
type failureCallback = string => unit;

type dimensions = {
    width: int,
    height: int,
};

let load: (string) => Lwt.t(t);
let fromColor: (int, int, int, int) => t;
let getDimensions: t => dimensions;
let debug_print: t => unit;
