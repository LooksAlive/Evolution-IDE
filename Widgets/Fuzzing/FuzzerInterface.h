#ifndef FUZZERINTERFACE_H
#define FUZZERINTERFACE_H

/*
 * at first there is llvm-fuzz injection
 * suggetions, where to put this fuzzer
 * basic settings, how many times to repeat, etc.
 * add flag to clang for this section
 *
 * implement also some coverage generators, AFL, and other fuzzers
 *
 * later create custom fuzzer function inside IDE
 *
 * add also interface for sanitizing flags within clang
*/

/*
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  DoSomethingWithData(Data, Size);
  return 0;
}


clang++ -g -fsanitize=address,fuzzer fuzzing/tutorial/libFuzzer/fuzz_me.cc

- number of operations --- !!! implement
./a.out -runs=1000000



https://www.moritz.systems/blog/an-introduction-to-llvm-libfuzzer/
*/

#include <QWidget>

class FuzzerInterface : public QWidget
{
    Q_OBJECT
public:
    explicit FuzzerInterface(QWidget *parent = nullptr);
    ~FuzzerInterface() = default;

signals:

};

#endif // FUZZERINTERFACE_H
