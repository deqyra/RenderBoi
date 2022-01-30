Renderboi was written with a few things in mind, regarding architecture as well as code style:
* **Keep things simple.** If it makes sense, go for it. If it's convoluted, break it down. Even if it does make sense as is. Complex is better than complicated.
* **Explicit over implicit.** Verbosity is (almost) never a problem. `using` directives for whole namespaces are frowned upon, especially in the global scope.
* Naming stuff:
  * Make it clear. Explicit over implicit.
  * Local variables:
    * `regularVariable`
    * `ConstVariable`
    * `_staticVariable`
  * Class attributes (public, both static and non-static):
    * `regularVariable`
    * `ConstVariable`
  * Class attributes (protected or private, both static and non-static):
    * Same as above, but with a leading `_`: `_regularVariable`, etc.
  * Class methods (public):
    * `myNonStaticMethod`
    * `MyStaticMethod`
  * Class methods (protected or private):
    * Same as above, but with a leading `_`: `_MyStaticMethod`, etc.
* Provide Doxygen readable doc above each function *declaration* (header, not source).
* Proposed maximum line width: 80 chars for docs/comments, 120 chars for code.

These guidelines were merely put up here for the sake of having them around.
I personally do my best to stick to them, but I won't go out of my way to enforce them.

Quick checklist for polishing things before committing:
* Docstrings are complete with up-to-date information
* Docstrings are well formated
* Comments are present where relevant
* To-do list is up-to-date
* Files are named in a coherent manner in the correct directories
* Header include guards match file names
* CMakeLists.txt is up-to-date **and building succeeds** (and runs too, preferably)
* Includes are properly written, grouped, and sorted
* Enforce const-correcteness on both variables (all kinds) and member functions
* Member ordering in class declarations makes sense and is reproduced in source file
* Thematic sections of code are surrounded by #pragma region indicators. Example of said sections:
  * Declaration of type aliases
  * Declaration of member variables
  * Declaration/definition of getters and/or setters
  * Declaration/definition of overriding member functions
