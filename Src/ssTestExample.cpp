#include "ssTest.hpp"

int SomeVar = 0;    //Test variables

bool SomeFunction(){return SomeVar == 1;};

ssTEST_INIT();      //Initialize ssTest

ssTEST_SET_UP       //(OPTIONAL) Any setup work
{
    SomeVar = 1;
}

ssTEST_CLEAN_UP     //(OPTIONAL) Any cleanup work
{
    SomeVar = -1;
}

//ssTEST_SET_UP is called beforing running any tests

ssTEST("SomeFunction Test")
{
    ssTEST_OUTPUT_ASSERT(SomeFunction() == true);
}

ssTEST("Calling CleanUp and SetUp manually")
{
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
}

ssTEST_SKIP("Skipping A Test")
{
    ssTEST_OUTPUT_ASSERT(SomeFunction());
}

ssTEST("Asserting with extra info")
{
    ssTEST_OUTPUT_ASSERT(true);
    ssTEST_OUTPUT_ASSERT("Optional Info", true);
}

ssTEST("Skipping certain assertion")
{
    #define SKIP
    #ifdef SKIP
        ssTEST_OUTPUT_SKIP("Skipped");
    #else
        ssTEST_OUTPUT_ASSERT(SomeFunction());
    #endif
}

//ssTEST_CLEAN_UP is called beforing running any tests

ssTEST_END();