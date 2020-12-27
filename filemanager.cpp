#include "filemanager.h"
#include <QDebug>

FileManager::FileManager()
{
    // QSettings settings;
}

// run on new thread
void FileManager::getFilesRecursively(const QString &Project_RootDir){

    Project_Dir = Project_RootDir;
    all_files.reserve(128);
    source_files.reserve(128);

    // FileExplorer_RootDir;
    QDirIterator directories(Project_RootDir, QDir::Files/*Dirs*/ | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    //QStringList dirs;
    while(directories.hasNext()){
        directories.next();
        all_files.push_back(directories.filePath());
        if(getFileExtension(directories.filePath()) == "cpp" |
                getFileExtension(directories.filePath()) == "h" |
                getFileExtension(directories.filePath()) == "hpp" |
                getFileExtension(directories.filePath()) == "c"|
                getFileExtension(directories.filePath()) == "cxx")
        {
            source_files.push_back(directories.filePath());
        }
    }
    // qDebug() << source_files;
}

QString FileManager::getFileExtension(const QString &filename){

    return QFileInfo(filename).suffix();
}

void FileManager::appendFileExtension(){

    if (QFileInfo(current_file_name).suffix().isEmpty()){
        current_file_name.append(".txt");
        current_full_filepath.append(".txt");
    }

    if (QFileInfo(current_full_filepath).suffix() == "bin" |
            QFileInfo(current_full_filepath).suffix() == "exe" |
            QFileInfo(current_full_filepath).suffix() == "elf"){
        // do something like open hexview, return bool or something
    }

    /*
    if (QFileInfo(filepath).suffix() == "pdf") {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(filepath);
        //QTextDocument.setPageSize(printer.pageRect().size());
        ((PlainTextEdit*)Tabs->currentWidget())->document()->print(&printer);
    }
    */

}

QString FileManager::read(const QString &full_file_path){

    QString buffer;

    // strip last / from file path
    current_full_filepath = full_file_path;
    // qDebug() << current_full_filepath;
    QString filename = full_file_path.section("/",-1,-1);
    current_file_name = filename;
    // qDebug() << current_file_name;

    // appendFileExtension();



    QFile file(current_full_filepath);

    if (file.open(QIODevice::ReadOnly)){
        buffer = file.readAll();
    }

    else {
        (new QErrorMessage())->showMessage("Cannot open file!");
        return "";
    }

    file.close();

    return buffer;
}
void FileManager::write(const QString &full_file_path, const char *buffer){

    current_full_filepath = full_file_path;
    // qDebug() << current_full_filepath;
    QString filename = full_file_path.section("/",-1,-1);
    current_file_name = filename;

    // appendFileExtension();


    QFile file(full_file_path);
    if (file.open(QIODevice::WriteOnly)){
        file.write(buffer);
    }

    else {
        (new QErrorMessage())->showMessage("Cannot save file!");
        return;
    }

    file.close();
}
