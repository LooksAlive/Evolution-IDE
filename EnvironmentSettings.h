#ifndef ENVIRONMENTSETTINGS_H
#define ENVIRONMENTSETTINGS_H

#include <QSettings>
#include <QStringList>

// tabs
constexpr auto NEW_TAB_NAME = "untitled";



// paths
constexpr auto FILE_DIALOG_PATH = "/home";



// fonts, margins,  positions
constexpr auto FONT_FAMILY = "Console";
constexpr auto FONT_SIZE = 16;


//static QSettings settings("Evolution");

// C++ template examples to insert into code: check clangd
/*
main();
templates,

void <name>()
{

}
*/


const QStringList CommentTags = {
        "TODO",    /* consider TODO1,2,3 level to select which is more important (similar with FIXME; or ) */
        "FIXME",
        "NOTE",
        "LONG",
        "CHECK",
        "USELESS",
        "SPEEDUP",
        "EXPAND",
        "SHRINK",
        "REARRANGE",
        "DEPRECATED"
};

const QStringList C_CPP_Types = {
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

const QStringList C_CPP_Statements = {
        "if",
        "ifndef",
        "endif",
        "pragma",
        "else",
        "template",
        "for",
        "while",
        "const",
        "constexpr",
        "auto",
        "return"
};

const QStringList C_CPP_Templates = {
        "/*",
        "for",
        "while",
        "if",
        "ifndef",
        "do while",
        "#include <>",
        "#include \"\""
};

const QStringList cmakeCommands = {
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

// TODO: regular expression for set(firstArgument x x1, x2, ...) and also ${variable} and curly braces (highlighter)

const QStringList pythonTypes = {
        "int",
        "float",
        "double",
        "bool",
        "str",
        "byte",
        "import",
        "return",

        "for",
        "while",
        "if",
        "else",
};


#endif // ENVIRONMENTSETTINGS_H
