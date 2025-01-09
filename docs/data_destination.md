## data_destination.h

The `file_data_destination` class provides functionality to write data to a file. Data destination objects implement a write method, and can be used for streaming data classes, e.g. write_stream 

### write() Function
To implement a data_destination class, implement the method:

```cpp
status_return<status, u64> write(const u8* src_buffer, u64 write_count)
```

The method is expected to be a blocking call which writes to the destination from a src_buffer, until the write_count bytes have been written, or an error occurs. On succes, the method must return status::ok, and the actual number of bytes written to the destination.

### Example Usage

```cpp
#include "data_destination.h"
#include <iostream>
#include <vector>

int main() 
{
    // Define the file path and create a file_data_destination object
    std::string filepath = "output.dat";
    ctle::file_data_destination file_dest(filepath);

    // Define the data to write
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0x05};

    // Write the data to the file
    auto result = file_dest.write(data.data(), data.size());

    // Check the result
    if (result.status == ctle::status::ok) 
	{
        std::cout << "Successfully wrote " << result.value << " bytes to the file." << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to write to the file." << std::endl;
    }

    return 0;
}