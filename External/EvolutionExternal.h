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
#include <filesystem>
#include <fstream>
#include <sstream>

#include <functional>
// #include <stack>

#include <chrono>

#include <assert.h>
#include <stdio.h>
#include <csignal>
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
        std::filesystem::path full_path(std::filesystem::current_path());
        full_path += "/Tests.txt";

        std::ofstream ofs(full_path, std::ios_base::app);
        ofs << output;
        ofs.close();
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const UnitTestData& data) {
        return data.success + ";" + data.errorCode + ";" + data.filepath + ";" + data.line + ";" + data.function + ";" + data.failureMessage;
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

#define REQUIRE(expr, failureMessage, errorCode) if(expr) { dataManager::processOutput(UnitTestData { __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, failureMessage, errorCode, "Success" }); } \
    else{ dataManager::processOutput(UnitTestData { __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__, failureMessage, errorCode, "Failure" }); }


template<typename T>
extern constexpr void REQUIRE_EQUAL(T what, T equalTo, const std::string& failureMessage = "", const std::string& errorCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    if(what == equalTo) {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "S" });
    }
    else {
        dataManager::processOutput(UnitTestData { file, line, func, failureMessage, errorCode, "F" });
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



// helpful functions to organize data for further debugging etc.
// also Log based functions
// IDE log system. In greater projects it is handy to know things hapening in code and do not use only unit testing macros
// mainly because unit tests are here to make sure code is going the right way, but this log system is used to
// see what ways it has chosen to go

class Logger {

    struct loggerData {
        std::string filePath;
        std::string line;
        std::string function;

        std::string logMessage;
        // code [M1, ECHO_01, ...] to organize all logs into dependent trees
        std::string sectionCode;
    };


    static inline void writeToFile(const std::string& output) {
        // executable path is tracked in IDE
        std::filesystem::path full_path(std::filesystem::current_path());
        full_path += "/Tests.txt";

        std::ofstream ofs(full_path, std::ios_base::app);
        ofs << output;
        ofs.close();
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const loggerData& data) {
        return data.filePath + ";" + data.line + ";" + data.function + ";" + data.logMessage + ";" + data.sectionCode;
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

template<typename T>
extern constexpr void LOG_INFO(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL
    // auto s = static_cast<const char *>(what);
#endif
}

#define LOG(logMessage, sectionCode) LOG_INFO(logMessage, sectionCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);

/*
template<typename T>
extern constexpr void LOG(T what[], const std::string& logMessage = "", const std::string& sectionCode = "") {
#ifdef EVOLUTION_EXTERNAL

#endif
}
*/

template<typename T>
extern constexpr void LOG_WARN(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL

#endif
}

#define LOG_WARNING(logMessage, sectionCode) LOG_WARN(logMessage, sectionCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);

template<typename T>
extern constexpr void LOG_ERR(const std::string& logMessage = "", const std::string& sectionCode = "", const std::string& file = "", const std::string& line = "", const std::string& func = "") {
#ifdef EVOLUTION_EXTERNAL

#endif
}

#define LOG_ERROR(logMessage, sectionCode) LOG_ERR(logMessage, sectionCode, __FILE__, std::to_string(__LINE__), __PRETTY_FUNCTION__);

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
        std::filesystem::path full_path(std::filesystem::current_path());
        full_path += "/Benchmarks.txt";

        std::ofstream ofs(full_path, std::ios_base::app);
        ofs << output;
        ofs.close();        
    }

    static inline void writeToConsole(const std::string& output) {
        std::cout << output;
    }

    static inline std::string constructOutput(const bench_data& data) {
        return "BENCH;" + data.bench_label;
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
extern inline void BENCH_START(const std::string& bench_label) {
    Benchmark::bench_data data;
    data.bench_label = bench_label;
    Benchmark::BenchData.push_back(data);

    // TODO: start benchmarking
}

extern inline void BENCH_END(const std::string& bench_label)  {
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


class MemmoryTracker {
public:

    struct Position {
        std::string filePath;
        std::string function;
        std::string line;
    };

    struct MemTrackData {
        std::string memTrackLabel;

        Position allocation;
        Position deallocation;

        std::string size; // bytes
        std::string blocks;

        std::string pointerAddress;
        std::string variableAddress;

        // std::atexit();
    };

    // count of both vector is the same
    static std::vector<MemTrackData> MemTrackedData;
    // tracking all pointers and its size, paired with vector above
    static std::vector<void *> allocatedPointers;

    static inline void handleAllocation(void *ptr) {
        MemmoryTracker::MemTrackData data;
        // int p = (int)&ptr;

        std::ostringstream get_the_address;
        // ptr address, not variable stored in
        // for variable address use only ptr
        get_the_address << &ptr;
        const std::string paddress =  get_the_address.str();
        data.pointerAddress = paddress;

        get_the_address << ptr;
        const std::string vaddress =  get_the_address.str();
        data.variableAddress = vaddress;

        data.size = sizeof(ptr);
    }

    static inline void handleDeletion(void* ptr) {
        // pair pointer and size
        for(unsigned int i = 1; i <= allocatedPointers.size(); i++) {
            if(allocatedPointers[i] == ptr) {
                if(sizeof(allocatedPointers[i]) == sizeof(ptr)) {
                    allocatedPointers.erase(allocatedPointers.begin(), allocatedPointers.begin() + i);
                    MemTrackedData.erase(MemTrackedData.begin(), MemTrackedData.begin() + i);
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

    static inline void writeToFile(const MemTrackData& data) {
        // executable path is tracked in IDE
        //std::filesystem::path full_path(std::filesystem::current_path());
        //full_path += "/Benchmarks.txt";

        //const std::string output = "{ BENCHMARK }" + data.bench_label;

        //std::ofstream ofs(full_path, std::ios_base::app);
        //ofs << output;
        //ofs.close();
    }

    static inline void writeToConsole(const MemTrackData& data) {
        //const std::string output = "{ BENCHMARK }" + data.bench_label;
        //std::cout << output;
    }

    static inline std::string constructOutput(const MemTrackData& data) {
        // return "MEM;" + data.filePath + ";" + data.line + ";" + data.function + ";" + data.address + ";" + data.blocks + ";" + data.size;
    }

    static inline void processOutput(const MemTrackData& data) {
    #ifdef EVOLUTION_EXTERNAL_CONSOLE
        writeToFile(data);
    #endif

    #ifdef EVOLUTION_EXTERNAL_FILE
        writeToConsole(data);
    #endif
    }

    static void signalHandler( int signum ) {
       // std::cout << "Interrupt signal (" << signum << ") received.\n";

       // cleanup and close up stuff here
       // terminate program

       exit(signum);
    }

};

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

#define OVERLOAD_ALL_BACK


// label if there are only some parts of code user want to track
// otherwise insert into main function start, end
extern inline void MEM_START(const std::string& memTrackLabel) {
    MemmoryTracker::MemTrackData data;
    data.memTrackLabel = memTrackLabel;
    // MemmoryTracker::MemTrackedData.push_back(data);




    // register signal SIGINT and signal handler
    // signal(SIGINT, MemmoryTracker::signalHandler);
}

#define MEMTRACK_START(memTrackLabel) MEM_START(memTrackLabel) OVERLOAD_ALL;

extern inline void MEM_STOP(const std::string& memTrackLabel) {
    for(const auto& d : MemmoryTracker::MemTrackedData) {
        if(d.memTrackLabel == memTrackLabel) {
            // ...
        }
    }
}

#define MEMTRACK_STOP(memTrackLabel) MEM_STOP(memTrackLabel)






#endif // EVOLUTIONEXTERNAL_H
