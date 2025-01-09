## bimap.h

The `bimap` class implements a bi-directional unordered map, providing O(1) average lookup for both key->value and value->key. This class stores both key and value directly and requires `std::hash` functors to be defined for both. 

### Caveats

Bimap is currently implemented using two maps, which duplicates the memory used. This will be changed to a shared memory layout in the future, but this future change should not affect the method signatures.

### Example Usage

```cpp
#include "bimap.h"
#include <iostream>

int main() 
{
    ctle::bimap<int, std::string> bm;

    // Insert elements
    bm.insert(1, "one");
    bm.insert(2, "two");
    bm.insert(3, "three");

    // Access elements by key
    std::cout << "Key 1 maps to value: " << bm.at(1) << std::endl;

    // Access elements by value
    std::cout << "Value 'two' maps to key: " << bm.at("two") << std::endl;

    // Check if key exists
    if (bm.contains(2)) 
	{
        std::cout << "Key 2 exists in the bimap." << std::endl;
    }

    // Check if value exists
    if (bm.contains("three")) 
	{
        std::cout << "Value 'three' exists in the bimap." << std::endl;
    }

    // Iterate over elements
    for (auto it = bm.begin(); it != bm.end(); ++it) 
	{
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }

    return 0;
}```
