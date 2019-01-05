type t;
type pixelBuffer;

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

let create:
  (~width:int, ~height:int, ~numChannels:int, ~channelSize:int) => t;
let destroy: t => unit;
let getBuffer: t => pixelBuffer;
let save: (t, string) => unit;
