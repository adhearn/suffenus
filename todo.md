== Compiler Features
* Finish adding symbol table
* Add type checking pass
* PLan the connection between the front end and the back end

== Cleanup
* Standardize naming convention (e.g. `Block_free` vs `free_Program`). I'm leaning toward GLib style `<datatype>_<functionality>`, instead of my previous `<functionality>_<datatype>`, as well as using CamelCase for types and snake_case for functions. I don't really care, but I need to standardize or I'll drive myself crazy.

* Figure out why bison-mode formats my parser files so weirdly and fix it
