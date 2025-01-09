## hasher.h

The `hasher.h` file provides various hasher classes for generating hash values. It includes a no-operation hasher, a SHA-256 hasher, and XXH3 XXH64/128 hashers.

The classes provide declarations of the different hashers, but the user needs to add an implementation of the hashing code. See [ctle.h](#ctle.h) for details.

### Example Usage

```cpp
#include "hasher.h"
#include <iostream>

int main() 
{
	// Uncomment a line to select that hasher and hash size
    // SHA-256:
	ctle::hasher_sha256 hasher;
    
	// XXH 64 or 128
	//ctle::hasher_xxh64 hasher;
    //ctle::hasher_xxh128 hasher;

	// no-operation hashers (just 0 hashes) 64-512 bits sizes:
    //ctle::hasher_noop<64> hasher;
    //ctle::hasher_noop<128> hasher;
    //ctle::hasher_noop<256> hasher;
    //ctle::hasher_noop<512> hasher;

	// update the hash with 3 bytes
    uint8_t data[] = {0x01, 0x02, 0x03};
    hasher.update(data, sizeof(data));

	// retrieve the resulting hash value 
    auto result = hasher.finish();
    if (result.status == ctle::status::ok) 
	{
        std::cout << "No-op hash generated successfully." << std::endl;
        auto hash_value = result.value;
        std::cout << "Hash value: ";
        for (size_t i = 0; i < sizeof(hash_value.data); ++i) 
		{
            std::cout << std::hex << static_cast<int>(hash_value.data[i]);
        }
        std::cout << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to generate no-op hash." << std::endl;
    }

    return 0;
}
```