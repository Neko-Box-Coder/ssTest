## Usage

```c++
#include "ssTest.hpp"

//Test variables
int SomeVar = 0;

bool SomeFunction(){return 1};

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

//======================================================
//Tests
//======================================================

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
    #ifdef SKIP
        ssTEST_OUTPUT_SKIP("Skipped");
    #else
        ssTEST_OUTPUT_ASSERT(SomeFunction());
    #endif
}

ssTEST_END();
```