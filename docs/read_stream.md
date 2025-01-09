## read_stream

The `read_stream` template class provides a read-only input stream designed for streaming data sequentially, using a memory buffer, while also calculating a hash on the input stream. The template can use any data source which implements a read method compatible with read_stream.

### Template Parameters

- `_DataSourceTy`: The data source type (must implement a `read` method)
- `_HashTy`: The hasher type (defaults to `hasher_noop<64>`, which is a no-op template that calculates no hash value)

### Examples

#### Basic Reading from File

```cpp
#include "read_stream.h"
#include "data_source.h"

int main()
{
    // Create a file data source 
    ctle::file_data_source source("data.bin");
    
    // Create read stream with default noop hasher
    ctle::read_stream<ctle::file_data_source> stream(source);
    
    // Read some integers
    std::vector<int> numbers(10);
    auto status = stream.read(numbers.data(), numbers.size());
    
    if (status == ctle::status::ok)
    {
        for (const auto& num : numbers)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```

### Reading with SHA-256 Hash

```cpp
#include "read_stream.h"
#include "data_source.h"
#include "hasher.h"

int main()
{
    // Create file data source
    ctle::file_data_source source("data.bin");
    
    // Create read stream with SHA-256 hasher
    ctle::read_stream<ctle::file_data_source, ctle::hasher_sha256> stream(source);
    
    // Read data in chunks
    std::vector<char> buffer(1024);
    while (!stream.has_ended())
    {
        auto status = stream.read_bytes(
            reinterpret_cast<uint8_t*>(buffer.data()), 
            buffer.size()
        );
        
        if (status != ctle::status::ok)
        {
            break;
        }
    }
    
    // Get hash after reading all data
    auto hash_result = stream.get_digest();
    if (hash_result.status == ctle::status::ok)
    {
        const auto& hash = hash_result.value;
        // Use hash...
    }
    
    return 0;
}
```

### Custom Data Source

```cpp
#include "read_stream.h"

// Custom data source
class memory_data_source
{
private:
    const std::vector<uint8_t>& data;
    size_t position = 0;

public:
    memory_data_source(const std::vector<uint8_t>& source_data) 
        : data(source_data)
    {
    }

    ctle::status_return<ctle::status, uint64_t> read(uint8_t* dest_buffer, uint64_t read_count)
    {
        if (position >= data.size())
        {
            return 0;
        }

        size_t bytes_to_read = std::min(
            read_count, 
            data.size() - position
        );
        
        std::memcpy(dest_buffer, data.data() + position, bytes_to_read);
        position += bytes_to_read;
        
        return bytes_to_read;
    }
};

int main()
{
    // Create some test data
    std::vector<uint8_t> test_data = {1, 2, 3, 4, 5};
    
    // Create memory data source
    memory_data_source source(test_data);
    
    // Create read stream
    ctle::read_stream<memory_data_source> stream(source);
    
    // Read values one by one
    while (!stream.has_ended())
    {
        uint8_t value = stream.read<uint8_t>();
        std::cout << static_cast<int>(value) << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```