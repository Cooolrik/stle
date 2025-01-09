## n_tup and mn_tup

The `n_tup` and `mn_tup` templates provide lightweight vector and matrix types with 1-4 dimensions. They support various numeric types and optional GLM interoperability.

### n_tup
n_tup is a vector-type, with 1-4 dimensions. 

- `_Ty`: The type of the elements (i8, u8, i16, u16, i32, u32, i64, u64, f32, f64)
- `dims`: The number of dimensions (1-4)

### mn_tup
mn_tup is matrix-type, with 1x1 - 4x4 dimensions. The class is implemented as an array of n_tup tuples, which are the "inner" values in the 
class. mn_tup can be interpreted as either a row-major matrix, where the rows are the inner values, or as a column-major matrix, where the 
columns are the inner values. Use the optional defines CTLE_MN_TUP_COLUMNMAJOR and CTLE_MN_TUP_ROWMAJOR to add additional size values `num_rows` and `num_columns`, which
alias `inner_dims` and `outer_dims` as appropriate. Note! The GLM interops assume that the mn_tup is in column-major order, as this is the
order which GLM is defined. Transpose GLM matrices before and after conversion if you use row-major mn_tup.

- `_Ty`: The type of the elements
- `_InnerSize`: The size of each inner tuple
- `_OuterSize`: The number of outer tuples (1-4)

### Features

- Value types for 1D to 4D vectors and matrices
- Support for all common numeric types
- Optional GLM interoperability (when `glm.hpp` is included before `ntup.h`)
- Optional row/column major matrix options via CTLE_MN_TUP_COLUMNMAJOR or CTLE_MN_TUP_ROWMAJOR
- Standard comparison operators
- String conversion support

### Examples

#### Basic Vector Usage

```cpp
#include "ntup.h"
#include <iostream>

int main()
{
    // Create a 3D vector of floats
    ctle::n_tup<float, 3> vec1(1.0f, 2.0f, 3.0f);
    
    // Access components using x,y,z or r,g,b
    std::cout << "Vector components: " << vec1.x << ", " << vec1.y << ", " << vec1.z << std::endl;
    
    // Array-style access
    for (size_t i = 0; i < vec1.dims; ++i)
    {
        std::cout << "Component " << i << ": " << vec1[i] << std::endl;
    }
    
    return 0;
}
```

#### Matrix Usage

```cpp
#include "ntup.h"
#include <iostream>

int main()
{
    // Create a 2x3 matrix of integers. 
	// This code assumes row-major order, so 2 rows of 3 values each
	// (inner_dims = 3, outer_dims = 2)
    ctle::mn_tup<int, 3, 2> mat;
    
    // Set values for first row
    mat[0] = ctle::n_tup<int, 3>(1, 2, 3);
    
    // Set values for second row
    mat[1] = ctle::n_tup<int, 3>(4, 5, 6);
    
    // Access values
    for (size_t i = 0; i < mat.outer_dims; ++i)
    {
        for (size_t j = 0; j < mat.inner_dims; ++j)
        {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}
```

#### GLM Interoperability

```cpp
#include <glm/glm.hpp>
#include "ntup.h"
#include <iostream>

int main()
{
    // Create a GLM vector
    glm::vec3 glm_vec(1.0f, 2.0f, 3.0f);
    
    // Convert to ctle n_tup
    ctle::n_tup<float, 3> ctle_vec = glm_vec;
    
    // Convert back to GLM
    glm::vec3 back_to_glm = ctle_vec;
    
    // Print to verify
    std::cout << "Original: " << glm_vec.x << ", " << glm_vec.y << ", " << glm_vec.z << std::endl;
    std::cout << "Converted: " << back_to_glm.x << ", " << back_to_glm.y << ", " << back_to_glm.z << std::endl;
    
    return 0;
}
```

#### String Conversion

```cpp
#include "ntup.h"
#include <iostream>

int main()
{
    // Create a vector
    ctle::n_tup<int, 3> vec(1, 2, 3);
    
    // Convert to string
    std::string str = ctle::to_string(vec);
    std::cout << "Vector as string: " << str << std::endl;
    
    // Parse from string
    bool success = false;
    auto parsed_vec = ctle::from_string<ctle::n_tup<int, 3>>("1 2 3", success);
    
    if (success)
    {
        std::cout << "Parsed vector: " << ctle::to_string(parsed_vec) << std::endl;
    }
    
    return 0;
}
```