## sockets.h

The `sockets.h` file provides classes and functions for socket programming. It includes classes for handling sockets, stream sockets, and server sockets, as well as functions for initializing and deinitializing the sockets library.

### Enums

#### `enum class socket_protocol_family`

Defines the protocol family for sockets.

### Functions

#### `status initialize_sockets()`

Initializes the sockets library. This function is not required to be called, but it is recommended for defensive reasons and performance.

#### `status deinitialize_sockets()`

Deinitializes the sockets library. The library is deinitialized for real when the allocated sockets are 0.

### Classes

#### `class socket`

Base class for sockets. This class is not used directly and implements generic functionality shared by derived classes.

#### `class stream_socket : public socket`

Class for handling stream sockets.

#### `class server_socket : public socket`

Class for handling server sockets.

### Example Usage

#### Initializing and Deinitializing Sockets

```cpp
#include "sockets.h"
#include <iostream>

int main() {
    ctle::status init_status = ctle::initialize_sockets();
    if (init_status == ctle::status::ok) {
        std::cout << "Sockets initialized successfully." << std::endl;
    } else {
        std::cerr << "Failed to initialize sockets." << std::endl;
        return 1;
    }

    // Perform socket operations...

    ctle::status deinit_status = ctle::deinitialize_sockets();
    if (deinit_status == ctle::status::ok) {
        std::cout << "Sockets deinitialized successfully." << std::endl;
    } else {
        std::cerr << "Failed to deinitialize sockets." << std::endl;
    }

    return 0;
}
```

#### Using Stream Sockets

```cpp
#include "sockets.h"
#include <iostream>

int main() {
    ctle::initialize_sockets();

    ctle::stream_socket client_socket;
    ctle::status connect_status = client_socket.connect("127.0.0.1", 8080);
    if (connect_status == ctle::status::ok) {
        std::cout << "Connected to server." << std::endl;

        const char *message = "Hello, server!";
        client_socket.send(message, strlen(message));

        char buffer[1024];
        auto receive_result = client_socket.receive(buffer, sizeof(buffer));
        if (receive_result.status == ctle::status::ok) {
            std::cout << "Received from server: " << std::string(buffer, receive_result.value) << std::endl;
        } else {
            std::cerr << "Failed to receive data from server." << std::endl;
        }
    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }

    ctle::deinitialize_sockets();
    return 0;
}
```
