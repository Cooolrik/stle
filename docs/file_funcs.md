## file_funcs.h

The `file_funcs.h` file provides various file handling functions and classes. It includes functions to check file existence, access files, read files into a vector, and write files from a pointer or container. Additionally, it defines the `_file_object` class for encapsulating file operations.

### Example Usage

#### Checking File Existence

```cpp
#include "file_funcs.h"
#include <iostream>

int main() {
    std::string filepath = "example.txt";

    if (ctle::file_exists(filepath)) 
	{
        std::cout << "File exists." << std::endl;
    } 
	else 
	{
        std::cout << "File does not exist." << std::endl;
    }

    return 0;
}
```

#### Reading a File into a Vector

```cpp
#include "file_funcs.h"
#include <iostream>
#include <vector>

int main() {
    std::string filepath = "example.txt";
    std::vector<uint8_t> data;

    ctle::status result = ctle::read_file(filepath, data);

    if (result == ctle::status::ok) 
	{
        std::cout << "File read successfully."
				  << "Data size: " << data.size() << " bytes." 
				  << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to read the file." << std::endl;
    }

    return 0;
}
```

#### Writing a File from a Vector

```cpp
#include "file_funcs.h"
#include <iostream>
#include <vector>

int main() {
    std::string filepath = "output.txt";
    std::vector<uint8_t> data = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII

    ctle::status result = ctle::write_file(filepath, data);

    if (result == ctle::status::ok) 
	{
        std::cout << "File written successfully." << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to write the file." << std::endl;
    }

    return 0;
}
```

#### Using `_file_object` to Open a File for Reading

```cpp
#include "file_funcs.h"
#include <iostream>

int main() {
    std::string filepath = "example.txt";
    ctle::_file_object file;

    ctle::status result = file.open_read(filepath);

    if (result == ctle::status::ok) 
	{
        std::cout << "File opened successfully." << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to open the file." << std::endl;
    }

    return 0;
}
```