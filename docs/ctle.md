# ctle.h

The `ctle.h` file is a single header file that includes all the components of the `ctle` library. It can be used in a source file that defines `CTLE_IMPLEMENTATION` to include the entire library's source code. This file simplifies the inclusion of the `ctle` library in your projects by providing a single point of inclusion. If you instead want to only include a select subset of the files, include these files individually instead of including `ctle.h`.

## Usage

To use some parts of the `ctle` library, you need to define `CTLE_IMPLEMENTATION` in one of your source files, and either include `ctle.h` or the specific headers to include of the `ctle` library. You can optionally also include headers of other libraries that may be used by `ctle` to add more functionality. Examples of this is including Vulkan to add support for Vulkan error codes in `status.h`, or include `picosha2.h` to add support for SHA-256 hashing in the `hasher.h` hashing classes.

### Example Implementation

```cpp
//
// library_implement.cpp - example implementation of the ctle library
//

// Define CTLE_IMPLEMENTATION in this file only, this will compile the source code
#define CTLE_IMPLEMENTATION

// Optionally, include headers of other libraries which may be used by ctle.
// Including these will add more functionality to ctle
// NOTE: Make sure to include these before including ctle.h in this file, as 
// these are needed by the ctle code, and ctle will not include the files automatically.
#include <vulkan/vulkan.h>   // Convert Vulkan errors to status errors
#include <system_error>      // Convert system errors to status errors
#include <picosha2.h>        // SHA-256 Hash calculation function

// Now, include ctle, which will implement the source code
#include <ctle/ctle.h>
```