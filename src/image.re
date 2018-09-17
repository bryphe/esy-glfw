/* Image loading / handling */

external stb_print_hello: unit => unit = "caml_stb_image_print_hello";

type t;

external load: string => t = "caml_stb_image_load";
external debug_print: t => unit = "caml_stb_image_debug_print";
