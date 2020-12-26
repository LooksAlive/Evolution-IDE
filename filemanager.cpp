#include "filemanager.h"

FileManager::FileManager()
{
    // QSettings settings;
}


void FileManager::getFilesRecursively(){
    // FileExplorer_RootDir;
}
void FileManager::determineExtension(const QString &filename){
    QString file_extension = QFileInfo(filename).suffix();
}
QString FileManager::read(const QString &full_file_path){

    QString buffer;
    // take only name, strip path
    QString filename = full_file_path.section("/",-1,-1);
    QFile file(filename);
    buffer = file.readAll();
    file.close();

    return buffer;
}
void FileManager::write(const QString &full_file_path, const char *buffer){
    QFile file(full_file_path);
    if (file.open(QIODevice::WriteOnly)){
        file.write(buffer);
    }
    file.close();
}
