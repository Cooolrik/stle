## optional_vector

The `optional_vector` class is a template class that wraps a `std::vector` with an optional flag, similar to `std::optional` but specifically for vectors.

### Examples

#### Basic Usage

```cpp
#include "optional_vector.h"
#include <iostream>

int main()
{
    // Create an empty optional vector
    ctle::optional_vector<int> opt_vec;
    
    // Check if it has a value
    if (!opt_vec.has_value())
    {
        std::cout << "Vector is not allocated\n";
    }
    
    // Set it to an empty but allocated vector
    opt_vec.set();
    
    if (opt_vec.has_value())
    {
        std::cout << "Vector is now allocated with size: " << opt_vec.values().size() << "\n";
    }
    
    return 0;
}
```

#### Setting and Accessing Values

```cpp
#include "optional_vector.h"
#include <iostream>

int main()
{
    ctle::optional_vector<int> opt_vec;
    
    // Initialize with values
    std::vector<int> initial_values = {1, 2, 3, 4, 5};
    opt_vec.set(initial_values);
    
    // Access values
    if (opt_vec.has_value())
    {
        for (const auto& val : opt_vec.values())
        {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
    // Reset the vector
    opt_vec.reset();
    
    if (!opt_vec.has_value())
    {
        std::cout << "Vector has been reset\n";
    }
    
    return 0;
}
```

#### Using with Custom Types

```cpp
#include "optional_vector.h"
#include <iostream>
#include <string>

struct CustomType 
{
    std::string name;
    int value;
    
    CustomType(const std::string& n, int v) : name(n), value(v) {}
};

int main()
{
    ctle::optional_vector<CustomType> opt_vec;
    
    // Create and set some custom values
    std::vector<CustomType> custom_values;
    custom_values.emplace_back("first", 1);
    custom_values.emplace_back("second", 2);
    
    opt_vec.set(custom_values);
    
    // Access the values
    if (opt_vec.has_value())
    {
        for (const auto& item : opt_vec.values())
        {
            std::cout << "Name: " << item.name 
                      << ", Value: " << item.value << "\n";
        }
    }
    
    return 0;
}
```