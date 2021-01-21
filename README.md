# Evolution-IDE

link:  https://github.com/adamko222/Evolution-IDE

Integrated Development Environment

- my IDE is simple, fast, multiplatform, based on Qt & all classes are in 
  separate folders(widgets) and can be reused without extern 
  requirement(headers, sources, etc.)

Features:

* Dark and White theme

Text:

* line highlight
* CTRL+SHIFT+C                  -> toggle comment
* tab selected text
* escape function
* CTRL+SHIFT+UP/DOWN            -> moving selected between lines
* CTRL+U                        -> to upper
* CTRL+SHIFT+U                  -> to lower
* HOME/END                      -> top bottom move
* cursor remembers x position when up/down arrow moves
* line number click selects whole line
* CTRL+Wheel                    -> increase, decrease size (CTRL +-  also)
* CTRL+UP/DOWN                  -> like srcolling action
* CTRL+LEFT/RIGHT               -> moving to start of next words


Overal:

* ctrl + wheel -> amplify font
* syntax highligting
* Terminal output
* different text Formats converter, small widget
* settings window
* registry settings (.ini format)


* hex view                      -> from active file
* cmake generator               -> cmake-build dir, options in settings
* lldb debugger                 -> stepping, manual commands, frame views
* gdbgui possible to launch

Future:

* clang indexer, completer, refractoring
* clang fix it
* clang-tidy
* clang-move
* binary information view (llvm implemented tools)
* git version control
* use (CINT) / Cling C++ interpreter
