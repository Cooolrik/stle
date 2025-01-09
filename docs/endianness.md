## endianness.h

The `endianness.h` file provides functionality for converting the endianness of values. It includes functions to create values from big-endian raw data and to swap the byte order of values.

### Example Usage

#### Creating Values from Big-Endian Data

```cpp
#include "endianness.h"
#include <iostream>

int main() 
{
    uint8_t big_endian_data_16[] = {0x12, 0x34};
    uint16_t value_16 = ctle::value_from_bigendian<uint16_t>(big_endian_data_16);
    std::cout << "16-bit value: " << std::hex << value_16 << std::endl;

    uint8_t big_endian_data_32[] = {0x12, 0x34, 0x56, 0x78};
    uint32_t value_32 = ctle::value_from_bigendian<uint32_t>(big_endian_data_32);
    std::cout << "32-bit value: " << std::hex << value_32 << std::endl;

    uint8_t big_endian_data_64[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    uint64_t value_64 = ctle::value_from_bigendian<uint64_t>(big_endian_data_64);
    std::cout << "64-bit value: " << std::hex << value_64 << std::endl;

    return 0;
}
```

#### Swapping Byte Order of Values

```cpp
#include "endianness.h"
#include <iostream>

int main() 
{
    uint16_t value_16 = 0x1234;
    ctle::swap_byte_order(&value_16);
    std::cout << "Swapped 16-bit value: " << std::hex << value_16 << std::endl;

    uint32_t value_32 = 0x12345678;
    ctle::swap_byte_order(&value_32);
    std::cout << "Swapped 32-bit value: " << std::hex << value_32 << std::endl;

    uint64_t value_64 = 0x123456789ABCDEF0;
    ctle::swap_byte_order(&value_64);
    std::cout << "Swapped 64-bit value: " << std::hex << value_64 << std::endl;

    return 0;
}
```

#### Swapping Byte Order of Multiple Values

```cpp
#include "endianness.h"
#include <iostream>
#include <vector>

int main() 
{
    std::vector<uint32_t> values = {0x12345678, 0x9ABCDEF0};
    ctle::swap_byte_order(values.data(), values.size());

    for (const auto &value : values) 
	{
        std::cout << "Swapped value: " << std::hex << value << std::endl;
    }

    return 0;
}
```