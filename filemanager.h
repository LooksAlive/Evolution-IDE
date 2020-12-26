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

#include <QSettings>

class FileManager
{
public:
    FileManager();

    void getFilesRecursively();
    void determineExtension(const QString &filename); // set source_files
    QString read(const QString &full_file_path);
    void write(const QString &full_file_path, const char *buffer);

    QString FileExplorer_RootDir = QDir::homePath();   // QSettings


private:

    QList<QString> all_files;
    QList<QString> source_files;


};

#endif // FILEMANAGER_H
