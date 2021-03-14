#ifndef EVOLUTION_IDE_CODESAMPLES_H
#define EVOLUTION_IDE_CODESAMPLES_H

#include <QCoreApplication>

#include "filemanager.h"

class CodeSamples {

public:

    CodeSamples();

    ~CodeSamples() = default;

    FileDirManager file;

    QString s;

    const QString AppDir = QCoreApplication::applicationDirPath();

    void setData();

    static constexpr auto xx = "#include <iostream>\n"
                               "using namespace std;\n"
                               "class DemoClass {\n"
                               "public:\n"
                               "    int demoFunction(int i) {\n"
                               "        return i;\n"
                               "    }\n"
                               "    double demoFunction(double d) {\n"
                               "        return d;\n"
                               "    }\n"
                               "};\n"
                               "int main(void) {\n"
                               "    DemoClass obj;\n"
                               "    cout<<obj.demoFunction(100)<<endl;\n"
                               "    cout<<obj.demoFunction(5005.516);\n"
                               "   return 0;\n"
                               "}";

    static constexpr auto Introduction_CPP_01
            = "/*\n"
              "C++ language is a direct descendant of C programming language with additional features such as type checking, object oriented programming, exception handling etc. You can call it a “better C”. It was developed by Bjarne Stroustrup.\n"
              "C++ is a general purpose language language, when I say general purpose it simply means that it is designed to be used for developing applications in a wide variety of domains."
              "*/";

    static constexpr auto Main_CPP_02
            = "In this guide we will write and understand the first program in C++ programming. We are writing a simple C++ program that prints “Hello World!” message. Lets see the program first and then we will discuss each and every part of it in detail.\n"
              "\n"
              "\n"
              "/*\n"
              "* Multiple line\n"
              "* comment\n"
              "*/\n"
              "#include<iostream>\n"
              "\n"
              "//Single line comment\n"
              "using namespace std;\n"
              "\n"
              "//This is where the execution of program begins\n"
              "int main()\n"
              "{\n"
              "   // displays Hello World! on screen\n"
              "   cout<<\"Hello World!\";\n"
              "\n"
              "   return 0;\n"
              "}\n"
              "\n"
              "Output:\n"
              "\n"
              "Hello World!\n"
              "\n"
              "/*\n"
              "Let’s discuss each and every part of the above program.\n"
              "\n"
              "1. Comments – You can see two types of comments in the above program\n"
              "\n"
              "// This is a single line comment\n"
              "/* This is a multiple line comment\n"
              "* suitable for long comments\n"
              "*/\n"
              "\n"
              "Comments as the names suggests are just a text written by programmer during code development. Comment doesn’t affect your program logic in any way, you can write whatever you want in comments but it should be related to the code and have some meaning so that when someone else look into your code, the person should understand what you did in the code by just reading your comment.\n"
              "\n"
              "For example:\n"
              "\n"
              "/* This function adds two integer numbers \n"
              " * and returns the result as an integer value\n"
              " */\n"
              "int sum(int num1, int num2) {\n"
              "   return num1+num2;\n"
              "}\n"
              "\n"
              "/*"
              "Now if someone reads my comment he or she can understand what I did there just by reading my comment. This improves readability of your code and when you are working on a project with your team mates, this becomes essential aspect.\n"
              "\n"
              "2. #include<iostream> – This statements tells the compiler to include iostream file. This file contains pre defined input/output functions that we can use in our program.\n"
              "\n"
              "3. using namespace std; – A namespace is like a region, where we have functions, variables etc and their scope is limited to that particular region. Here std is a namespace name, this tells the compiler to look into that particular region for all the variables, functions, etc. I will not discuss this in detail here as it may confuse you. I have covered this topic in a separate tutorial with examples. Just follow the tutorial in the given sequence and you would be fine.\n"
              "\n"
              "4. int main() – As the name suggests this is the main function of our program and the execution of program begins with this function, the int here is the return type which indicates to the compiler that this function will return a integer value. That is the main reason we have a return 0 statement at the end of main function.\n"
              "\n"
              "5. cout << “Hello World!”; – The cout object belongs to the iostream file and the purpose of this object is to display the content between double quotes as it is on the screen. This object can also display the value of variables on screen(don’t worry, we will see that in the coming tutorials).\n"
              "\n"
              "6. return 0; – This statement returns value 0 from the main() function which indicates that the execution of main function is successful. The value 1 represents failed execution."
              "*/";

    static constexpr auto Variables_CPP_03
            = "#include <iostream>\n"
              "using namespace std;\n"
              "// This is a global variable\n"
              "char myVar = 'A';\n"
              "char myFuncn() {\n"
              "   // This is a local variable\n"
              "   char myVar = 'B';\n"
              "   return myVar;\n"
              "}\n"
              "int main()\n"
              "{\n"
              "   cout <<\"Funcn call: \"<< myFuncn()<<endl;\n"
              "   cout <<\"Value of myVar: \"<< myVar<<endl;\n"
              "   myVar='Z';\n"
              "   cout <<\"Funcn call: \"<< myFuncn()<<endl;\n"
              "   cout <<\"Value of myVar: \"<< myVar<<endl;\n"
              "   return 0;\n"
              "}";

    static constexpr auto Operators_CPP_04
            = "#include <iostream>\n"
              "using namespace std;\n"
              "int main(){\n"
              "   int num1 = 11;  /* 11 = 00001011 */\n"
              "   int num2 = 22;  /* 22 = 00010110 */ \n"
              "   int result = 0;\n"
              "   result = num1 & num2;\n"
              "   cout<<\"num1 & num2: \"<<result<<endl;\n"
              "   result = num1 | num2;\n"
              "   cout<<\"num1 | num2: \"<<result<<endl;\n"
              "   result = num1 ^ num2;\n"
              "   cout<<\"num1 ^ num2: \"<<result<<endl;\n"
              "   result = ~num1;\n"
              "   cout<<\"~num1: \"<<result<<endl;\n"
              "   result = num1 << 2;\n"
              "   cout<<\"num1 << 2: \"<<result<<endl;\n"
              "   result = num1 >> 2;\n"
              "   cout<<\"num1 >> 2: \"<<result;\n"
              "   return 0;\n"
              "   int num1, num2; num1 = 99;\n"
              "  /* num1 is not equal to 10 that's why\n"
              "   * the second value after colon is assigned\n"
              "   * to the variable num2\n"
              "   */\n"
              "  num2 = (num1 == 10) ? 100: 200;\n"
              "  cout<<\"num2: \"<<num2<<endl;\n"
              "  /* num1 is equal to 99 that's why\n"
              "   * the first value is assigned\n"
              "   * to the variable num2\n"
              "   */\n"
              "  num2 = (num1 == 99) ? 100: 200;\n"
              "  cout<<\"num2: \"<<num2;"
              "}\n"
              "/*\n"
              "Unary Operators\n"
              "++ – – ! ~\n"
              "\n"
              "Multiplicative\n"
              "* / %\n"
              "\n"
              "Additive\n"
              "+ –\n"
              "\n"
              "Shift\n"
              "<< >> >>>\n"
              "\n"
              "Relational\n"
              "> >= < <=\n"
              "\n"
              "Equality\n"
              "== !=\n"
              "\n"
              "Bitwise AND\n"
              "&\n"
              "Bitwise XOR\n"
              "^\n"
              "\n"
              "Bitwise OR\n"
              "|\n"
              "\n"
              "Logical AND\n"
              "&&\n"
              "\n"
              "Logical OR\n"
              "||\n"
              "\n"
              "Ternary\n"
              "?:\n"
              "\n"
              "Assignment\n"
              "= += -= *= /= %= > >= < <= &= ^= |=\n"
              "*/";


    static constexpr auto IF_ELSE_CPP_05
            = "#include <iostream>\n"
              "using namespace std;\n"
              "int main(){\n"
              "   int num=66;\n"
              "   if( num < 50 ){\n"
              "      //This would run if above condition is true\n"
              "      cout<<\"num is less than 50\";\n"
              "   }\n"
              "   else {\n"
              "      //This would run if above condition is false\n"
              "      cout<<\"num is greater than or equal 50\";\n"
              "   }\n"
              "   return 0;\n"
              "}";

    static constexpr auto SWITCH_CPP_06
            = "/*\n"
              "Switch case statement is used when we have multiple conditions and we need to perform different action based on the condition. When we have multiple conditions and we need to execute a block of statements when a particular condition is satisfied. In such case either we can use lengthy if..else-if statement or switch case. The problem with lengthy if..else-if is that it becomes complex when we have several conditions. The switch case is a clean and efficient method of handling such scenarios.*/\n"
              "#include <iostream>\n"
              "using namespace std;\n"
              "int main(){\n"
              "   int i=2;\n"
              "   switch(i) {\n"
              "      case 1: cout<<\"Case1 \"<<endl;\n"
              "      case 2: cout<<\"Case2 \"<<endl;\n"
              "      case 3: cout<<\"Case3 \"<<endl;\n"
              "      case 4: cout<<\"Case4 \"<<endl;\n"
              "      default: cout<<\"Default \"<<endl; \n"
              "   }\n"
              "   return 0;\n"
              "}";

    static constexpr auto For_Loop_CPP_07
            = "/*\n"
              "A loop is used for executing a block of statements repeatedly until a particular condition is satisfied. For example, when you are displaying number from 1 to 100 you may want set the value of a variable to 1 and display it 100 times, increasing its value by 1 on each loop iteration.\n"
              "\n"
              "In C++ we have three types of basic loops: for, while and do-while. In this tutorial we will learn how to use “for loop” in C++.\n"
              "*/\n"
              "// for loop is range based loop.\n"
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

    static constexpr auto WHILE_DO_WHILE_CPP_08
            = "/*\n"
              "In while loop, condition is evaluated first and if it returns true then the statements inside while loop execute, this happens repeatedly until the condition returns false. When condition returns false, the control comes out of loop and jumps to the next statement in the program after while loop.\n"
              "*/\n"
              "#include <iostream>\n"
              "using namespace std;\n"
              "int main(){\n"
              "   int i=1;\n"
              "   /* The loop would continue to print\n"
              "    * the value of i until the given condition\n"
              "    * i<=6 returns false.\n"
              "    */\n"
              "   while(i<=6){\n"
              "      cout<<\"Value of variable i is: \"<<i<<endl; i++;\n"
              "   }\n"
              "   int num=1;\n"
              "   do{\n"
              "      cout<<\"Value of num: \"<<num<<endl;\n"
              "      num++;\n"
              "   }while(num<=6);"
              "}";


};

#endif //EVOLUTION_IDE_CODESAMPLES_H
