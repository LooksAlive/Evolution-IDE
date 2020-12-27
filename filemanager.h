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

#include <QSettings>

class FileManager
{
public:
    FileManager();
    ~FileManager() = default;

    void getFilesRecursively(const QString &Project_RootDir);
    QString getFileExtension(const QString &filename); // set source_files
    void appendFileExtension();                        // on linux append extensions
    QString read(const QString &full_file_path);
    void write(const QString &full_file_path, const char *buffer);

    QString Project_Dir = QDir::homePath();   // QSettings, if new root dir is set up ... new files, new session

    QString current_full_filepath = "";  // /home/user/file.txt
    QString current_file_name = "";       // file.txt


private:

    QList<QString> all_files;       // filled with absolute paths
    QList<QString> source_files;    // later into cmdexecutor or cmake -> strip front known path or not, however


};

#endif // FILEMANAGER_H
