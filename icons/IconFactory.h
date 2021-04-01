/*
  * this class manages icons, images, etc.
  * if you have any idea how to initialize static and add prefix, write me, i tried,  == empty variable, with QIcon
  *
  * Qt requires also cpp file for generating moc files...
*/

#ifndef ICONFACTORY_H
#define ICONFACTORY_H

#include <QApplication>
#include <QIcon>
#include <QString>
#include <iostream>
#include <string>


class IconFactory
{
public:
    IconFactory() = default;
    ~IconFactory() = default;

    // QApplication::applicationDirPath();      NOTE: this returns dir for executable, we need source dir soo configure your own
    // into icons/  folder
    // CHANGE THIS:
    static const QString prefix;

    // Editor Icon
    static const QString Evolution;

    // TOOL BARS
    static const QString EditorView;
    static const QString HexView;
    static const QString NodeView;
    static const QString DebuggerView;
    static const QString BinaryView;

    static const QString OpenFile;
    static const QString NewFile;
    static const QString SaveFile;
    static const QString SaveAllFiles;

    static const QString Build;
    static const QString Run;
    static const QString Rerun;
    static const QString Stop;
    static const QString ShutDown;
    static const QString FullScreen;
    static const QString Push ;
    static const QString Pull;
    static const QString Merge;
    static const QString Commit;

    static const QString Undo;
    static const QString Redo ;

    static const QString Settings;

    static const QString Color;
    static const QString Error;
    static const QString Warning;


    // TEXT
    static const QString Copy;
    static const QString Paste;
    static const QString Cut;
    static const QString SelectAll;
    static const QString Collapse;
    static const QString Expand;
    static const QString BreakPoint;
    static const QString Comment;

    // EXPLORERS
    static const QString Plus;
    static const QString Minus;
    static const QString DefaultDir;
    static const QString Back;
    static const QString Search;
    static const QString Replace;
    static const QString NewDirectory;
    static const QString Remove;
    static const QString RemoveAll2;
    static const QString AllFiles;
    static const QString CurrentFile;
    static const QString Done;
    static const QString ScrollUp;
    static const QString ScrollDown;
    static const QString TrashBin;

    // DEBUGGER
    static const QString StartDebug;
    static const QString StopDebug;// square
    static const QString Resume;
    static const QString RunToCursor;
    static const QString NextLine;
    static const QString StepInto;
    static const QString StepInstruction;// StepInstruction
    static const QString GetOutOfFunction;
    static const QString AddWatch;
    static const QString RemoveWatch;
    static const QString ModifyWatch;
    static const QString Mute;
    static const QString RemoveAll;
};


#endif // ICONFACTORY_H
