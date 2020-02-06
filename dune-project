(lang dune 1.11)
(using fmt 1.2)
(name reglfw)
(generate_opam_files true)

(source (github revery-ui/reason-glfw))
(authors "Bryan Phelps")
(maintainers "Bryan Phelps <bryphe@outlook.com>")
(license MIT)

(package
  (name reglfw)
  (synopsis "Cross-platform Reason / OCaml bindings for GLFW")
  (description "Cross-platform Reason / OCaml bindings for GLFW")
  (depends
    (ocaml (>= 4.07))
    (dune (>= 1.11)))
  )
