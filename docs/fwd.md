## fwd.h

This header provides basic type definitions and forward declarations for the ctle library. 
### Examples

#### Using Basic Types

```cpp
#include "fwd.h"
#include <iostream>

int main()
{
    // Integer types
    ctle::i32 signed_val = -42;
    ctle::u32 unsigned_val = 42;
    
    // Floating point types
    ctle::f32 float_val = 3.14f;
    ctle::f64 double_val = 3.14159265359;
    
    std::cout << "Signed: " << signed_val << std::endl;
    std::cout << "Unsigned: " << unsigned_val << std::endl;
    std::cout << "Float: " << float_val << std::endl;
    std::cout << "Double: " << double_val << std::endl;
    
    return 0;
}
```

#### Using String Conversion

```cpp
#include "fwd.h"
#include "string_funcs.h"
#include <iostream>

int main()
{
    // Converting values to strings
    ctle::i32 int_val = 42;
    ctle::f32 float_val = 3.14f;
    
    std::string int_str = ctle::to_string(int_val);
    std::string float_str = ctle::to_string(float_val);
    
    std::cout << "Int as string: " << int_str << std::endl;
    std::cout << "Float as string: " << float_str << std::endl;
    
    // Converting strings to values
    bool success = false;
    ctle::i32 parsed_int = ctle::from_string<ctle::i32>(
        ctle::string_span<char>("42"), 
        success
    );
    
    if (success)
    {
        std::cout << "Parsed int: " << parsed_int << std::endl;
    }
    
    return 0;
}
```
