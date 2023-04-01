#ifndef SSTEST_BASE_H
#define SSTEST_BASE_H

#include <exception>

// =======================================================================
// Macros for allowing overloadable Macro functions
// =======================================================================

// https://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments
#define INTERNAL_ssTEST_CAT( A, B ) A ## B
#define INTERNAL_ssTEST_SELECT( NAME, NUM ) INTERNAL_ssTEST_CAT( NAME ## _, NUM )
#define INTERNAL_ssTEST_COMPOSE( NAME, ARGS ) NAME ARGS

#define INTERNAL_ssTEST_GET_COUNT( _0, _1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ... ) COUNT
#define INTERNAL_ssTEST_EXPAND() ,,,,,, // 6 commas (or 7 empty tokens)
#define INTERNAL_ssTEST_VA_SIZE( ... ) INTERNAL_ssTEST_COMPOSE( INTERNAL_ssTEST_GET_COUNT, (INTERNAL_ssTEST_EXPAND __VA_ARGS__ (), 0, 6, 5, 4, 3, 2, 1) )

#ifndef _MSC_VER
#define INTERNAL_ssTEST_VA_SELECT( NAME, ... ) INTERNAL_ssTEST_SELECT( NAME, INTERNAL_ssTEST_VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
#else

//MSVC workaround: https://stackoverflow.com/questions/48710758/how-to-fix-variadic-macro-related-issues-with-macro-overloading-in-msvc-mic
//This is honestly retarded.
#define INTERNAL_ssTEST_VA_ARGS_FIX( macro, args ) macro args
#define INTERNAL_ssTEST_VA_SELECT( NAME, ... ) INTERNAL_ssTEST_VA_ARGS_FIX(INTERNAL_ssTEST_SELECT, ( NAME, INTERNAL_ssTEST_VA_SIZE( __VA_ARGS__ ) )) (__VA_ARGS__)
#endif


#include <string>
#include <functional>
#include <iostream>
#include <cstdio>
#include <vector>


#define INTERNAL_ssTEST_FILE_NAME()\
[]()\
{\
    std::string ssTestfileName = __FILE__;\
    std::size_t ssTestfound = ssTestfileName.find_last_of("/\\");\
    std::size_t ssTestExtfound = ssTestfileName.rfind(".c");\
    return ssTestfileName.substr(ssTestfound+1, ssTestExtfound - ssTestfound - 1);\
}()

#ifdef _WIN32
    #include <windows.h>
    #define INTERNAL_ssTEST_TITLE(title)\
    SetConsoleOutputCP(CP_UTF8);\
    ssTestName = title;\
    printf("==========================================================\n");\
    printf("🚀 %s\n", title.c_str());\
    printf("==========================================================\n");
#else
    #define INTERNAL_ssTEST_TITLE(title)\
    ssTestName = title;\
    printf("==========================================================\n");\
    printf("🚀 %s\n", title.c_str());\
    printf("==========================================================\n");
#endif

#define ssTEST_SET_UP ;ssTestSetUp = [&]()
#define ssTEST_CLEAN_UP ;ssTestCleanUp = [&]()

#define ssTEST_CALL_SET_UP() ssTestSetUp()
#define ssTEST_CALL_CLEAN_UP() ssTestCleanUp()

#define ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS()\
;ssTestResetBetweenTests = false;

//#define ssTEST( ... ) INTERNAL_ssTEST_VA_SELECT(ssTEST, __VA_ARGS__)

#define INTERNAL_ssTEST_SETUP_INIT_IF_NEEDED()\
;if(!ssTestSetUpCalled)\
{\
    ssTestSetUp();\
    ssTestSetUpCalled = true;\
}\

#define ssTEST(name)\
;ssTestFunctions.resize(ssTestFunctions.size()+1);\
ssTestFunctionsNames.push_back(name);\
ssTestFunctionsSkipFlags.push_back(false);\
ssTestFunctions[ssTestFunctions.size() - 1] = [&]()

#define ssTEST_SKIP(name)\
;ssTestFunctions.resize(ssTestFunctions.size()+1);\
ssTestFunctionsNames.push_back(name);\
ssTestFunctionsSkipFlags.push_back(true);\
ssTestFunctions[ssTestFunctions.size() - 1] = [&]()

#define ssTEST_OUTPUT_ASSERT( ... ) INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_ASSERT, __VA_ARGS__ )

#define INTERNAL_ssTEST_OUTPUT_ASSERT_1(assert) INTERNAL_ssTEST_OUTPUT_ASSERT_2("", assert)

#define INTERNAL_ssTEST_OUTPUT_ASSERT_2(info, assert)\
{\
    std::string titleString;\
    std::string resultString;\
    try\
    {\
        bool result = assert;\
        if(std::string(info).empty())\
            titleString = std::string("🧪 ")+ssTestFunctionsNames[ssTestCurrentTestIndex];\
        else\
            titleString = std::string("🧪 ")+ssTestFunctionsNames[ssTestCurrentTestIndex]+std::string(" (")+std::string(info)+std::string(")");\
        if(result)\
        {\
            resultString = "Passed ✅";\
            ssTestSuccess++;\
        }\
        else\
        {\
            resultString = "Failed ❌";\
            ssTestFailed++;\
        }\
    }\
    catch(std::exception& e)\
    {\
        titleString = ssTestFunctionsNames[ssTestCurrentTestIndex];\
        resultString = std::string("Error Catched: ")+std::string(e.what());\
        ssTestFailed++;\
    }\
    catch(...)\
    {\
        titleString = ssTestFunctionsNames[ssTestCurrentTestIndex];\
        resultString = std::string("Error occured");\
        ssTestFailed++;\
        throw;\
    }\
    if(titleString.size() < 50)\
        printf("%-50s %20s\n", (titleString+":").c_str(), resultString.c_str());\
    else\
        printf("%-60s %20s\n", (titleString+":").c_str(), resultString.c_str());\
}

#define ssTEST_OUTPUT_SKIP( ... ) INTERNAL_ssTEST_VA_SELECT( INTERNAL_ssTEST_OUTPUT_SKIP, __VA_ARGS__ )

#define INTERNAL_ssTEST_OUTPUT_SKIP_0() INTERNAL_ssTEST_OUTPUT_SKIP_1("")

#define INTERNAL_ssTEST_OUTPUT_SKIP_1(info)\
{\
    std::string titleString;\
    if(std::string(info).empty())\
        titleString = std::string("🧪 ")+ssTestFunctionsNames[ssTestCurrentTestIndex];\
    else\
        titleString = std::string("🧪 ")+ssTestFunctionsNames[ssTestCurrentTestIndex]+std::string(" (")+std::string(info)+std::string(")");\
    std::string resultString = "Skipped 🚫";\
    printf("%-50s %21s\n", (titleString+":").c_str(), resultString.c_str());\
}

#define ssTEST_INIT()\
{\
    int ssTestSuccess = 0;\
    int ssTestFailed = 0;\
    std::string ssTestName = "";\
    std::vector<std::function<void()>> ssTestFunctions;\
    std::vector<std::string> ssTestFunctionsNames;\
    std::vector<bool> ssTestFunctionsSkipFlags;\
    int ssTestCurrentTestIndex = 0;\
    bool ssTestSetUpCalled = false;\
    bool ssTestResetBetweenTests = true;\
    std::function<void()> ssTestSetUp = [](){};\
    std::function<void()> ssTestCleanUp = [](){};\
    try\
    {\
        INTERNAL_ssTEST_TITLE(INTERNAL_ssTEST_FILE_NAME());

#define ssTEST_END()\
        ;if(!ssTestResetBetweenTests)\
            ssTestSetUp();\
        for(int i = 0; i < ssTestFunctions.size(); i++)\
        {\
            ssTestCurrentTestIndex = i;\
            if(ssTestFunctionsSkipFlags[i])\
            {\
                ssTEST_OUTPUT_SKIP();\
                continue;\
            }\
            else\
            {\
                if(ssTestResetBetweenTests)\
                    ssTestSetUp();\
                \
                ssTestFunctions[i]();\
                \
                if(ssTestResetBetweenTests)\
                    ssTestCleanUp();\
            }\
        }\
        if(!ssTestResetBetweenTests)\
            ssTestCleanUp();\
        int ssTotal = ssTestSuccess + ssTestFailed;\
        std::cout<<"\nResults:\n";\
        std::cout<<ssTestSuccess<<"/"<<ssTotal<<" tests passed\n";\
        if(ssTestFailed > 0)\
        {\
            std::cout<<ssTestName<<" has failed some tests 😟\n\n";\
            return EXIT_FAILURE;\
        }\
        else\
        {\
            std::cout<<ssTestName<<" has passed all tests 😄\n\n";\
            return EXIT_SUCCESS;\
        }\
    }\
    catch(std::exception e)\
    {\
        std::cout<<"Exception Caught: "<<e.what()<<"\n";\
        throw;\
    }\
    catch(...)\
    {\
        std::cout<<"Unknow Exception Caught \n";\
        throw;\
    }\
}

#endif