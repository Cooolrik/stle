## idx_vector.h

The `idx_vector.h` file provides the `idx_vector` class template, which is a vector of values with an index vector into the values. This allows for efficient indexing and manipulation of the values using the index vector.

### Example Usage

```cpp
#include "idx_vector.h"
#include <iostream>
#include <vector>

int main() 
{
    // Define an idx_vector with int values and default index and values vector types
    ctle::idx_vector<int> iv;

    // Add some values to the values vector
    iv.values_m.push_back(10);
    iv.values_m.push_back(20);
    iv.values_m.push_back(30);

    // Add some indices to the index vector
    iv.index_m.push_back(2);
    iv.index_m.push_back(0);
    iv.index_m.push_back(1);

    // Access values using the index vector
    for (const auto &index : iv.index_m) 
	{
        std::cout << "Value at index " << index << ": " << iv.values_m[index] << std::endl;
    }

    return 0;
}
```

