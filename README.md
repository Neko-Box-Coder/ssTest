# 🧪 ssTest

An easy to use, flexible, single header, C++11 testing framework.

A testing framework is literally just a fancy way of printing asserts results. 
The last thing you want is to spend so much time trying to figure out how to import and use a testing framework.

## 📦️ Installation

Just include `ssTest.hpp` into your project

## 📖 Documentations

### 😶‍🌫️ Concepts
In ssTest, **asserts** (`ssTEST_OUTPUT_ASSERT()`) are grouped into **test** (`ssTEST()`), 
**tests** are grouped into **test group** (`ssTEST_INIT_TEST_GROUP()`).

You can also name each assert, test and test group however you want.

Test groups are also nestable so you can have a hierarchy of tests.

<details> 

<summary>So you can have a simple structure like this (Expandable)</summary>

```text
- Test Group: My App
    - Test: Feature A Scenario 1
        - Assert: Action 1
        - Assert: Action 2
    - Test: Feature A Scenario 2
        - Assert: Action 1
        - Assert: Action 2
```

</details>

<details> 

<summary>Or a more complex structure like this (Expandable)</summary>

```text
- Test Group: My App
    - Test: Feature A
        - Test Group: Feature A
            - Test: Feature A Scenario 1
                - Assert: Action 1
                - Assert: Action 2
            - Test: Feature A Scenario 2
                - Assert: Action 1
                - Assert: Action 2
    
    - Test: Feature B
        - Test Group: Feature B
            - Test: Feature B Scenario 1
                - Assert: Action 1
                - Assert: Action 2
            - Test: Feature B Scenario 2
                - Assert: Action 1
                - Assert: Action 2
```

</details>

Each test in a test group can optionally share a **common setup** (`ssTEST_COMMON_SET_UP()`) and 
a **common cleanup** function (`ssTEST_COMMON_CLEAN_UP()`).

### 📂 Test Group

Each test group must be inside a **function that returns an `int`**.

If that test group is successful, it will **return 0**. Otherwise, it will return **non 0**.

It is common to put a single test group in a single `.cpp` file inside the `int main()` function
but you can do any other way you see fit.

#### Initializing A Test Group (Required)
- `ssTEST_INIT_TEST_GROUP();` : Initialize a test group with the name of the current file
- `ssTEST_INIT_TEST_GROUP("testGroupName");`: Initialize a test group with `testGroupName`.

#### Common Setup And Cleanup
> These should be called before declaring the first test (`ssTEST()`) in the test group

- `ssTEST_COMMON_SET_UP(){...};`: A common setup function that is called before each test in the test group
- `ssTEST_COMMON_CLEAN_UP(){...};`: A common cleanup function that is called after each test in the test group
- `ssTEST_DISABLE_COMMON_SETUP_CLEANUP_BETWEEN_TESTS();`: Disable calling the common setup and cleanup functions **between** tests. 
But still be called at the beginning and end of the test group.

#### Ending A Test Group (Required)
- `ssTEST_END_TEST_GROUP();`: Finishes the declarations of all the tests, executes them and outputs the results.

A short example of a test group

```c++
int main()
{
    int testVar = 0;
    int cleanUpCount = 0;
    
    ssTEST_INIT_TEST_GROUP();
    
    ssTEST_COMMON_SET_UP(){ testVar = 1;};
    ssTEST_COMMON_CLEAN_UP(){ cleanUpCount++; };
    //ssTEST_DISABLE_COMMON_SETUP_CLEANUP_BETWEEN_TESTS();
    
    ssTEST("A Normal Test")
    {
        ssTEST_OUTPUT_ASSERT(testVar == 1);
        testVar = 5;
    };
    
    ssTEST("Another Normal Test")
    {
        ssTEST_OUTPUT_ASSERT(testVar == 1);
        ssTEST_OUTPUT_ASSERT(cleanUpCount == 1);
        
        //If ssTEST_DISABLE_COMMON_SETUP_CLEANUP_BETWEEN_TESTS() is called, 
        //  testVar would be 5 and cleanUpCount would be 0
    };

    ssTEST_END_TEST_GROUP();
}
```

#### Nesting Test Groups (Advanced)

<details>

Just like a normal test group, you just need to call `ssTEST_INIT_TEST_GROUP()` 
inside a function that is called from the outer test group.

However, the (optional) extra step you need to do to make it format well is to pass the indentation
string from the outer test group to the inner test group.

> These should be called before declaring the first test (`ssTEST()`) in the test group
- `ssTEST_GET_NESTED_TEST_GROUP_INDENT();`: Gets the indentation string for nested test group
- `ssTEST_SET_TEST_GROUP_INDENT("indent");`: Sets the indentation level using the string `indent`

Ax example would be

```c++
int AssertGroupA(std::string outerIndent)
{
    ssTEST_INIT_TEST_GROUP("Test Group A");
    ssTEST_SET_TEST_GROUP_INDENT(outerIndent);
    
    ssTEST("A Normal Test In Group A")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };

    ssTEST_END_TEST_GROUP();
}

int main()
{
    ssTEST_INIT_TEST_GROUP("Root Test Group");
    ssTEST("A Normal Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };

    ssTEST("Calling A Nested Test Group")
    {
        ssTEST_OUTPUT_ASSERT( AssertGroupA( ssTEST_GET_NESTED_TEST_GROUP_INDENT() ) == 0 );
    };

    ssTEST_END_TEST_GROUP();
}
```

</details>

#### Disable Output For Certain Actions Inside A Test (Advanced)
<details>

> These should be called before declaring the first test (`ssTEST()`) in the test group
- `ssTEST_DISABLE_OUTPUT_SETUP();`: Disable outputting to the console for the test setup action
- `ssTEST_DISABLE_OUTPUT_EXECUTION();`: Disable outputting to the console for the test execution action
- `ssTEST_DISABLE_OUTPUT_ASSERT();`: Disable outputting to the console for the test assert action.
If the assert fails however, it will still output associated line that failed the assert.

</details>

### 🧪 Test

- `ssTEST("testName"){...};`: Creates a test with `testName`
- `ssTEST_ONLY_THIS("testName"){...};`: Creates a test with `testName` and only runs this test, 
great when debugging a single test.
- `ssTEST_SKIP("testName"){...};`: Creates a test with `testName` and skips it

Example:

```c++
int main()
{
    ssTEST_INIT_TEST_GROUP();
    
    ssTEST("A Normal Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };
    
    ssTEST_ONLY_THIS("Will Only Run This Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };
    
    ssTEST_SKIP("Will Skip This Test")
    {
        ssTEST_OUTPUT_ASSERT(true);
    };
    
    ssTEST_END_TEST_GROUP();
}
```

### 👊 Actions And Assertions Inside A Test

#### Required Assertions
- `ssTEST_OUTPUT_ASSERT(assertionStatement);`: Assert and output the assertion statement.
- `ssTEST_OUTPUT_ASSERT("info", assertionStatement);`: Assert and output the assertion statement with info.
- `ssTEST_OUTPUT_ASSERT("info", assertValue, expectedValue);`: Assert if the `assertValue` and `expectedValue` are 
equal and output result with `info`.
- `ssTEST_OUTPUT_ASSERT("info", assertValue, expectedValue, operator);`: Assert if the `assertValue` 
and `expectedValue` returns true when compared with the `operator` and output result with `info`.

#### Optional Assertions
- `ssTEST_OUTPUT_OPTIONAL_ASSERT(...);`: Execution the assertions with arguments same as "Required Assertions"
but failure of the assertion will **not** fail the test.

#### Skipping Assertions
- `ssTEST_OUTPUT_SKIP_ASSERT(...)`: **Skip** the assertion with arguments same as "Required Assertions".

#### Test Setup and Execution
>❗️ If using macro inside these action fails to compile, simple wrap them in a parenthesis instead.

- `ssTEST_OUTPUT_SETUP(...);`: Executes and output the setup statements.
- `ssTEST_OUTPUT_EXECUTION(...);`: Executes and output the execution **statement(s)**.

#### Outputting Values (Up to 5 values) When An Assertion Failed
- `ssTEST_OUTPUT_VALUES_WHEN_FAILED(comma, separated, values,...);`: Output the values of the variables when the assertion failed.

#### Calling Common Setup And Cleanup Manually (Advanced)
- `ssTEST_CALL_COMMON_SETUP();`: Calls the common setup function manually
- `ssTEST_CALL_COMMON_CLEANUP();`: Calls the common cleanup function manually

### 📊 Tests And Assert Results

When all the tests have finished running, you can quickly find the ones that failed by searching
for certain keywords from different types of result output.

#### Assertions
- Required Assertions:
    - `Assertion Passed (O)` when passed
    - `Assertion Failed (X)` when failed
    - `Error Caught (X) ...` when an error is caught
- Optional Assertions:
    - `Assertion Passed (+)` when passed
    - `Assertion Failed (-)` when failed
    - `Error Caught (-) ...` when an error is caught
- Skipped Assertions:
    - `Assertion Skipped (/)` when skipped

#### Test
- `...passed all tests (OOO)` when all assertions passed
- `...failed some tests (XXX)` when some assertions failed
- `...Error Caught (XXX)...` when an error is caught

Notice each result has a symbol inside the parentheses, which makes it easier to identify the result.

You can easily capture the symbol inside the parentheses to quickly output the related failures in CI.
For example, if you want the context of all failed asserts, you can quickly do
```bash
cat testResults.txt | grep -B15 "(X)"
```

Or in powershell
```powershell
Select-String -path .\testResult.txt -Pattern "\(X\)" -Context 15,0
```

### 📝 Example
