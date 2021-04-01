# Evolution-IDE

link:  https://github.com/adamko222/Evolution-IDE

Integrated Development Environment

- my IDE is simple, fast, multiplatform, based on Qt & all classes are in 
  separate folders(widgets) and can be reused without extern 
  requirement(headers, sources, etc.)
  
Contribution:
- you may notices folders include(.h files), libs(.so, .a libraries) for build
- NOTE: these are manually built clang, llvm libs and they are compressed, so if you want to build... decompress
 them in same location, similarly if you heading to work with them, you can download already built .a libs from here
- if any problem with commit, try "git gc"


Requirements:
* Qt5+
* QCharts (I had to download this on linux with "sudo apt-get install libqt5chars5-dev", Windows: download through client)
* clang, llvm, git, hunspell (linking with libs folder already, therefore that huge project :)  )

Features:

* Dark and White theme

Text:

* line highlight
* CTRL+SHIFT+C                  -> toggle comment
* CTRL+F                        -> search
* tab selected text
* escape function
* CTRL+SHIFT+UP/DOWN            -> moving selected between lines
* CTRL+U                        -> to upper
* CTRL+SHIFT+U                  -> to lower
* HOME/END                      -> top bottom move
* cursor remembers x position when up/down arrow moves
* line number click selects whole line
* CTRL+Wheel                    -> increase, decrease size (CTRL +-  also)
* CTRL+UP/DOWN                  -> like scrolling  action
* CTRL+LEFT/RIGHT               -> moving to start of next words
* automatic insertions like  () [] {} <> "" ''

* spell checking

Overall:

* ctrl + wheel -> amplify font
* syntax highlighting
* Terminal output
* different text Formats converter, small widget
* settings window
* registry settings (.ini format)


* hex view                      -> from active file
* cmake generator               -> cmake-build dir, options in settings
* lldb debugger                 -> stepping, manual commands, frame views
* gdbgui possible to launch

File Explorer:

* creating, files, dirs
* removing, renaming, copying(different things from tree)
* custom menu, tool bars

Debugger:

* frame view
* variable view
* manual console input

Future:

* clang indexer, completer, refactoring
* clang fix it
* clang-tidy
* clang-move
* binary information view (llvm implemented tools)
* git version control
* use (CINT) / Cling C++ interpreter
