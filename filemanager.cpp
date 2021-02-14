#include <QDebug>
#include <QSettings>

#include "Clang/ClangBridge.h"      // add source files to index
#include "filemanager.h"

FileDirManager::FileDirManager()
{
    // set stored project dir , if is empty leave default home dir
    QSettings settings("Evolution");
    Project_Dir = settings.value("Evolution/Project_Root_Dir").toString();
    source_files = settings.value("Evolution/sources").toStringList();
    source_files_names = settings.value("Evolution/sources_names").toStringList();
    executable_file_path = settings.value("Evolution/executable_path").toString();
}

// run on new thread
void FileDirManager::getFilesRecursively(const QString &Project_RootDir){

    QSettings settings("Evolution");
    settings.setValue("Evolution/Project_Root_Dir", Project_RootDir);
    Project_Dir = Project_RootDir;
    other_files.reserve(25);
    source_files.reserve(25);
    source_files_names.reserve(25);

    // FileExplorer_RootDir;
    QDirIterator directories(Project_RootDir, QDir::Files/*Dirs*/ | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    //QStringList dirs;
    while(directories.hasNext()){
        directories.next();

        if(directories.fileName() == "CMakeLists.txt"){
            project_cmake_file_exists = true;
            settings.setValue("Evolution/CMakeLists.txt", directories.fileName().remove(0, 2));
        }
        if(directories.fileInfo().dir().dirName() == "cmake-build"){
            if(directories.fileInfo().isExecutable()){ // binary file
                executable_file_path = directories.filePath().remove(0, 2); // here it takes all executables in cmake dir
                settings.setValue("Evolution/executable_path", executable_file_path);
            }
            else{
                break;
            }
            /*
            if(directories.fileInfo().isExecutable()){ // binary file
                executable_file_path = directories.filePath();
                settings.setValue("Evolution/executable_path", executable_file_path);
                break;
            }
            else{
                break; // in build dir -> do not want generated files from here, only executable
            }
            */
        }


        if(getFileExtension(directories.filePath()) == "cpp" |
                getFileExtension(directories.filePath()) == "h" |
                getFileExtension(directories.filePath()) == "hpp" |
                getFileExtension(directories.filePath()) == "c"|
                getFileExtension(directories.filePath()) == "cxx")
        {
            // remove // in ///somedir/simefile.cxx
            source_files.push_back(directories.filePath().remove(0, 2));
            source_files_names.push_back(directories.fileName());
        }
        else{
            other_files.push_back(directories.filePath());
        }
        if(directories.fileInfo().isExecutable()){ // binary file
            qDebug() << directories.filePath();
            executable_file_path = directories.filePath(); // here it takes all executables in cmake dir
            settings.setValue("Evolution/executable_path", executable_file_path);
        }
    }
    settings.setValue("Evolution/other_files", other_files);
    settings.setValue("Evolution/sources", source_files);
    settings.setValue("Evolution/sources_names", source_files_names);
}

QString FileDirManager::getFileExtension(const QString &filename){

    return QFileInfo(filename).suffix();
}

void FileDirManager::appendFileExtension(){

    if (QFileInfo(current_file_name).suffix().isEmpty()){
        current_file_name.append(".txt");
        current_full_filepath.append(".txt");
    }

    if (QFileInfo(current_full_filepath).suffix() == "bin" ||
            QFileInfo(current_full_filepath).suffix() == "exe" ||
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

QString FileDirManager::simple_read(const QString &full_file_path){
    QString buffer;

    current_full_filepath = full_file_path;
    QFile file(current_full_filepath);

    if (file.open(QIODevice::ReadOnly)){
        buffer = file.readAll();
    }
    file.close();

    return buffer;
}

QString FileDirManager::read(const QString &full_file_path){

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
        (new QErrorMessage())->showMessage("Cannot open file " + current_full_filepath);
        return "";
    }

    file.close();

    return buffer;
}
void FileDirManager::write(const QString &full_file_path, const char *buffer){

    current_full_filepath = full_file_path;
    // qDebug() << current_full_filepath;
    QString filename = full_file_path.section("/",-1,-1);
    current_file_name = filename;

    // appendFileExtension();

    QFile file(current_full_filepath);
    if (file.open(QIODevice::WriteOnly)){
        file.write(buffer);
    }

    else {
        (new QErrorMessage())->showMessage("Cannot save file " + current_full_filepath);
        return;
    }

    file.close();
}
// later if files will change change also in registry, everywhere
void FileDirManager::rename(const QString &old_path, const QString &new_name) {

    QFileInfo info(old_path);
    if(info.isDir()){
        // dir
        QDir dir(old_path);
        dir.rename(dir.dirName(), new_name); // why this does not work ???
    }
    else{
        // file
        QFile file(old_path);
        // file.rename(new_name);  // why this does not work ??? going old fashion way then ...
        QString buffer = read(old_path);
        // dir in which is file located
        QString new_path = info.dir().absolutePath();
        file.remove();
        file.close();

        write(new_path + "/" + new_name, buffer.toStdString().c_str());
    }
}

void FileDirManager::duplicate(const QString &file_path) {
    QString buffer = read(file_path);
    QFileInfo finfo(file_path);
    QString path = finfo.absoluteDir().path();
    QString name = finfo.fileName();
    write(path + "/" +  name + "_copy", buffer.toStdString().c_str());
}

void FileDirManager::move(const QString &old_path, const QString &new_path) {
    QFileInfo info(old_path);
    if(info.isFile()){
        QFile file(old_path);
        file.copy(new_path);
        file.close();

        QFile file2(old_path);
        file2.remove();
        file2.close();
    }
    // Directory
    else{
        return;  // for now , later copy all files recursively ...
    }

}

void FileDirManager::addFileToIndex(const QString &filepath) {
    //clang->addDocument();
}

void FileDirManager::removeFileFromIndex(const QString &filepath) {
    //clang->removeDocument(filepath.toLatin1().data());
}

