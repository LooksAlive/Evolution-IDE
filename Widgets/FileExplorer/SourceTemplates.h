#ifndef EVOLUTION_IDE_SOURCETEMPLATES_H
#define EVOLUTION_IDE_SOURCETEMPLATES_H


class SourceTemplates{

public:
    static constexpr auto H = "#ifndef HEADER\n"
                              "#define HEADER\n"
                              "\n\n\n"
                              "#endif // HEADER";

    static constexpr auto CPP = "int main(int argc, char *argv[]) {\n"
                                "}";


};







#endif //EVOLUTION_IDE_SOURCETEMPLATES_H
