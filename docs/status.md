## status

The `status` class provides a unified error handling system that maps various error codes from different sources (STL, Vulkan, etc.) into a single enumeration.

### Features

- Unified error codes from multiple sources
- Automatic conversion from STL error codes (`std::errc`)
- Automatic conversion from Vulkan error codes (`VkResult`)
- String descriptions for all error codes
- Boolean operator for quick success/failure checks
- Stream output operator support

### Examples

#### Basic Usage

```cpp
#include "status.h"
#include <iostream>

void function_that_might_fail()
{
    // Return error status
    return ctle::status::cant_allocate;
}

int main()
{
    ctle::status result = function_that_might_fail();
    
    if (!result)
    {
        std::cout << "Error occurred: " << result << std::endl;
        // Prints: Error occurred: cant_allocate ("cant allocate memory")
    }
    
    return 0;
}
```

#### Converting from STL Errors

```cpp
#include "status.h"
#include <system_error>
#include <iostream>

int main()
{
    // Convert from std::errc
    ctle::status result = std::errc::no_such_file_or_directory;
    
    if (!result)
    {
        std::cout << "STL error: " << result << std::endl;
        // Prints: STL error: stl_no_such_file_or_directory ("stl error code: errc::no_such_file_or_directory")
    }
    
    return 0;
}
```

#### Using with Vulkan

```cpp
#include "status.h"
#include <vulkan/vulkan.h>
#include <iostream>

VkResult vulkan_function_which_fails()
{
	return VK_ERROR_OUT_OF_HOST_MEMORY;
}

int main()
{
    ctle::status result = vulkan_function_which_fails();
    
    if (!result)
    {
        std::cout << "Vulkan error: " << result << std::endl;
        // Prints: Vulkan error: vulkan_out_of_host_memory ("Vulkan error code VkResult::VK_ERROR_OUT_OF_HOST_MEMORY")
    }
    
    return 0;
}
```

#### Error Handling Pattern

```cpp
#include "status.h"
#include <iostream>

ctle::status operation1()
{
    // Simulated failure
    return ctle::status::cant_open;
}

ctle::status operation2()
{
    // Check result of operation1
    ctle::status result = operation1();
    if (!result)
    {
        return result;  // Propagate error
    }
    
    return ctle::status::ok;
}

int main()
{
    ctle::status result = operation2();
    
    if (!result)
    {
        std::cerr << "Operation failed: " << result << std::endl;
        return -1;
    }
    
    std::cout << "Operation succeeded" << std::endl;
    return 0;
}
```