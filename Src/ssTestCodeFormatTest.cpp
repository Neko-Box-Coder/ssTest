#include "./ssTest.hpp"

#include <iostream>

int main(int, char**)
{
    std::cout << "Hello World" << std::endl;

    INTERNAL_ssTEST_OUTPUT_FORMATTED_CODE
    (
        "| |    ",
        (
        //Checking mappings file path
        std::shared_ptr<OverrideResult> pathExistsResult = CreateOverrideResult();
        CO_SETUP_OVERRIDE   (OverrideInstance, Mock_exists)
                            .WhenCalledWith<const ghc::filesystem::path&, 
                                            CO_ANY_TYPE>(mappingsFilePath, CO_ANY)
                            .Returns<bool>(true)
                            .Times(1)
                            .AssignResult(pathExistsResult);
        //Open mappings file
        std::shared_ptr<OverrideResult> ifstreamResult = CreateOverrideResult();
        CO_SETUP_OVERRIDE   (OverrideInstance, Mock_ifstream)
                            .WhenCalledWith<const ghc::filesystem::path&>(mappingsFilePath)
                            .Returns<void>()
                            .Times(1)
                            .AssignResult(ifstreamResult);
        )
    );

    return 0;
}
