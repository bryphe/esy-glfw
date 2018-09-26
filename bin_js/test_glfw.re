open EsyGlfwExamples;

type testType =
| A
| B(string)

let derp = B("hi");

external print_hello: testType => unit = "caml_print_hello";

let _ = print_hello(derp);

try%lwt (Basic.run()) {
| Js.Error(s) => Lwt.return(print_endline("ERROR: " ++ Js.string_of_error(s)));
| _  => Lwt.return(print_endline("Unknown error"));
}
