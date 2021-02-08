#ifndef EVOLUTION_IDE_CODESAMPLES_H
#define EVOLUTION_IDE_CODESAMPLES_H

class CodeSamples{

public:
    static constexpr auto Introduction_CPP_01
    = "/*\n"
      "C and C++ are low level languages, that means -> compiler\n"
      "will generate binary code instead of other languages like\n"
      "python, which is interpreted (text code is in run-time converted\n"
      "into assembly like code and run instruction by instruction), mostly\n"
      "there is no optimization or very small kind\n"
      "*/";

    static constexpr auto Main_CPP_02
    = "// main function represents entry point for every program. \n"
      "// code in main function is executed line by line, no jumps or other\n"
      "// things\n"
      "int main(){\n"
      "\t\n"
      "}";

    static constexpr auto Variables_CPP_03
    = "// every variable must have its type\n"
      "// common types: int(integer), bool(true/false or 1/0), char(characters), ...\n"
      "// syntax:\n"
      "int main(){\n"
      "\t// only declare\n"
      "\tint variable;\n"
      "\t// assign variable to 5\n"
      "\tvariable = 5;\n"
      "\t// now the variable is pushed on stack with value of 5\n"
      "\t// we can also reassign it\n"
      "\t variable = 10;\n"
      "\t// copy variable into another variable, ! must have same or convertible type\n"
      "\t// variable_2 has also value of 10\n"
      "\tint variable_2 = variable;\n"
      "}";

    static constexpr auto For_Loop_CPP_04
    = "// for loop is range based loop.\n"
      "// syntax:\n"
      "int main(){\n"
      "\t// only declare\n"
      "\tint var = 0;\n"
      "\t// i is changing its value every iteration cycle\n"
      "\t// i++ means that i will raise instead of i-- - decrease;  i++ is equivalent to i = i + 1\n"
      "\tfor(int i = 0; i <= 5; i++){\n"
      "\t\tvar = var + 1;\n"
      "\t\t// or simpler version: var++;\n"
      "\t}\n"
      "\t// at the end var will be increased 6 times(from 0 to 5) to value of 6\n"
      "\t// we can play with is and change range to < 5 --> var = 5;\n"
      "\t// < 5  means <= 4, so it will never reach 5, only 4\n"
      "\t// i = 1; i <= 5    --> var = 5\n"
      "}";




















};

#endif //EVOLUTION_IDE_CODESAMPLES_H
