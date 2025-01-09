## optional_idx_vector

The `optional_idx_vector` class is a template class that provides an optional `idx_vector`. It allows for the storage of a vector that may or may not have a value.

### Template Parameters

- `_Ty`: The type of the elements in the vector.
- `_IdxTy`: The type of the index vector. Defaults to `std::vector<std::int32_t>`.
- `_VecTy`: The type of the values vector. Defaults to `std::vector<_Ty>`.

### Examples

#### Example 1: Basic Usage

```cpp
#include "optional_idx_vector.h"

int main() {
    ctle::optional_idx_vector<int> opt_vec;

    // Check if the optional vector has a value
    if (!opt_vec.has_value()) {
        std::cout << "No value present." << std::endl;
    }

    // Set a value vector
    opt_vec.set();
    if (opt_vec.has_value()) {
        std::cout << "Value is now set." << std::endl;
    }

    return 0;
}
```

#### Example 2: Setting and Accessing Values

```cpp
#include "optional_idx_vector.h"

int main() {
    ctle::optional_idx_vector<int> opt_vec;

    // Create values and index vectors
    std::vector<int> values = {1, 2, 3};
    std::vector<std::int32_t> index = {0, 1, 2};

    // Set the optional vector with values and index
    opt_vec.set(values, index);

    // Access the values and index
    if (opt_vec.has_value()) {
        const auto &vals = opt_vec.values();
        const auto &idx = opt_vec.index();

        for (size_t i = 0; i < vals.size(); ++i) {
            std::cout << "Value: " << vals[i] << ", Index: " << idx[i] << std::endl;
        }
    }

    return 0;
}
```