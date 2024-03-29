## Usage

Just include the header and then

```c++
#include "ssTest.hpp"

int SomeVar = 0;    //Test variables

bool SomeFunction(){return SomeVar == 1;};

int main()
{
    ssTEST_INIT();      //Initialize ssTest

    ssTEST_SET_UP       //(OPTIONAL) Any setup work
    {
        SomeVar = 1;
    };

    ssTEST_CLEAN_UP     //(OPTIONAL) Any cleanup work
    {
        SomeVar = -1;
    };

    //ssTEST_SET_UP and ssTEST_CLEAN_UP is called between tests
    //Unless ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS(); is called;

    ssTEST("SomeFunction Test")
    {
        ssTEST_OUTPUT_ASSERT(SomeFunction() == true);
    };

    ssTEST("Calling SetUp and cleanup manually")
    {
        ssTEST_CALL_SET_UP();
        ssTEST_CALL_CLEAN_UP();
    };

    ssTEST_SKIP("Skipping A Test")
    {
        ssTEST_OUTPUT_ASSERT(SomeFunction());
    };

    ssTEST("Asserting with extra info")
    {
        ssTEST_OUTPUT_ASSERT(true);
        ssTEST_OUTPUT_ASSERT("Optional Info", true);
    };

    ssTEST("Skipping certain assertion")
    {
        #define SKIP
        #ifdef SKIP
            ssTEST_OUTPUT_SKIP("Skipped");
        #else
            ssTEST_OUTPUT_ASSERT(SomeFunction());
        #endif
    };

    ssTEST_END();
}
```

![](screenshot.png)

`⚠️ On Linux, you need to install an emoji font such as NotoColorEmoji.ttf in order for emoji to work`
`⚠️ On Windows, you need to use powershell (ctrl+shift+right-click) or terminal in order to show emoji`