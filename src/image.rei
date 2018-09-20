let stb_print_hello: unit => unit;

type t;

type successCallback = t => unit;
type failureCallback = string => unit;

let load: (string, successCallback, failureCallback) => unit;

let debug_print: t => unit;
