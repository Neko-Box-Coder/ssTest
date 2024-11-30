#include "ssTest.hpp"

#include <cassert>
#include <sstream>

#if !defined(ssTEST_NO_COLOR_OUTPUT) || !ssTEST_NO_COLOR_OUTPUT
    #ifdef assert
        #undef assert
    #endif
    
    #define assert(...) (void)(__VA_ARGS__)
#endif

//From https://stackoverflow.com/a/5419388
struct cout_redirect 
{
    cout_redirect(std::streambuf* new_buffer) : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) 
    {
        std::cout.rdbuf( old );
    }

    private:
        std::streambuf * old;
};

int ssTestInitTestGroup_ShouldUseFileNameForTestGroupName_WhenNothingIsSupplied()
{
    ssTEST_INIT_TEST_GROUP();
    ssTEST_END_TEST_GROUP();
}

int ssTestInitTestGroup_ShouldUseCustomNameForTestGroupName_WhenNameIsSupplied()
{
    ssTEST_INIT_TEST_GROUP("Test Group Name");
    ssTEST_END_TEST_GROUP();
}

int NestedTestGroup(std::string indentation)
{
    ssTEST_INIT_TEST_GROUP("Nested Test Group");
    
    ssTEST_SET_TEST_GROUP_INDENT(indentation);
    
    ssTEST("Nested Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
        ssTEST_OUTPUT_SKIP_ASSERT(true);
        ssTEST_OUTPUT_OPTIONAL_ASSERT(true);
    };
    
    ssTEST_SKIP("Nested Skip Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestTestGroup_ShouldHaveCorrectFormat_WhenNestedWithOtherTestGroups()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST("Outer Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };
    
    ssTEST("Calling Another Test Group")
    {
        ssTEST_OUTPUT_ASSERT(true);
        ssTEST_OUTPUT_ASSERT( NestedTestGroup( ssTEST_GET_NESTED_TEST_GROUP_INDENT() ) == 0 );
    };
    
    ssTEST("Outer Test 2")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestCommonSetUp_ShouldBeCalled_WhenEachTestStarts()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    int someVar = 0;
    ssTEST_COMMON_SETUP
    {
        someVar = 1;
    };
    
    ssTEST("Test")
    {
        assert(someVar == 1);
        someVar = 2;
    };
    
    ssTEST("Test2")
    {
        assert(someVar == 1);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestCommonSetup_ShouldBeCalled_WhenCalledManually()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    int someVar = 0;
    ssTEST_COMMON_SETUP
    {
        someVar = 1;
    };
    
    ssTEST("Test")
    {
        assert(someVar == 1);
        someVar = 2;
        
        ssTEST_CALL_COMMON_SETUP();
        assert(someVar == 1);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestCommonCleanUp_ShouldBeCalled_WhenEachTestEnds()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    int someVar = 0;
    ssTEST_COMMON_CLEANUP
    {
        someVar = -1;
    };
    
    ssTEST("Test")
    {
        assert(someVar == 0);
    };
    
    ssTEST("Test2")
    {
        assert(someVar == -1);
        someVar = 1;
    };
    
    ssTEST("Test3")
    {
        assert(someVar == -1);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestCommonCleanup_ShouldBeCalled_WhenCalledManually()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    int someVar = 0;
    ssTEST_COMMON_CLEANUP
    {
        someVar = -1;
    };
    
    ssTEST("Test")
    {
        assert(someVar == 0);
        someVar = 1;
        
        ssTEST_CALL_COMMON_CLEANUP();
        assert(someVar == -1);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestCommonSetupCleanup_ShouldNotBeCalled_WhenDisabledBetweenTests()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    ssTEST_DISABLE_COMMON_SETUP_CLEANUP_BETWEEN_TESTS();
    
    int someVar = 0;
    ssTEST_COMMON_SETUP
    {
        someVar = 1;
    };
    
    ssTEST_COMMON_CLEANUP
    {
        someVar = -1;
    };
    
    ssTEST("Test")
    {
        assert(someVar == 1);
        someVar = 2;
    };
    
    ssTEST("Test2")
    {
        assert(someVar == 2);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestSkip_ShouldSkipTest_WhenCalled()
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST("Test")
    {
    };
    
    ssTEST_SKIP("Test2")
    {
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestOnlyThis_ShouldOnlyRunThisTest_WhenCalled(bool& called)
{
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST("Test")
    {
        assert(false);
    };
    
    ssTEST_ONLY_THIS("Test2")
    {
        assert(true);
        called = true;
    };
    
    ssTEST("Test3")
    {
        assert(false);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestOutputAssert_ShouldAssertCorrectly_WhenAssertingExpression()
{
    int someVar = 0;
    
    auto assertPass = [&someVar](bool disableOutput, std::string indent)
    {
        ssTEST_INIT_TEST_GROUP("Assert Pass");
    
        ssTEST_SET_TEST_GROUP_INDENT(indent)
    
        if(disableOutput)
            ssTEST_DISABLE_OUTPUT_ASSERT();
    
        ssTEST_COMMON_SETUP
        {
            someVar = 0;
        };
        
        ssTEST("Test")
        {
            someVar = 1;
            ssTEST_OUTPUT_ASSERT(someVar == 1);
            ssTEST_OUTPUT_ASSERT("Test Info", someVar == 1);
            ssTEST_OUTPUT_ASSERT("", someVar, 1);
            ssTEST_OUTPUT_ASSERT("", someVar, 1, ==);
            ssTEST_OUTPUT_ASSERT("", someVar, 1 + 1, <);
        };
        
        ssTEST_END_TEST_GROUP();
    };
    (void)assertPass;
    
    auto assertFail = [&someVar](bool disableOutput, std::string indent)
    {
        ssTEST_INIT_TEST_GROUP("Assert Fail");
    
        ssTEST_SET_TEST_GROUP_INDENT(indent)
    
        if(disableOutput)
            ssTEST_DISABLE_OUTPUT_ASSERT();
    
        ssTEST_COMMON_SETUP
        {
            someVar = 0;
        };
        
        ssTEST("Test Should Fail")
        {
            someVar = 1;
            ssTEST_OUTPUT_ASSERT(someVar == 0);
            ssTEST_OUTPUT_ASSERT("Should fail", someVar == 0);
            ssTEST_OUTPUT_ASSERT("Should fail 2", someVar, 0);
            ssTEST_OUTPUT_ASSERT("Should fail 3", someVar, 0, ==);
            ssTEST_OUTPUT_ASSERT("Should fail 4", someVar, 0, <);
        };
        
        ssTEST_END_TEST_GROUP();
    };
    (void)assertFail;
    
    auto assertOptional = [&someVar](bool disableOutput, std::string indent)
    {
        ssTEST_INIT_TEST_GROUP("Assert Optional");
    
        ssTEST_SET_TEST_GROUP_INDENT(indent)
    
        if(disableOutput)
            ssTEST_DISABLE_OUTPUT_ASSERT();
    
        ssTEST_COMMON_SETUP
        {
            someVar = 0;
        };
        
        ssTEST("Test Should Pass With Optional Failed Asserts")
        {
            someVar = 1;
            ssTEST_OUTPUT_OPTIONAL_ASSERT(someVar == 0);
            ssTEST_OUTPUT_OPTIONAL_ASSERT("Should optionally fail", someVar == 0);
            ssTEST_OUTPUT_OPTIONAL_ASSERT("Should optionally pass", someVar, 1);
            ssTEST_OUTPUT_OPTIONAL_ASSERT("Should optionally pass", someVar, 1, ==);
            ssTEST_OUTPUT_OPTIONAL_ASSERT("Should optionally fail", someVar, 0, <);
        };
        
        ssTEST_END_TEST_GROUP();
    };
    (void)assertOptional;
    
    auto assertSkip = [&someVar](bool disableOutput, std::string indent)
    {
        ssTEST_INIT_TEST_GROUP("Assert Skip");
    
        ssTEST_SET_TEST_GROUP_INDENT(indent)
    
        if(disableOutput)
            ssTEST_DISABLE_OUTPUT_ASSERT();
    
        ssTEST_COMMON_SETUP
        {
            someVar = 0;
        };
        
        ssTEST("Test Should Skip All Asserts")
        {
            someVar = 1;
            ssTEST_OUTPUT_SKIP_ASSERT(someVar == 0);
            ssTEST_OUTPUT_SKIP_ASSERT("Should Skip", someVar == 0);
            ssTEST_OUTPUT_SKIP_ASSERT("Should Skip 2", someVar, 0);
            ssTEST_OUTPUT_SKIP_ASSERT("Should Skip 3", someVar, 0, ==);
            ssTEST_OUTPUT_SKIP_ASSERT("Should Skip 4", someVar, 0, <);
        };
        
        ssTEST_END_TEST_GROUP();
    };
    (void)assertSkip;
    
    
    ssTEST_INIT_TEST_GROUP("Test Asserts");
    
    ssTEST("Asserts With Output")
    {
        std::stringstream outputBuffer;
        std::string indent = ssTEST_GET_NESTED_TEST_GROUP_INDENT();
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertPass(false, indent) == 0);
            assert(outputBuffer.str().find("Assertion Passed (O)") != std::string::npos);
            assert(outputBuffer.str().find("(Test Info)") != std::string::npos);
            assert(outputBuffer.str().find("(OOO)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
        
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertFail(false, indent) != 0);
            assert(outputBuffer.str().find("Assertion Failed (X)") != std::string::npos);
            assert(outputBuffer.str().find("(Should fail)") != std::string::npos);
            assert(outputBuffer.str().find("(XXX)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
        
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertOptional(false, indent) == 0);
            assert(outputBuffer.str().find("Assertion Failed (-)") != std::string::npos);
            assert(outputBuffer.str().find("(Optional)") != std::string::npos);
            assert(outputBuffer.str().find("(Should optionally fail)") != std::string::npos);
            assert(outputBuffer.str().find("Assertion Passed (+)") != std::string::npos);
            assert(outputBuffer.str().find("(OOO)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
        
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertSkip(false, indent) == 0);
            assert(outputBuffer.str().find("Assertion Failed (-)") == std::string::npos);
            assert(outputBuffer.str().find("Assertion Skipped (/)") != std::string::npos);
            assert(outputBuffer.str().find("(Should Skip)") != std::string::npos);
            assert(outputBuffer.str().find("(OOO)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
    };
    
    ssTEST("Asserts Without Output")
    {
        std::stringstream outputBuffer;
        std::string indent = ssTEST_GET_NESTED_TEST_GROUP_INDENT();
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertPass(true, indent) == 0);
            assert(outputBuffer.str().find("Assertion Passed (O)") == std::string::npos);
            assert(outputBuffer.str().find("(Test Info)") == std::string::npos);
            assert(outputBuffer.str().find("(OOO)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
        
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertFail(true, indent) != 0);
            assert(outputBuffer.str().find("Assertion Failed (X)") != std::string::npos);
            assert(outputBuffer.str().find("(Should fail)") == std::string::npos);
            assert(outputBuffer.str().find("(XXX)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
        
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertOptional(true, indent) == 0);
            assert(outputBuffer.str().find("Assertion Failed (-)") != std::string::npos);
            assert(outputBuffer.str().find("(Optional)") != std::string::npos);
            assert(outputBuffer.str().find("(Should optionally fail)") == std::string::npos);
            assert(outputBuffer.str().find("Assertion Passed (+)") == std::string::npos);
            assert(outputBuffer.str().find("(OOO)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
        
        {
            cout_redirect cr(outputBuffer.rdbuf());
            assert(assertSkip(true, indent) == 0);
            assert(outputBuffer.str().find("Assertion Failed (-)") == std::string::npos);
            assert(outputBuffer.str().find("Assertion Skipped (/)") == std::string::npos);
            assert(outputBuffer.str().find("(Should Skip)") == std::string::npos);
            assert(outputBuffer.str().find("(OOO)") != std::string::npos);
        }
        std::cout << outputBuffer.str();
        outputBuffer.str("");
        outputBuffer.clear();
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestOutputValuesWhenFailed_ShouldOutputValues_WhenFailed()
{
    int someVar = 0;
    char someVar2 = 'a';
    std::string someVar3 = "Test Var";
    float someVar4 = 1.0f;
    
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST("Test")
    {
        someVar = 1;
        
        ssTEST_OUTPUT_ASSERT("Test", someVar == 2);
        ssTEST_OUTPUT_VALUES_WHEN_FAILED(someVar, 2, someVar2, someVar3, someVar4);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestOutputSetup_ShouldSetupCorrectly_WhenCalled()
{
    int someVar = 0;
    
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST("Test")
    {
        ssTEST_OUTPUT_SETUP
        (
            someVar = 1;
        );
        
        assert(someVar == 1);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestOutputSetup_ShouldNotOutput_WhenOutputDisabled()
{
    int someVar = 0;
    
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST_DISABLE_OUTPUT_SETUP();
    
    ssTEST("Test")
    {
        ssTEST_OUTPUT_SETUP
        (
            someVar = 1;
        );
        
        assert(someVar == 1);
    };
    
    ssTEST_END_TEST_GROUP();
}

int ssTestOutputExecution_ShouldExecuteCorrectly_WhenCalled()
{
    int someVar = 0;
    
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST("Test")
    {
        ssTEST_OUTPUT_EXECUTION
        (
            someVar = 1;
        );
        
        assert(someVar == 1);
    };
    
    ssTEST_END_TEST_GROUP();
}


int ssTestOutputExecution_ShouldNotOutput_WhenOutputDisabled()
{
    int someVar = 0;
    
    ssTEST_INIT_TEST_GROUP(__func__);
    
    ssTEST_DISABLE_OUTPUT_EXECUTION();
    
    ssTEST("Test")
    {
        ssTEST_OUTPUT_EXECUTION
        (
            someVar = 1;
        );
        
        assert(someVar == 1);
    };
    
    ssTEST_END_TEST_GROUP();
}

//TODO: Add tests for exceptions

int main()
{
    std::stringstream outputBuffer;
    {
        cout_redirect cr(outputBuffer.rdbuf());
        assert(ssTestInitTestGroup_ShouldUseFileNameForTestGroupName_WhenNothingIsSupplied() == 0);
    }
    std::cout << outputBuffer.str() << std::endl;
    assert(outputBuffer.str().find("ssTestCheck") != std::string::npos);
    
    {
        outputBuffer.str("");
        outputBuffer.clear();
        cout_redirect cr(outputBuffer.rdbuf());
        assert(ssTestInitTestGroup_ShouldUseCustomNameForTestGroupName_WhenNameIsSupplied() == 0);
    }
    std::cout << outputBuffer.str() << std::endl;
    assert(outputBuffer.str().find("Test Group Name") != std::string::npos);
    
    {
        outputBuffer.str("");
        outputBuffer.clear();
        cout_redirect cr(outputBuffer.rdbuf());
        assert(ssTestTestGroup_ShouldHaveCorrectFormat_WhenNestedWithOtherTestGroups() == 0);
    }
    std::cout << outputBuffer.str() << std::endl;
    assert(outputBuffer.str().find("| |     |") != std::string::npos);
    
    {
        assert(ssTestCommonSetUp_ShouldBeCalled_WhenEachTestStarts() == 0);
        assert(ssTestCommonSetup_ShouldBeCalled_WhenCalledManually() == 0);
    }
    
    {
        assert(ssTestCommonCleanUp_ShouldBeCalled_WhenEachTestEnds() == 0);
        assert(ssTestCommonCleanup_ShouldBeCalled_WhenCalledManually() == 0);
    }
    
    assert(ssTestCommonSetupCleanup_ShouldNotBeCalled_WhenDisabledBetweenTests() == 0);
    
    {
        outputBuffer.str("");
        outputBuffer.clear();
        cout_redirect cr(outputBuffer.rdbuf());
        assert(ssTestSkip_ShouldSkipTest_WhenCalled() == 0);
    }
    std::cout << outputBuffer.str() << std::endl;
    assert(outputBuffer.str().find("- (Skipped) \"Test2\"") != std::string::npos);
    
    {
        bool called = false;
        assert(ssTestOnlyThis_ShouldOnlyRunThisTest_WhenCalled(called) == 0);
        assert(called);
    }
    
    assert(ssTestOutputAssert_ShouldAssertCorrectly_WhenAssertingExpression() == 0);
    
    {
        {
            outputBuffer.str("");
            outputBuffer.clear();
            cout_redirect cr(outputBuffer.rdbuf());
            assert(ssTestOutputSetup_ShouldSetupCorrectly_WhenCalled() == 0);
        }
        std::cout << outputBuffer.str() << std::endl;
        assert(outputBuffer.str().find("Setting up:") != std::string::npos);
        assert(outputBuffer.str().find("someVar = 1;") != std::string::npos);
        
        {
            outputBuffer.str("");
            outputBuffer.clear();
            cout_redirect cr(outputBuffer.rdbuf());
            assert(ssTestOutputSetup_ShouldNotOutput_WhenOutputDisabled() == 0);
        }
        std::cout << outputBuffer.str() << std::endl;
        assert(outputBuffer.str().find("Setting up:") == std::string::npos);
        assert(outputBuffer.str().find("someVar = 1;") == std::string::npos);
    }
    
    {
        {
            outputBuffer.str("");
            outputBuffer.clear();
            cout_redirect cr(outputBuffer.rdbuf());
            assert(ssTestOutputExecution_ShouldExecuteCorrectly_WhenCalled() == 0);
        }
        std::cout << outputBuffer.str() << std::endl;
        assert(outputBuffer.str().find("Executing:") != std::string::npos);
        assert(outputBuffer.str().find("someVar = 1;") != std::string::npos);
        
        {
            outputBuffer.str("");
            outputBuffer.clear();
            cout_redirect cr(outputBuffer.rdbuf());
            assert(ssTestOutputExecution_ShouldNotOutput_WhenOutputDisabled() == 0);
        }
        std::cout << outputBuffer.str() << std::endl;
        assert(outputBuffer.str().find("Executing:") == std::string::npos);
        assert(outputBuffer.str().find("someVar = 1;") == std::string::npos);
    }
    
    {
        outputBuffer.str("");
        outputBuffer.clear();
        cout_redirect cr(outputBuffer.rdbuf());
        assert(ssTestOutputValuesWhenFailed_ShouldOutputValues_WhenFailed() == 1);
    }
    std::cout << outputBuffer.str() << std::endl;
    assert(outputBuffer.str().find("someVar = 1") != std::string::npos);
    assert(outputBuffer.str().find("2 = 2") != std::string::npos);
    assert(outputBuffer.str().find("someVar2 = a") != std::string::npos);
    assert(outputBuffer.str().find("someVar3 = Test Var") != std::string::npos);
    assert(outputBuffer.str().find("someVar4 = 1") != std::string::npos);
    
    std::cout << "Everything passed" << std::endl;
    
    return 0;
}
