#ifndef EVOLUTIONEXTERNAL_H
#define EVOLUTIONEXTERNAL_H

/*
 * unit testing output is going to be in specific format to parse it in console later    XXXXXXXXXXXXX
 * It could also create a file, fill it and after execution read it and display in console [for each section]
 * In this IDE is console way prefered
 *
 *
 * NOTE: all of this can be simply shutdown in compile stage if macro is not defined (handy stuff)
*/

// set this flags in your main(); before including other files

// base macro to compile this code
#define EVOLUTION_EXTERNAL
// output in console (in Evolution-IDE is output parsed and selected)
#define EVOLUTION_EXTERNAL_CONSOLE
// output into file(s) (if someone want to see rather all of it in files)
// no json file; simple txt formated output
#define EVOLUTION_EXTERNAL_FILE


// TODO: add ftm library to console output
// TODO: catch signal SIGINT and then output all all at the end


#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <functional>
// #include <stack>
// https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3
// https://www.tutorialspoint.com/how-to-automatically-generate-a-stacktrace-when-a-gcc-cplusplus-program-crashes
// use deep backtrace, also implement signal when error occures.
// this replaces debugging stages.
#include <execinfo.h>  // for backtrace
#include <dlfcn.h>     // for dladdr
#include <cxxabi.h>    // for __cxa_demangle

#include <chrono>

#include <assert.h>
#include <stdio.h>
// #include <csignal>
#include <signal.h>
#include <stdlib.h>
#include <algorithm>
#include <new>
#include <typeinfo>





struct UnitTestData {
    std::string filepath;
    std::string line;
    std::string function;

    std::string failureMessage;
    // std::string successMessage;
    std::string errorCode;

    std::string success;    
};



class dataManager {
public:
    static inline void writeToFile(const std::string& output) {
        // executable path is tracked in IDE
        /*
        std::filesystem::path full_path(std::filesystem::current_path());
        full_path += "/Tests.txt";

        std::ofstream ofs(full_path, std::ios_base::app);
        ofs << output;
        ofs.close();
        */
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const UnitTestData& data) {
        return data.success + ";" + data.filepath + ";" + data.line + ";" + data.function + ";" + data.errorCode  + ";" + data.failureMessage + "\n";
    }

    static inline void processOutput(const UnitTestData& data) {
    #ifdef EVOLUTION_EXTERNAL_CONSOLE
        writeToFile(constructOutput(data));
    #endif

    #ifdef EVOLUTION_EXTERNAL_FILE
        writeToConsole(constructOutput(data));
    #endif
    }

};

// REQUIRE(5 == 5) for example with expression as first argument

#define REQUIRE(expr, failureMessage, errorCode) if(expr) { dataManager::processOutput(UnitTestData { __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, failureMessage, errorCode, "SUCCESS" }); } \
    else{ dataManager::processOutput(UnitTestData { __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, failureMessage, errorCode, "FAILURE" }); }


template<typename T>
extern constexpr void REQUIRE_EQUAL(T what, T equalTo, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(typeid(what) == typeid(equalTo)) {

    }
    if(what == equalTo) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "SUCCESS" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "FAILURE" });
    }
#endif
}

#define REQUIRE_EQ(what, equalTo, failureMessage, errorCode) REQUIRE_EQUAL(what, equalTo, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);

template<typename T>
extern constexpr void REQUIRE_NOT_EQUAL(T what, T equalTo, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(what != equalTo) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "S" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
    }
#endif
}

#define REQUIRE_NOT_EQ(what, equalTo, failureMessage, errorCode) REQUIRE_NOT_EQUAL(what, equalTo, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);

template<typename T>
extern constexpr void REQUIRE_GREATERT_THAN(T what, T greaterThan, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(what > greaterThan) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "S" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
    }
#endif
}

#define REQUIRE_GT(what, equalTo, failureMessage, errorCode) REQUIRE_GREATERT_THAN(what, equalTo, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);


template<typename T>
extern constexpr void REQUIRE_GREATERT_THAN_EQUAL(T what, T greaterEqualThan, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(what >= greaterEqualThan) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "S" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
    }
#endif
}

#define REQUIRE_GTE(what, equalTo, failureMessage, errorCode) REQUIRE_GREATERT_THAN_EQUAL(what, equalTo, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);


template<typename T>
extern constexpr void REQUIRE_LOWER_THAN(T what, T lowerThan, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(what < lowerThan) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "S" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
    }
#endif
}

#define REQUIRE_LT(what, equalTo, failureMessage, errorCode) REQUIRE_LOWER_THAN(what, equalTo, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);


template<typename T>
extern constexpr void REQUIRE_LOWER_THAN_EQUAL(T what, T lowerEqualThan, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(what <= lowerEqualThan) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "S" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
    }
#endif
}

#define REQUIRE_LTE(what, equalTo, failureMessage, errorCode) REQUIRE_LOWER_THAN_EQUAL(what, equalTo, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);


template<typename T>
extern constexpr void REQUIRE_PTR_NOT_NULL(const T* ptr, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(ptr == nullptr) {
        if(failureMessage.empty()) {
            dataManager::processOutput(UnitTestData { file, line, func, "not valid pointer", errorCode, "F" });
            return;
        }
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
        return;
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, "pointer is valid", errorCode, "S" });
    }

#endif
}

#define REQUIRE_PTR(ptr, failureMessage, errorCode) REQUIRE_PTR_NOT_NULL(ptr, failureMessage, errorCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);


// check pointer address validation via syscall
// NOTE: slows down a program
template<typename T>
extern constexpr void CHECK_PTR_ADDRRESS(const T* ptr, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL

#endif
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-program-crashes


// helpful functions to organize data for further debugging etc.
// also Log based functions
// IDE log system. In greater projects it is handy to know things hapening in code and do not use only unit testing macros
// mainly because unit tests are here to make sure code is going the right way, but this log system is used to
// see what ways it has chosen to go

class Logger {
public:

    struct loggerData {
        std::string filePath;
        std::string line;
        std::string function;

        std::string logMessage;
        // code [M1, ECHO_01, ...] to organize all logs into dependent trees
        std::string sectionCode;

        std::string type;
    };

    // label, log message
    static std::vector<std::pair<std::string, std::string>> internalLogBuffer;

    static inline void writeToFile(const std::string& output) {
        // executable path is tracked in IDE
        /*
        std::filesystem::path full_path(std::filesystem::current_path());
        full_path += "/Tests.txt";

        std::ofstream ofs(full_path, std::ios_base::app);
        ofs << output;
        ofs.close();
        */
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const loggerData& data) {
        return data.type + ";" + data.filePath + ";" + data.line + ";" + data.function + ";" + data.sectionCode + ";" + data.logMessage + "\n";
    }

    static inline void processOutput(const loggerData& data) {
    #ifdef EVOLUTION_EXTERNAL_CONSOLE
        writeToFile(constructOutput(data));
    #endif

    #ifdef EVOLUTION_EXTERNAL_FILE
        writeToConsole(constructOutput(data));
    #endif
    }

};

extern inline void LOG_INFO(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "", const std::string& type = "") {
#ifdef EVOLUTION_EXTERNAL
    Logger::processOutput(Logger::loggerData { file, line, func, logMessage, sectionCode, type });
#endif
}

#define LOG(logMessage, sectionCode) LOG_INFO(logMessage, sectionCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, "INFO");

extern inline void LOG_APPEND_WRITE(const std::string& label, const std::string& append_text, const bool& write = false, const std::string& file = "", const std::string& line = "", const std::string& func = "", const std::string& type = "") {
#ifdef EVOLUTION_EXTERNAL
    if(write) {
        for(unsigned int i = 0; i < Logger::internalLogBuffer.size(); i++) {
            if(Logger::internalLogBuffer[i].first == label) {
                Logger::processOutput(Logger::loggerData { file, line, func, Logger::internalLogBuffer[i].second, "sectionCode", type });
                Logger::internalLogBuffer.erase(Logger::internalLogBuffer.begin(), Logger::internalLogBuffer.begin() + i);
            }
        }
    }
    else {
        for(auto& lbl : Logger::internalLogBuffer) {
            if(lbl.first == label) {
                lbl.second.append(append_text);
            }
            return;
        }
        // not yet in buffer

        Logger::internalLogBuffer.push_back(std::make_pair(label, append_text));
    }
#endif
}

// use internal buffer to concatenate 1 log message comming from more places
// only frst call position is tracked, for now.
#define LOG_APPEND(label, append_text) LOG_APPEND_WRITE(label, append_text, false, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, "INFO");
// outputs concatenated log message to be recorded + free buffer
// NOTE: once writen, cannot be again even with same label writes nothing ""
#define LOG_WRITE(label) LOG_APPEND_WRITE(label, "", true, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, "INFO");

/*
extern constexpr void LOG(T what[], const std::string& logMessage = "", const std::string& sectionCode = "") {
#ifdef EVOLUTION_EXTERNAL

#endif
}
*/

extern inline void LOG_WARN(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "", const std::string& type = "") {
#ifdef EVOLUTION_EXTERNAL
    Logger::processOutput(Logger::loggerData { file, line, func, logMessage, sectionCode, type });
#endif
}

#define LOG_WARNING(logMessage, sectionCode) LOG_WARN(logMessage, sectionCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, "WARNING");

extern inline void LOG_ERR(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "", const std::string& type = "") {
#ifdef EVOLUTION_EXTERNAL
     Logger::processOutput(Logger::loggerData { file, line, func, logMessage, sectionCode, type });
#endif
}

#define LOG_ERROR(logMessage, sectionCode) LOG_ERR(logMessage, sectionCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, "ERROR");
/*
template<typename T>
extern constexpr void LOG_VALUE(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "", const std::string& type = "") {
#ifdef EVOLUTION_EXTERNAL
     Logger::processOutput(Logger::loggerData { file, line, func, logMessage, sectionCode, type });
#endif
}
*/


// might be this one
// log also with assert
// REQUIRE function

#define REQUIRE_LOG(macroTest, logMessage, sectionCode, file, line, func)  macroTest(logMessage, sectionCode, file, line, func);




/* Benchmarking
 * 
 * Is done by 2 functions start, endand can be used in code 
 * each bench starts its own timer and END terminates it by bench label
*/


class Benchmark {
public:
    struct bench_data {
        std::string filePath;
        std::string line;
        std::string function;

        std::string bench_label;
        // chrono stuffs
        // start automatically when struct created
        const std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        std::chrono::milliseconds duration;
        std::chrono::milliseconds max;
        std::chrono::milliseconds min;
        // std::chrono::milliseconds::max();
        // measurements
        std::string cpuTicks;
    };

    static inline void writeToFile(const std::string& output) {
        // executable path is tracked in IDE
        /*
        std::filesystem::path full_path(std::filesystem::current_path());
        full_path += "/Benchmarks.txt";

        std::ofstream ofs(full_path, std::ios_base::app);
        ofs << output;
        ofs.close();        
        */
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const bench_data& data) {
        return "BENCHMARK;" + data.filePath + ";" + data.line + ";" + data.function + ";" + data.bench_label + "\n";
    }

    static inline void processOutput(const bench_data& data) {
    #ifdef EVOLUTION_EXTERNAL_CONSOLE
        writeToFile(constructOutput(data));
    #endif

    #ifdef EVOLUTION_EXTERNAL_FILE
        writeToConsole(constructOutput(data));
    #endif
    }

    static std::vector<Benchmark::bench_data> BenchData;
};


// label holds benchmark name user would define in case of organizing more benchs
// bc. there might be more of them, so they have its label
extern inline void BENCH_START(const std::string& bench_label, const std::string& file = "", const std::string& line = "", const std::string& func = "") {
    Benchmark::bench_data data;
    data.bench_label = bench_label;
    Benchmark::BenchData.push_back(data);

    // TODO: start benchmarking
}

#define BENCHMARK_START(bench_label) BENCH_START(bench_label, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);

extern inline void BENCH_END(const std::string& bench_label, const std::string& file = "", const std::string& line = "", const std::string& func = "")  {
    for(const auto& d : Benchmark::BenchData) {
        if(d.bench_label == bench_label) {
            // d. ....  stop
            const auto end = std::chrono::steady_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - d.start).count(); // ms,   µs
            // d.duration = duration;
            // calculate them and insert to data structure to write down later
            Benchmark::processOutput(d);
            return;
        }
    }
}

#define BENCHMARK_END(bench_label) BENCH_END(bench_label, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);



/* Fuzzer
 *
 *
*/

class Fuzzer {

    struct fuzzerData {

    };

};


enum Operation {
    filePath = 0,
    variable
};

// TODO: fix this somehow
template<typename T>
extern inline void FUZZ_LOOP(const int& range, std::function<T (T args ...)> func) {
    va_list arguments;
    T arg;
    for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char *)) {
        std::cout << arg;
        // T retVal = func(args...);
    }

    va_end(arguments);

    for(int i = 0; i< range; i++) {
        T retVal = func(0, 0, 0);
        // manage return value for function



    }
}

template<typename T>
extern inline void FUZZ_LOOP(const int& range, std::function<T> func, const std::vector<T>& dataArgs ...) {
    va_list arguments;
    T arg;
    for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char *)) {
        std::cout << arg;
        // T retVal = func(args...);
    }

    va_end(arguments);

    for(int i = 0; i< range; i++) {
        T retVal = func(0, 0, 0);
        // manage return value for function

    }
}


template<typename T>
extern inline void FUZZ_LOOP(const int& range, const T& data, const Operation& op, std::function<T> func, const std::vector<T>& dataArgs ...) {
    switch (op) {
        case filePath:
            break;
        case variable:
            break;

    }


    va_list arguments;
    for (va_start(arguments, dataArgs); dataArgs != NULL; dataArgs = va_arg(arguments, const char *)) {
        std::cout << dataArgs;
        // T retVal = func(args...);
    }

    va_end(arguments);

    for(int i = 0; i< range; i++) {
        T retVal = func(0, 0, 0);
        // manage return value for function



    }
}

template<typename T>
extern inline void FUZZ_RANDOM(const int& range, const T& data, const Operation& op, std::function<T> func, const std::vector<T>& dataArgs ...) {
    switch (op) {
        case filePath:
            break;
        case variable:
            break;

    }


    va_list arguments;
    T arg;
    for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char *)) {
        std::cout << arg;
        // T retVal = func(args...);
    }

    va_end(arguments);

    for(int i = 0; i< range; i++) {
        T retVal = func(0, 0, 0);
        // manage return value for function



    }
}

enum Type {
    integer = 0
};

extern inline void FUZZ_GENERATE_RANDOM() {

}




/* Memory manager
 *
 * It is sometimes better track this in code, but one can also use valgrind or such tools.
 * But this way we can actually in execution see where is memory allocated, etc. instead of
 * running within other memory profiling tools.
*/

// https://github.com/seladb/MemPlumber
// http://www.almostinfinite.com/memtrack.html      https://github.com/bertrandmt/memtrack
// https://github.com/uraimo/barememtracer      ---
// https://github.com/mpetri/mem_monitor
// https://github.com/micwypych/MemoryLeakDetection     --

// tools:
// https://github.com/yodaos-project/memory-trace
// https://github.com/samsk/log-malloc2

#define VARIABLE_TO_STRING(Variable) (void(Variable),#Variable);
#define TYPE_TO_STRING(Type) (void(sizeof(Type)),#Type)
// https://stackoverflow.com/questions/3386861/converting-a-variable-name-to-a-string-in-c
class MemmoryTracker {
public:

    struct Position {
        std::string filePath;
        std::string function;
        std::string line;

        std::string size; // bytes
        // std::string blocks;
    };

    struct MemTrackData {
        std::string memTrackLabel;

        Position allocation;
        Position deallocation;

        std::string variableName;
        std::string variableType;
        std::string pointerAddress;
        std::string variableAddress;

        // std::vector<std::string> varList;
        void* ptr;
        // track double deletion
        bool freed = false;

        // std::atexit();
    };

    // count of both vector is the same
    static std::vector<MemTrackData> MemTrackedData;

    static void handleAllocation(void *ptr);

    static void handleDeletion(void* ptr);

    static const char* getCaller() {
        void* backtraceArr[3];
        size_t backtraceArrSize;

        // get void*'s for all entries on the stack
        backtraceArrSize = backtrace(backtraceArr, 3);

        if (backtraceArrSize < 3) {
            return "Unknown";
        }

        // get the symbols
        char** backtraceSymbols = backtrace_symbols(backtraceArr, backtraceArrSize);

        // the caller is second in the backtrace
        return backtraceSymbols[2];
    }

    static inline void writeToFile(const std::string& output) {
        // executable path is tracked in IDE
        //std::filesystem::path full_path(std::filesystem::current_path());
        //full_path += "/Benchmarks.txt";

        //const std::string output = "{ BENCHMARK }" + data.bench_label;

        //std::ofstream ofs(full_path, std::ios_base::app);
        //ofs << output;
        //ofs.close();
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const MemTrackData& data) {
        return "MEMTRACK;" + data.variableType + ";" + data.variableName + ";" + data.pointerAddress + ";" + data.variableAddress + ";" +
                data.allocation.filePath + ";" + data.allocation.line + ";" + data.allocation.function + ";" + data.allocation.size
                + ";" + data.deallocation.filePath + ";" + data.deallocation.line + ";" + data.deallocation.function + ";" + data.deallocation.size/* + constructValuesOutput(data.varList)*/ + "\n";
    }

    static inline std::string constructValuesOutput(const std::vector<std::string>& list) {
        std::string output;
        for(const auto& str : list) {
            output += ";" + str;
        }
        return output;
    }

    static inline void processOutput(const MemTrackData& data) {
    #ifdef EVOLUTION_EXTERNAL_CONSOLE
        writeToFile(constructOutput(data));
    #endif

    #ifdef EVOLUTION_EXTERNAL_FILE
        writeToConsole(constructOutput(data));
    #endif
    }

    static void signalHandler( int signum ) {
       // std::cout << "Interrupt signal (" << signum << ") received.\n";

       // cleanup and close up stuff here
       // terminate program

       exit(signum);
    }

};

std::vector<MemmoryTracker::MemTrackData> MemmoryTracker::MemTrackedData;
void MemmoryTracker::handleDeletion(void* ptr) {
    // pair pointer and size
    for(unsigned int i = 1; i <= MemmoryTracker::MemTrackedData.size(); i++) {
        if(MemmoryTracker::MemTrackedData[i].ptr == ptr) {
            if(MemmoryTracker::MemTrackedData[i].freed) {
                // more times deleted same pointer, therefore it was not found and yet deleting by user
                // raise error message

                break;
            }
            if(sizeof(MemmoryTracker::MemTrackedData[i].ptr) == sizeof(ptr)) {
                // finish data scraping and output them
                MemmoryTracker::MemTrackedData[i].deallocation.filePath = __FILE__;
                MemmoryTracker::MemTrackedData[i].deallocation.line = std::to_string(__LINE__);
                MemmoryTracker::MemTrackedData[i].deallocation.function = __PRETTY_FUNCTION__;
                MemmoryTracker::MemTrackedData[i].deallocation.size = sizeof(ptr);
                // FIXME: not entirely freed resources might be freed later, sooooo ?
                if(MemmoryTracker::MemTrackedData[i].allocation.size != MemmoryTracker::MemTrackedData[i].deallocation.size) {
                    // solve this
                }
                MemmoryTracker::MemTrackedData[i].freed = true;
                processOutput(MemmoryTracker::MemTrackedData[i]);
                // not needed anymore
                // MemTrackedData.erase(MemTrackedData.begin(), MemTrackedData.begin() + i);

                break;
            }
            // in case of delete[i]
            else {
                std::cout << "Not everything is freed";
                // int* a = new int[10];
                // delete[] &a[5];
            }
        }
    }
}

void MemmoryTracker::handleAllocation(void *ptr) {
    printf ("allocated in : %u %d", sizeof(ptr), __LINE__);
    // int p = (int)&ptr;
    /*
    std::ostringstream get_the_address;
    // ptr address, not variable stored in
    // for variable address use only ptr
    data.allocation.filePath = __FILE__;
    data.allocation.line = std::to_string(__LINE__);
    data.allocation.function = __PRETTY_FUNCTION__;

    data.variableName = VARIABLE_TO_STRING(ptr);
    data.variableType = TYPE_TO_STRING(ptr);

    get_the_address << &ptr;
    const std::string paddress =  get_the_address.str();
    data.pointerAddress = paddress;

    get_the_address << ptr;
    const std::string vaddress =  get_the_address.str();
    data.variableAddress = vaddress;

    data.allocation.size = sizeof(ptr);

    data.ptr = ptr;

    MemmoryTracker::MemTrackedData.push_back(data);
    */
}
/*
#define OVERLOAD_ALL \
    void* operator new(std::size_t size) { \
        void *ptr = malloc(size); \
        MemmoryTracker::handleAllocation(ptr); \
        return ::operator new(size); \
    } \
    void* operator new[](std::size_t size) { \
        void *ptr = malloc(size); \
        MemmoryTracker::handleAllocation(ptr); \
        return ::operator new[](size); \
    } \
    void operator delete(void* p) { \
        MemmoryTracker::handleDeletion(p); \
        return ::operator delete(p); \
    } \
    void operator delete[](void* p) { \
        MemmoryTracker::handleDeletion(p); \
        return ::operator delete(p); \
    } \
*/

/*
void* operator new(std::size_t size) {
    void *ptr = malloc(size);
    std::cout << MemmoryTracker::getCaller();
    // MemmoryTracker::handleAllocation(ptr);
    return ptr;
}
void* operator new[](std::size_t size) {
    void *ptr = malloc(size);
    //MemmoryTracker::handleAllocation(ptr);
    return ptr;
}
void operator delete(void* p) {
    //MemmoryTracker::handleDeletion(p);
    return free(p);
}
void operator delete[](void* p) {
    //MemmoryTracker::handleDeletion(p);
    return free(p);
}
*/
#define OVERLOAD_ALL_BACK


// updates containers for all knows ptr(s) values
// at start this is never called. call whenever you want to know them.
extern inline void MEM_REGISTER_VALUES(const std::string& memTrackLabel = "") {
    /*
    for(unsigned int i = 1; i <= MemmoryTracker::allocatedPointers.size(); i++) {
        if(MemmoryTracker::allocatedPointers[i] && typeid(MemmoryTracker::allocatedPointers[i]) == typeid(std::string)) {
            auto v = MemmoryTracker::allocatedPointers[i];

        }
    }
    */
}

// label if there are only some parts of code user want to track
// otherwise insert into main function start, end
extern inline void MEM_START(const std::string& memTrackLabel = "") {
    // MemmoryTracker::MemTrackData data;
    // data.memTrackLabel = memTrackLabel;
    // MemmoryTracker::MemTrackedData.push_back(data);




    // register signal SIGINT and signal handler
    // signal(SIGINT, MemmoryTracker::signalHandler);
}

#define MEMTRACK_START(memTrackLabel) /*OVERLOAD_ALL;*/ MEM_START(memTrackLabel);

extern inline void MEM_STOP(const std::string& memTrackLabel = "") {
    // TODO: memTrackLabel was yet not considered

    // not freed resources does not have deallocation data set
    if(!MemmoryTracker::MemTrackedData.empty()) {
        for(auto& d : MemmoryTracker::MemTrackedData) {
            // if(d.memTrackLabel == memTrackLabel)
                // ...
            d.deallocation.size = "0";

            MemmoryTracker::processOutput(d);
        }
    }
}

#define MEMTRACK_STOP(memTrackLabel) MEM_STOP(memTrackLabel);




/* Backtrace, Call Stack traces when signal emited
 * will install signal callback and print comma separated callstack data to handle in IDE console
*/

// https://gist.github.com/jvranish/4441299

class CallStack {
public:

    struct StackData {
        std::string sname;
        std::string saddr;
    };

    struct CallStackData {
        std::string signalID;
        // sname, saddr
        std::vector<std::pair<std::string, std::string>> stackData;
    };


    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const CallStackData& data) {
        std::string output = "BACKTRACE;";
        for(int i = 0; i < data.stackData.size(); i++) {
            output += ";" + data.stackData[i].first + ";" + data.stackData[i].second;
        }
        return output;
    }

    static inline void writeToFile(const std::string& output) {

    }

    static inline void processOutput(const CallStackData& data) {
    #ifdef EVOLUTION_EXTERNAL_CONSOLE
        writeToFile(constructOutput(data));
    #endif

    #ifdef EVOLUTION_EXTERNAL_FILE
        writeToConsole(constructOutput(data));
    #endif
    }

    static std::string Backtrace(int sigID) {
        CallStackData data;
        data.signalID = sigID;

        void *callstack[128];
        const int nMaxFrames = 10;  //sizeof(callstack) / sizeof(callstack[0]);
        char buf[1024];
        int nFrames = backtrace(callstack, nMaxFrames);
        char **symbols = backtrace_symbols(callstack, nFrames);

        std::ostringstream trace_buf;
        for (int i = 0; i < nFrames; i++) {
            Dl_info info;
            if (dladdr(callstack[i], &info)) {
                char *demangled = NULL;
                int status;
                demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
                snprintf(buf, sizeof(buf), "%-3d %*0p %s + %zd\n",
                         i, 2 + sizeof(void*) * 2, callstack[i],
                         status == 0 ? demangled : info.dli_sname,
                         (char *)callstack[i] - (char *)info.dli_saddr);
                free(demangled);
            } else {
                snprintf(buf, sizeof(buf), "%-3d %*0p\n",
                         i, 2 + sizeof(void*) * 2, callstack[i]);
            }
            trace_buf << buf;

            snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
            trace_buf << buf;
        }
        free(symbols);
        // data.stackData.push_back(std::pair( info.dli_sname, info.dli_saddr));


        //if (nFrames == nMaxFrames)
        //    trace_buf << "[truncated]\n";
        return trace_buf.str();
    }


    void signalHandler(int sig) {
      Backtrace(sig);
      exit(1);
    }

};

extern const char* BACKTRACE(const int& deph = 10) {
    void *array[deph];
    std::size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, deph);

    char** symbols = backtrace_symbols(array, size);
    const char* trace = *symbols;
    free(symbols);

    return trace;
}

#define SIGNAL_HANDLER signal(SIGSEGV, CallStack::signalHandler); \
                       signal(SIGINT, CallStack::signalHandler); \
                       signal(SIGKILL, CallStack::signalHandler); \





#endif // EVOLUTIONEXTERNAL_H
