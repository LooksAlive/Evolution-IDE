#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QString>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>
#include <QDomDocument>
#include <QHash>
#include <QStringList>

/*

Only temporary highliter taken from Qt examples,
intill set up clang to choose what to highlight

*/

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit Highlighter(const QString &filename, QObject *parent = 0);

    void highlightBlock(const QString &text) override;
    bool setExtension(const QString &_filename);
private:
    QString xml_filename;
    QRegExp current_extension;
    struct HighlightFormat{
        QRegExp pattern;
        QTextCharFormat format;
    };
    struct HighlightLang{
        QVector <HighlightFormat> for_keywords;
        QRegExp comment_beg_exp;
        QRegExp comment_end_exp;
        QTextCharFormat single_comment;
        QTextCharFormat multi_comment;
        QTextCharFormat keyword_format;
        QTextCharFormat classname_format;
        QTextCharFormat function_format;
        QTextCharFormat quotation_format;
        QTextCharFormat preproc_format;
    };
    /*
     // recognizing file extension and chose language word xml file to do thing
     QRegExp file_extension;
     QVector <HighlightFormat> for_keywords;
     QRegExp comment_beg_exp;
     QRegExp comment_end_exp;
     QTextCharFormat single_comment;
     QTextCharFormat multi_comment;
     QTextCharFormat keyword_format;
     QTextCharFormat classname_format;
     QTextCharFormat function_format;
     QTextCharFormat quotation_format;
     QTextCharFormat preproc_format;

     */

    QHash<QRegExp, HighlightLang> langs;

    static constexpr auto C_CPP_Types = {
            "int",
            "uint",
            "char",
            "unsigned",
            "signed",
            "short",
            "long",
            "float",
            "double",
            "wchar_t",
            "size_t"
    };

    static constexpr auto cmakeCommands = {
            "add_compile_options",
            "add_custom_command",
            "add_custom_target",
            "add_definitions",
            "add_dependencies",
            "add_executable",
            "add_library",
            "add_subdirectory",
            "add_test",
            "aux_source_directory",
            "break",
            "build_command",
            "cmake_host_system_information",
            "cmake_minimum_required",
            "cmake_policy",
            "configure_file",
            "create_test_sourcelist",
            "define_property",
            "elseif",
            "else",
            "enable_language",
            "enable_testing",
            "endforeach",
            "endfunction",
            "endif",
            "endmacro",
            "endwhile",
            "execute_process",
            "export",
            "file",
            "find_file",
            "find_library",
            "find_package",
            "find_path",
            "find_program",
            "fltk_wrap_ui",
            "foreach",
            "function",
            "get_cmake_property",
            "get_directory_property",
            "get_filename_component",
            "get_property",
            "get_source_file_property",
            "get_target_property",
            "get_test_property",
            "if",
            "include_directories",
            "include_external_msproject",
            "include_regular_expression",
            "include",
            "install",
            "link_directories",
            "list",
            "load_cache",
            "load_command",
            "macro",
            "mark_as_advanced",
            "math",
            "message",
            "option",
            "project",
            "qt_wrap_cpp",
            "qt_wrap_ui",
            "remove_definitions",
            "return",
            "separate_arguments",
            "set_directory_properties",
            "set_property",
            "set", /* main command at least for me :) */
            "set_source_files_properties",
            "set_target_properties",
            "set_tests_properties",
            "site_name",
            "source_group",
            "string",
            "target_compile_definitions",
            "target_compile_options",
            "target_include_directories",
            "target_link_libraries",
            "try_compile",
            "try_run",
            "unset",
            "variable_watch",
            "while",

            /* Deprecated */

            "build_name",
            "exec_program",
            "export_library_dependencies",
            "install_files",
            "install_programs",
            "install_targets",
            "link_libraries",
            "make_directory",
            "output_required_files",
            "remove",
            "subdir_depends",
            "subdirs",
            "use_mangled_mesa",
            "utility_source",
            "variable_requires",
            "write_file",

            /* CTest */

            "ctest_build",
            "ctest_configure",
            "ctest_coverage",
            "ctest_empty_binary_directory",
            "ctest_memcheck",
            "ctest_read_custom_files",
            "ctest_run_script",
            "ctest_sleep",
            "ctest_start",
            "ctest_submit",
            "ctest_test",
            "ctest_update",
            "ctest_upload",

            /* Other */
            "PRIVATE",
            "SHARED"
    };

    // TODO: regular expression for set(firstArgument x x1, x2, ...) and also ${variable} and curly braces

    static constexpr auto pythonTypes = {
            "int",
            "float",
            "double",
            "bool",
            "str",
            "byte"
    };


};

#endif // HIGHLIGHTER_H
