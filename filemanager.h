#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/*

  this class take care of reading, writing, filepaths, if exist, writeable, readable ...
  simply all stuffs around files and dirs ,  to reduce repetitive code

*/

#include <QString>
#include <QList>
#include <QDir>
#include <QFile>
#include <QErrorMessage>
#include <QDebug>
#include <QDirIterator>
#include <QStringList>

class ClangBridge;

class FileDirManager
{
public:
    FileDirManager();
    ~FileDirManager() = default;

    // source files adding, removing from index
    ClangBridge *clang;
    void setClang(ClangBridge *clang_bridge) {clang = clang_bridge;}
    // registry settings and other
    void addFileToIndex(const QString &filepath);
    void removeFileFromIndex(const QString &filepath);

    void getFilesRecursively(const QString &Project_RootDir);
    QString getFileExtension(const QString &filename);// set source_files
    void appendFileExtension();                       // on linux append extensions
    QString simple_read(const QString &full_file_path);
    QString read(const QString &full_file_path);
    void write(const QString &full_file_path, const char *buffer);
    void rename(const QString &old_path, const QString &new_name);
    void duplicate(const QString &file_path);
    void move(const QString &old_path, const QString &new_path);

    QString Project_Dir = QDir::homePath();// QSettings, if new root dir is set up ... new files, new session

    QString current_full_filepath;// /home/user/file.txt
    QString current_file_name;    // file.txt
    QString executable_file_path; // executable for debugger
    QString clang_format_path;
    QString clang_tidy_path;
    bool project_cmake_file_exists = false;

    QStringList other_files;       // filled with absolute paths
    QStringList source_files;      // whole path
    QStringList source_files_names;// only file name

private:
};

#endif // FILEMANAGER_H
