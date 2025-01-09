## optional_value

The `optional_value` class is a template class that provides an optional value, similar to `std::optional`, but available for all C++ versions since C++11.

### Member Types

- `T`: The type of the value.
### Examples

#### Example 1: Basic Usage

```cpp
#include "optional_value.h"

int main() {
    ctle::optional_value<int> opt_val;

    // Check if the optional value has a value
    if (!opt_val.has_value()) {
        std::cout << "No value present." << std::endl;
    }

    // Set a value
    opt_val.set(42);
    if (opt_val.has_value()) {
        std::cout << "Value is now set to: " << opt_val.value() << std::endl;
    }

    return 0;
}
```

#### Example 2: Resetting and Accessing Values

```cpp
#include "optional_value.h"

int main() {
    ctle::optional_value<int> opt_val(10);

    // Access the value
    if (opt_val.has_value()) {
        std::cout << "Initial value: " << opt_val.value() << std::endl;
    }

    // Reset the value
    opt_val.reset();
    if (!opt_val.has_value()) {
        std::cout << "Value has been reset." << std::endl;
    }

    return 0;
}
```