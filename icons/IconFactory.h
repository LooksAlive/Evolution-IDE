/*
  * this class manages icons, images, etc.
  * if you have any idea how to initialize static and add prefix, write me, i tried,  == empty variable, with QIcon
*/

#ifndef ICONFACTORY_H
#define ICONFACTORY_H

#include <QString>
#include <QIcon>
#include <iostream>
#include <string>


class IconFactory
{
public:
    IconFactory();
    ~IconFactory() = default;

    static constexpr auto prefix = "/home/adam/Desktop/sources/Evolution-IDE/icons/";

    // Editor Icon
    static constexpr auto Evolution = "/home/adam/Desktop/sources/Evolution-IDE/icons/editor.png";

    // TOOL BARS
    static constexpr auto EditorView = "/home/adam/Desktop/sources/Evolution-IDE/icons/SourceView.png";
    static constexpr auto HexView = "/home/adam/Desktop/sources/Evolution-IDE/icons/HexView.png";
    static constexpr auto NodeView = "/home/adam/Desktop/sources/Evolution-IDE/icons/NodeView.png";
    static constexpr auto DebuggerView = "/home/adam/Desktop/sources/Evolution-IDE/icons/DebuggerView.svg";
    static constexpr auto BinaryView = "/home/adam/Desktop/sources/Evolution-IDE/icons/BinaryView.png";

    static constexpr auto OpenFile = "/home/adam/Desktop/sources/Evolution-IDE/icons/open_file.svg";
    static constexpr auto NewFile = "/home/adam/Desktop/sources/Evolution-IDE/icons/new_file.svg";
    static constexpr auto SaveFile = "/home/adam/Desktop/sources/Evolution-IDE/icons/save_file.svg";
    static constexpr auto SaveAllFiles = "/home/adam/Desktop/sources/Evolution-IDE/icons/save_all_files.png";

    static constexpr auto Build = "/home/adam/Desktop/sources/Evolution-IDE/icons/Build.png";
    static constexpr auto Run = "/home/adam/Desktop/sources/Evolution-IDE/icons/run.svg";
    static constexpr auto Stop = "/home/adam/Desktop/sources/Evolution-IDE/icons/stop.svg";
    static constexpr auto ShutDown = "/home/adam/Desktop/sources/Evolution-IDE/icons/shutdown.svg";
    static constexpr auto FullScreen = "/home/adam/Desktop/sources/Evolution-IDE/icons/fullscreen.svg";

    static constexpr auto Undo = "/home/adam/Desktop/sources/Evolution-IDE/icons/undo.svg";
    static constexpr auto Redo = "/home/adam/Desktop/sources/Evolution-IDE/icons/redo.svg";

    static constexpr auto Settings = "/home/adam/Desktop/sources/Evolution-IDE/icons/settings.svg";

    // TEXT
    static constexpr auto Copy = "/home/adam/Desktop/sources/Evolution-IDE/icons/copy.svg";
    static constexpr auto Paste = "/home/adam/Desktop/sources/Evolution-IDE/icons/paste.svg";
    static constexpr auto Cut = "/home/adam/Desktop/sources/Evolution-IDE/icons/cut.svg";
    static constexpr auto SelectAll = "/home/adam/Desktop/sources/Evolution-IDE/icons/select_all.svg";
    static constexpr auto Collapse = "/home/adam/Desktop/sources/Evolution-IDE/icons/collapse.svg";
    static constexpr auto Expand = "/home/adam/Desktop/sources/Evolution-IDE/icons/expand.svg";
    static constexpr auto BreakPoint = "/home/adam/Desktop/sources/Evolution-IDE/icons/BreakPoint.png";
    static constexpr auto Comment = "/home/adam/Desktop/sources/Evolution-IDE/icons/comment.png";

    // EXPLORERS
    static constexpr auto Plus = "/home/adam/Desktop/sources/Evolution-IDE/icons/plus.svg";
    static constexpr auto Minus = "";     // TODO: find some
    static constexpr auto DefaultDir = "/home/adam/Desktop/sources/Evolution-IDE/icons/default_dir.svg";
    static constexpr auto Back = "/home/adam/Desktop/sources/Evolution-IDE/icons/back.svg";
    static constexpr auto Search = "/home/adam/Desktop/sources/Evolution-IDE/icons/search.svg";
    static constexpr auto NewDirectory = "/home/adam/Desktop/sources/Evolution-IDE/icons/new_directory.svg";
    static constexpr auto Remove = "/home/adam/Desktop/sources/Evolution-IDE/icons/remove2.svg";
    static constexpr auto Done = "/home/adam/Desktop/sources/Evolution-IDE/icons/done.svg";
    static constexpr auto ScrollUp = "/home/adam/Desktop/sources/Evolution-IDE/icons/scroll_up.svg";
    static constexpr auto ScrollDown = "/home/adam/Desktop/sources/Evolution-IDE/icons/scroll_down.svg";
    static constexpr auto TrashBin = "/home/adam/Desktop/sources/Evolution-IDE/icons/trash_bin.svg";

    // DEBUGGER
    static constexpr auto StartDebug = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/StartDebug.png";
    static constexpr auto StopDebug = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/StopDebug.png"; // square
    static constexpr auto Resume = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/Resume.png";
    static constexpr auto RunToCursor = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/RunToCursor.png";
    static constexpr auto NextLine = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/NextLine.png";
    static constexpr auto StepInto = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/GetIntoFunction.png";
    static constexpr auto StepInstruction = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/NextInstruction.png";   // StepInstruction
    static constexpr auto GetOutOfFunction = "/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/GetOutOfFunction.png";

};

#endif // ICONFACTORY_H
