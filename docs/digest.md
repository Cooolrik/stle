## digest.h

The `digest.h` file provides the digest<> template structure for hash or checksum digests of various sizes (64, 128, 256, and 512 bits). It includes comparison operators for comparing digest values.

### `template<size_t _Size> struct digest`

A digest structure defined for 64, 128, 256, and 512 bits. Attempting to use other sizes will result in a static assertion failure.
The values are stored in big-endian format, so the most significant byte is at index 0, and the least significant byte is at the last index.
The comparison operators (<, ==, !=) allow for comparing digests.

### Example Usage

#### Creating and Comparing Digest

```cpp
#include "digest.h"
#include <iostream>

int main() 
{
    // Create two 256-bit digests
    ctle::digest<256> digest1;
    ctle::digest<256> digest2;

    // Set some data in the digestes
    digest1.data[0] = 0x01;
    digest2.data[0] = 0x02;

    // Compare the digestes
    if (digest1 < digest2) 
	{
        std::cout << "digest1 is less than digest2" << std::endl;
    } 
	else if (digest1 == digest2) 
	{
        std::cout << "digest1 is equal to digest2" << std::endl;
    } 
	else 
	{
        std::cout << "digest1 is greater than digest2" << std::endl;
    }

    return 0;
}
```

#### Using Different digest Sizes

```cpp
#include "digest.h"
#include <iostream>

int main() 
{
    // Create a 64-bit digest, assign 0x01 to the most significant byte
    ctle::digest<64> digest64;
    digest64.data[0] = 0x01;

    // Create a 128-bit digest, assign 0x01 to the most significant byte
    ctle::digest<128> digest128;
    digest128.data[0] = 0x01;

    // Create a 512-bit digest, assign 0x01 to the most significant byte
    ctle::digest<512> digest512;
    digest512.data[0] = 0x01;

    // Print the sizes of the digestes
    std::cout << "Size of 64-bit digest: " << sizeof(digest64) << " bytes" << std::endl;
    std::cout << "Size of 128-bit digest: " << sizeof(digest128) << " bytes" << std::endl;
    std::cout << "Size of 512-bit digest: " << sizeof(digest512) << " bytes" << std::endl;

    return 0;
}
```