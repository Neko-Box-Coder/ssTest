#include "ssTest.hpp"

//Test variables
int SomeVar = 0;

int main()
{
    //Initialize ssTest
    ssTEST_INIT();

    //(OPTIONAL) Any setup work
    ssTEST_SET_UP
    {
        SomeVar = 1;
    }

    //(OPTIONAL) Any cleanup work
    ssTEST_CLEAN_UP
    {
        SomeVar = -1;
    }

    //(OPTIONAL) Disable setup and cleanup between tests
    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    //======================================================
    //Tests
    //======================================================
    ssTEST("ssTEST_SET_UP Test")
    {
        ssTEST_OUTPUT_ASSERT(SomeVar == 1);
    }

    ssTEST("ssTEST_CALL_CLEAN_UP Test")
    {
        ssTEST_CALL_CLEAN_UP();
        ssTEST_OUTPUT_ASSERT(SomeVar == -1);
    }

    ssTEST("ssTEST_CALL_SET_UP Test")
    {
        ssTEST_CALL_SET_UP();
        ssTEST_OUTPUT_ASSERT(SomeVar == 1);
    }

    ssTEST_SKIP("ssTEST_SKIP Test")
    {
        ssTEST_OUTPUT_ASSERT(false);
    }

    ssTEST("ssTEST_OUTPUT_ASSERT Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
        ssTEST_OUTPUT_ASSERT("Optional Info", true);
    }

    ssTEST("ssTEST_OUTPUT_SKIP Test")
    {
        ssTEST_OUTPUT_SKIP();
        ssTEST_OUTPUT_SKIP("Optional info");
    }

    ssTEST_END();
}