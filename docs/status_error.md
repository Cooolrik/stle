## status_error

The `status_error` class is an exception class derived from `std::runtime_error` that includes a `status` value. This allows for throwing exceptions with specific status codes.

### Examples

#### Basic Usage

```cpp
#include "status_error.h"
#include <iostream>

void process_data()
{
    // Simulate an error condition
    bool error_condition = true;
    
    if (error_condition)
    {
        throw ctle::status_error(ctle::status::invalid_argument, "Invalid data provided");
    }
}

int main()
{
    try
    {
        process_data();
    }
    catch (const ctle::status_error& e)
    {
        std::cout << "Error status: " << static_cast<int>(e.value) << std::endl;
        std::cout << "Error message: " << e.what() << std::endl;
    }
    
    return 0;
}
```
