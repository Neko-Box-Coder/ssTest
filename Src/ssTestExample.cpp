#include "ssTest.hpp"

#include <cstring>
#include <unordered_map>

//An Example Class To Test
typedef struct
{
    int PageSize;
    int PageCount;
    short* AllocateID_Table;
    int NextAllocateID;
    char* Memory;
} MyMemoryAllocator;

MyMemoryAllocator MyMemoryAllocator_Create(size_t pageSize, size_t pageCount)
{
    MyMemoryAllocator Allocator;
    
    if(pageSize * pageCount > 1024 * 1024 * 5) //5MB
    {
        Allocator.PageSize = 0;
        Allocator.PageCount = 0;
        Allocator.AllocateID_Table = NULL;
        Allocator.NextAllocateID = 1;
        Allocator.Memory = NULL;
        
        return Allocator;
    }
    
    Allocator.PageSize = pageSize;
    Allocator.PageCount = pageCount;
    Allocator.NextAllocateID = 1;
    Allocator.Memory = (char*)malloc(pageSize * pageCount);
    Allocator.AllocateID_Table = (short*)malloc(pageCount * sizeof(short));
    
    memset(Allocator.AllocateID_Table, 0, pageCount * sizeof(short));

    return Allocator;
}

MyMemoryAllocator MyMemoryAllocator_CreateShared(void* sharedPointer, size_t pageSize, size_t pageCount)
{
    MyMemoryAllocator Allocator;
    Allocator.PageSize = pageSize;
    Allocator.PageCount = pageCount;
    Allocator.AllocateID_Table = (short*)sharedPointer;
    Allocator.NextAllocateID = 1;
    Allocator.Memory = (char*)sharedPointer + pageCount * sizeof(short);
    
    memset(Allocator.AllocateID_Table, 0, pageCount * sizeof(short));
    
    return Allocator;
}


bool MyMemoryAllocator_Destroy(MyMemoryAllocator* allocator)
{
    if(allocator->Memory == NULL)
        return false;
    
    if(allocator->Memory != (char*)allocator->AllocateID_Table + allocator->PageCount * sizeof(short))
    {
        free(allocator->AllocateID_Table);
        free(allocator->Memory);
    }
    
    allocator->PageSize = 0;
    allocator->PageCount = 0;
    allocator->AllocateID_Table = NULL;
    allocator->NextAllocateID = 0;
    allocator->Memory = NULL;
    
    return true;
}

void* MyMemoryAllocator_Allocate(MyMemoryAllocator* allocator, size_t size)
{
    if(allocator->Memory == NULL)
        return NULL;
    
    int requiredPageCount = (size + allocator->PageSize - 1) / allocator->PageSize;
    
    int currentStartPage = -1;
    for(int i = 0; i < allocator->PageCount; i++)
    {
        if(allocator->AllocateID_Table[i] != 0)
        {
            currentStartPage = -1;
            continue;
        }
        
        if(currentStartPage == -1)
            currentStartPage = i;
        
        if(i - currentStartPage + 1 == requiredPageCount)
            break;
    }
    
    if(currentStartPage == -1)
        return NULL;
    
    //std::cout << "allocator->PageCount: " << allocator->PageCount << std::endl;
    
    for(int i = currentStartPage; i < currentStartPage + requiredPageCount; i++)
    {
        //std::cout << "i: " << i << std::endl;
        allocator->AllocateID_Table[i] = allocator->NextAllocateID;
    }
    
    ++allocator->NextAllocateID;
    return allocator->Memory + currentStartPage * allocator->PageSize;
}

bool MyMemoryAllocator_Free(MyMemoryAllocator* allocator, void* memory)
{
    if(allocator->Memory == NULL)
        return false;
    
    if( memory < allocator->Memory || 
        memory >= allocator->Memory + allocator->PageSize * allocator->PageCount)
    {
        return false;
    }
    
    int page = ((char*)memory - allocator->Memory) / allocator->PageSize;
    short allocateID = allocator->AllocateID_Table[page];
    
    for(int i = page; i < allocator->PageCount; i++)
    {
        if(allocator->AllocateID_Table[i] != allocateID)
            break;
        
        allocator->AllocateID_Table[i] = 0;
    }
    
    return true;
}

int main()
{
    MyMemoryAllocator testAllocator;
    
    ssTEST_INIT_TEST_GROUP();

    ssTEST_COMMON_SET_UP
    {
        testAllocator = MyMemoryAllocator_Create(64, 12);
    };

    ssTEST_COMMON_CLEAN_UP
    {
        MyMemoryAllocator_Destroy(&testAllocator);
    };

    ssTEST("MyMemoryAllocator_Create Should Not Crash When Requesting Zero Memory")
    {
        //We want to create our own memory allocator and request empty memory
        ssTEST_CALL_COMMON_CLEANUP();
        
        ssTEST_OUTPUT_EXECUTION
        (
            testAllocator = MyMemoryAllocator_Create(64, 0);
        );
        
        ssTEST_OUTPUT_ASSERT(testAllocator.PageSize == 64);
        ssTEST_OUTPUT_ASSERT(testAllocator.PageCount == 0);
    };
    
    ssTEST("MyMemoryAllocator_CreateShared Should Create Shared Memory Allocator")
    {
        ssTEST_OUTPUT_SETUP
        (
            const int pageSize = 64;
            const int pageCount = 4;
            MyMemoryAllocator sharedAllocator;
            void* sharedMemory = MyMemoryAllocator_Allocate(&testAllocator, 
                                                            pageSize * pageCount + 
                                                            pageCount * sizeof(short));
        );
        
        ssTEST_OUTPUT_EXECUTION
        (
            sharedAllocator = MyMemoryAllocator_CreateShared(sharedMemory, pageSize, pageCount);
        );
        
        ssTEST_OUTPUT_ASSERT(sharedAllocator.AllocateID_Table == (short*)sharedMemory);
        ssTEST_OUTPUT_ASSERT(sharedAllocator.Memory == (char*)sharedMemory + pageCount * sizeof(short));
        ssTEST_OUTPUT_ASSERT(sharedAllocator.PageSize == pageSize);
        ssTEST_OUTPUT_ASSERT(sharedAllocator.PageCount == pageCount);
        
        MyMemoryAllocator_Destroy(&sharedAllocator);
    };

    ssTEST("MyMemoryAllocator_Allocate Should Allocate Memory When Enough Memory Is Available")
    {
        ssTEST_OUTPUT_SETUP
        (
            const int pageSize = 64;
            char* memory;
            int pageCount = 4;
        );
        
        for(int i = 0; i < 2; i++)
        {
            ssTEST_OUTPUT_EXECUTION
            (
                memory = (char*)MyMemoryAllocator_Allocate(&testAllocator, pageSize * pageCount);
            );
        }
        
        ssTEST_OUTPUT_EXECUTION
        (
            *memory = 17;
            *(memory + pageSize * pageCount - 1) = 17;
        );
        
        //This also works without a message as well
        ssTEST_OUTPUT_ASSERT("Memory Result", memory != NULL);
        
        //Asserting by passing two values (to compare equality) will also print the values
        ssTEST_OUTPUT_ASSERT("Reading to beginning of memory", *memory == 17);
        
        //You can also specify the operator to use for comparison
        ssTEST_OUTPUT_ASSERT(   "Reading to end of memory", 
                                *(memory + pageSize * pageCount - 1), 17, ==);
    };
    
    ssTEST("MyMemoryAllocator_Allocate Should Return NULL When Not Enough Memory Is Available")
    {
        ssTEST_OUTPUT_SETUP
        (
            const int pageSize = 64;
        );
        
        //Let's say this is not implemented or we know it is crashing, we can skip the assert 
        //or even skip the whole test with ssTEST_SKIP
        ssTEST_OUTPUT_SKIP_ASSERT(  "Allocation Result", 
                                    (char*)MyMemoryAllocator_Allocate(  &testAllocator, 
                                                                        pageSize * 250) == NULL);
    };
    
    ssTEST("MyMemoryAllocator_Allocate Should Return NULL When Zero Size Is Passed In")
    {
        ssTEST_OUTPUT_SETUP
        (
            char* memory;
        );
        (void)memory;
        
        ssTEST_OUTPUT_EXECUTION
        (
            memory = (char*)MyMemoryAllocator_Allocate(&testAllocator, 0);
        );
        
        //Let's say there's a bug in the allocator which will probably fail the assert, 
        //but we still want to know the result. An optional assert can be used in this case.
        ssTEST_OUTPUT_OPTIONAL_ASSERT("Allocation Result", memory == NULL);
    };

    //etc...

    ssTEST_END_TEST_GROUP();
}