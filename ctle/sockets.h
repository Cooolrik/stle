// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_SOCKETS_H_
#define _CTLE_SOCKETS_H_

/// @file sockets.h 
/// @brief Sockets library for ctle, for creating and managing sockets, and for creating server sockets.

#include <functional>
#include <memory>
#include <string>

#include "status.h"
#include "status_return.h"

namespace ctle
{
class socket;
class stream_socket;
class server_socket;

/// @brief The protocol family for the socket
enum class socket_protocol_family
{
	unspecified,
	ipv4,
	ipv6,
};

/// @brief Initialize the sockets code.
/// @details Initializes the sockets code. Not required to be called, but recommended for 
/// defensive reasons, if the sockets cannot be initialized, and for performance, 
/// since the sockets library keep an internal tally of how many sockets
/// are allocated, and deinitializes if the tally falls to 0, if the user
/// has not explicitly called initialize_sockets.
/// @returns status::ok if the sockets are initialized, or an error code if the sockets could not be initialized.
status initialize_sockets();

/// @brief Deinitialize the sockets code.
/// Deinitialize sockets code. If sockets are still allocated, deinitializes for real when the allocated sockets are 0
/// @returns status::ok if the call succeeded, or an error code if the sockets could not be deinitialized.
status deinitialize_sockets();

/// @brief The base class for the sockets.
/// @details Base class for sockets. Not to be used directly, implements generic functionality shared by derived classes
class socket
{
public:
	~socket();

protected:
	class file;

	socket();
	socket(std::unique_ptr<file>);
	
	static void transfer_socket_file(socket &dest, socket &src);

	std::unique_ptr<file> socket_file;
};

/// @brief A stream socket for sending and receiving data.
class stream_socket : public socket
{
public:
	// sockets can only be owned by one object at a time, but can be handed over
	stream_socket(std::unique_ptr<file>);
	stream_socket(stream_socket&&);
	stream_socket& operator=(stream_socket&&);
	~stream_socket();

	/// @brief Create and connect a stream socket to a specified address and port
	static status_return<status,std::unique_ptr<stream_socket>> connect(const std::string &address, uint16_t port, socket_protocol_family protocol_family = socket_protocol_family::ipv4);
	static status_return<status,std::unique_ptr<stream_socket>> connect(const std::string &address, const std::string &port, socket_protocol_family protocol_family = socket_protocol_family::ipv4);

	/// @brief send a message on a socket
	/// @param buf data buffer to copy from
	/// @param buflen number of bytes to send 
	/// @param sent receives actual number of bytes sent
	/// @returns status::ok if the message was sent, or an error code if the call failed
	status send(const void* buf, size_t buflen, size_t& sent);

	/// @brief receive a message on a socket
	/// @param buf data buffer to copy to
	/// @param buflen number of bytes to receive 
	/// @param received actual number of bytes received
	/// @returns status::ok if the message was received, or an error code if the call failed
	status recv(void* buf, size_t buflen, size_t& received);
};

/// @brief A server socket for accepting incoming connections.
class server_socket : public socket
{
public:
	using serve_func = const std::function<status(stream_socket)>;
	
	/// @brief state of the server
	enum class server_state
	{
		stopped,
		started,
		running,
		stopping
	};

	server_socket();
	~server_socket();

	/// @brief Start the server (blocking call)
	/// @details Opens a socket and runs a blocking listen() + accept() loop. 
	/// To stop the server, call the stop() function from another thread.
	/// To run the server async, wrap the call in an std::async() call.
	/// @param port port number or port type string (e.g. "http") to listen to
	/// @param serve_function a function to call for each accepted connection, and is 
	/// responsible to handle the connection(e.g.spawn a thread to handle the incoming connection)
	/// @param protocol_family is either ip4 or ip6
	/// @param backlog_size is the number of incoming connections to keep in queue when handling the current connection
	/// @returns status::ok if the server was started and ran successfully (since this is a blocking 
	/// call), or an error code if the server could not be started
	status start(uint16_t port, const serve_func& serve_function, socket_protocol_family protocol_family = socket_protocol_family::ipv4, size_t backlog_size = 10);
	status start(const std::string &port, const serve_func& serve_function, socket_protocol_family protocol_family = socket_protocol_family::ipv4, size_t backlog_size = 10);

	/// @brief Stop the server
	/// @details Signals the server to stop. This needs to be called from another thread than start(), which will block until the server is signaled to stop. 
	/// To make sure the server stops, check the server_state() function.
	/// @returns status::ok if the call succeeded (the server was signaled to stop), or an error code if the server could not be stopped
	status stop();

	/// @brief get the current state of the server
	server_state get_server_state() const;

private:
	struct internal_data;
	std::unique_ptr<internal_data> data;

	status run_internal(const std::string &port, const serve_func& serve_function, socket_protocol_family protocol_family, size_t backlog_size);
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include <string>
#include <atomic>
#include <utility>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#if defined(_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(linux)
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#endif

#include "log.h"
#include "_macros.inl"

namespace ctle
{
static std::mutex sockets_init_mutex;
static size_t sockets_reference_count = 0;

status initialize_sockets()
{
	const std::lock_guard<std::mutex> lock(sockets_init_mutex);

	// only init when passing from 0->1
	++sockets_reference_count;
	if( sockets_reference_count > 1 )
		return status::ok;

#if defined(_WIN32)
	ctLogDebug << "Initializing Windows Sockets 2" << ctLogEnd;

	WORD wsa_version = MAKEWORD(2, 2);
	WSADATA wsa_data = {};
    auto result = WSAStartup(wsa_version, &wsa_data);
	if( result != 0 )
	{
		sockets_reference_count = 0;
		auto socket_error = WSAGetLastError();
		ctLogError << "WSAStartup() call failed. WSAGetLastError() returned error code:" << socket_error << "." << ctLogEnd;
		return status::cant_allocate;
	}
	else
	{
		ctLogDebug << "WSAStartup() call succeeded." << ctLogEnd;
	}
#endif

	return status::ok;
}

status deinitialize_sockets()
{
	const std::lock_guard<std::mutex> lock(sockets_init_mutex);

	ctValidate( sockets_reference_count != 0, status::invalid ) << "deinitialize_sockets() called, when the number of references are already 0" << ctValidateEnd;

	// decrease reference count, but only deinit when passing from 1->0
	--sockets_reference_count;
	if( sockets_reference_count != 0 )
		return status::ok;

#if defined(_WIN32)
	ctLogDebug << "Shutting down Windows Sockets 2" << ctLogEnd;

	auto result = WSACleanup();
	if( result != 0 )
	{
		sockets_reference_count = 0;
		auto socket_error = WSAGetLastError();
		ctLogError << "WSACleanup() call failed. WSAGetLastError() returned error code:" << socket_error << "." << ctLogEnd;
		return status::not_initialized;
	}
	else
	{
		ctLogDebug << "WSACleanup() call succeeded." << ctLogEnd;
	}
#endif

	return status::ok;
}

/////////////////////////////////////////

#if defined(_WIN32)
using socket_type = SOCKET;
constexpr const socket_type invalid_socket = socket_type(SOCKET_ERROR);
#elif defined(linux)
using socket_type = int;
constexpr const socket_type invalid_socket = -1;
#endif

inline int protocol_family_to_AF(socket_protocol_family protocol_family)
{
	if (protocol_family == socket_protocol_family::ipv4)
		return AF_INET;
	else if (protocol_family == socket_protocol_family::ipv6)
		return AF_INET6;
	else
		return AF_UNSPEC;
}

inline void *get_inet_addr_pointer(sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((sockaddr_in*)sa)->sin_addr);
	else
		return &(((sockaddr_in6*)sa)->sin6_addr);
}

inline int get_last_socket_error()
{
#if defined(_WIN32)
	return WSAGetLastError();
#elif defined(linux)
	return errno;
#endif
}

class socket::file
{
public:
	file();
	~file();

	// create a socket descriptor. if an existing socket is owned by the file object, it is automatically closed
	// ai_family, ai_socktype and ai_protocol fields of the addrinfo &addr object is used to create the socket file descriptor
	status create( const addrinfo &addr );

	// connect the socket to another socket
	status connect( const addrinfo &addr ) const;

	// bind a socket descriptor to the specified address & port, to prepare for listening. optionally mark the address & port for reuse (default set), if it was recently closed (often the case when debugging)
	status bind( const addrinfo &addr, bool reuse_address = true ) const;

	// start listening on bound socket
	status listen( size_t backlog_size ) const;

	// accept a connection on listening port
	status_return<status,std::unique_ptr<socket::file>> accept( sockaddr *remote_addr, socklen_t &remote_addr_size ) const;

	// send data on a stream socket
	status send(const void* buf, size_t buflen, size_t& sent) const;

	// receive data on a stream socket
	status recv(void* buf, size_t buflen, size_t& received) const;

	// close the socket 
	status close();

	// returns if the socket is valid or invalid
	bool is_valid() const;

private:
	socket_type fd = invalid_socket;
};

inline status socket::file::close()
{
	if( this->fd != invalid_socket )
	{
#if defined(_WIN32)
		int result = ::closesocket(this->fd);
#elif defined(linux)
		int result = ::close(this->fd);
#endif
		this->fd = invalid_socket;
		ctValidate( result != invalid_socket, status::invalid) << "Got an error code: " << get_last_socket_error() << " when closing the socket. " << ctValidateEnd;
	}
	return status::ok;
}

inline socket::file::file()
{
	// add a reference to the sockets library, to make sure that it is initialized. 
	auto result = initialize_sockets();
	if( !result )
	{
		ctLogError 
			<< "initialize_sockets() failed, and returned the error: " << result 
			<< ", which is unexpected. The socket library may not be available. " 
			<< "Add an explict call to initialize_sockets() before allocating sockets to make sure the socket library exists." 
			<< ctLogEnd;
	}
}

inline socket::file::~file()
{
	auto result = this->close();
	if( !result )
	{
		ctLogError 
			<< "close() failed, and returned the error: " << result 
			<< ctLogEnd;
	}

	result = deinitialize_sockets();
	if( !result )
	{
		ctLogError 
			<< "deinitialize_sockets() failed, and returned the error: " 
			<< result << ", which is unexpected. It seems the number of allocated " 
			<< "sockets are out of sync, or deinitialize_sockets() has been called too many times." << ctLogEnd;
	}
}

inline status socket::file::create( const addrinfo &addr )
{
	ctStatusCall( this->close() );

	this->fd = ::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
	ctValidate( this->fd != invalid_socket , status::cant_open ) 
		<< "Could not create socket for family: " << addr.ai_family 
		<< ", protocol: " << addr.ai_protocol 
		<< ". System error code: " << get_last_socket_error() 
		<< ctValidateEnd;

	return status::ok;
}

inline status socket::file::connect( const addrinfo &addr ) const
{
	int result = {};
	ctValidate( this->fd != invalid_socket , status::invalid ) << "Invalid call when no socket is created." << ctValidateEnd;

#if defined(_WIN32)
	result = ::connect(this->fd, addr.ai_addr, (int)addr.ai_addrlen);
#elif defined(linux)
	result = ::connect(this->fd, addr.ai_addr, addr.ai_addrlen);
#endif
	ctValidate( result == 0 , status::cant_allocate ) 
		<< "Could not connect socket to remote address. Family: " << addr.ai_family 
		<< ", protocol: " << addr.ai_protocol 
		<< ". System error code: " << get_last_socket_error() 
		<< ctValidateEnd;

	return status::ok;
}

inline status socket::file::bind( const addrinfo &addr, bool reuse_address ) const
{
	int result = {};
	ctValidate( this->fd != invalid_socket , status::invalid ) << "Invalid call when no socket is created." << ctValidateEnd;

	// tell sockets api to reuse the address
	if( reuse_address )
	{
		const int option_value = 1;
#if defined(_WIN32)
		result = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option_value, sizeof(option_value));
#elif defined(linux)
		result = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
#endif
		ctValidate( result == 0 , status::cant_allocate ) 
			<< "Could not set the socket option on the socket file descriptor. System error code: " << get_last_socket_error() 
			<< ctValidateEnd;
	}

	// bind the socket
#if defined(_WIN32)
	result = ::bind(this->fd, addr.ai_addr, (int)addr.ai_addrlen);
#elif defined(linux)
	result = ::bind(this->fd, addr.ai_addr, addr.ai_addrlen);
#endif
	ctValidate( result == 0 , status::cant_allocate ) 
		<< "Could not bind socket for for family: " << addr.ai_family 
		<< ", protocol: " << addr.ai_protocol 
		<< ". System error code: " << get_last_socket_error() 
		<< ctValidateEnd;

	return status::ok;
}

inline status socket::file::listen( size_t backlog_size ) const
{
	ctValidate( this->fd != invalid_socket , status::invalid ) << "Invalid call when no socket is created." << ctValidateEnd;

	auto result = ::listen(this->fd, (int)backlog_size);
	ctValidate( result == 0 , status::not_ready ) 
		<< "Could place socket in listen mode. System error code: " << get_last_socket_error() 
		<< ctValidateEnd;

	return status::ok;
}

inline status_return<status,std::unique_ptr<socket::file>> socket::file::accept( sockaddr *remote_addr, socklen_t &remote_addr_size ) const
{
	ctValidate( this->fd != invalid_socket , status::invalid ) << "Invalid call when no socket is created." << ctValidateEnd;

	std::unique_ptr<socket::file> incoming_file( new socket::file() );

	incoming_file->fd = ::accept(this->fd, remote_addr, &remote_addr_size);
	ctValidate(incoming_file->fd != invalid_socket, status::invalid) 
		<< "Call to socket accept() failed. System error code: " << get_last_socket_error() 
		<< ctValidateEnd;

	return incoming_file;
}

inline status stream_socket::file::send(const void* buf, size_t buflen, size_t& sent) const
{
	ctValidate(this->fd != -1, status::not_initialized) << "The socked has not been initialized, or has been closed after being initialized." << ctValidateEnd;

#if defined(_WIN32)
	int result = ::send(this->fd, (const char*)buf, (int)buflen, 0);
#elif defined(linux)
	ssize_t result = ::send(this->fd, buf, buflen, 0);
#endif
	if( result < 0 )
	{
		sent = 0;
		return status::cant_write;
	}
	sent = result;

	return status::ok;
}

inline status stream_socket::file::recv(void* buf, size_t buflen, size_t& received) const
{
	ctValidate(this->fd != -1, status::not_initialized) << "The socked has not been initialized, or has been closed after being initialized." << ctValidateEnd;

#if defined(_WIN32)
	int result = ::recv(this->fd, (char*)buf, (int)buflen, 0);
#elif defined(linux)
	ssize_t result = ::recv(this->fd, buf, buflen, 0);
#endif
	if( result < 0 )
	{
		received = 0;
		return status::cant_write;
	}
	received = result;
	
	return status::ok;
}

inline bool stream_socket::file::is_valid() const
{
	return this->fd != invalid_socket;
}

/////////////////////////////////////////

socket::socket()
	: socket_file( new socket::file() )
{}

socket::socket(std::unique_ptr<file> other_file)
	: socket_file( std::move(other_file) )
{
}

socket::~socket()
{
}

void socket::transfer_socket_file(socket &dest, socket &src)
{
	// transfer over the pointer from the src to the dest, but also allocate a new empty file in src, so that it still has an allocated object
	dest.socket_file = std::move(src.socket_file);
	src.socket_file = std::unique_ptr<socket::file>(new socket::file());
}

/////////////////////////////////////////

stream_socket::stream_socket(std::unique_ptr<file> other_file)
	: socket( std::move(other_file))
{
}

stream_socket::stream_socket(stream_socket &&other)
{
	socket::transfer_socket_file(*this, other);
}

stream_socket& stream_socket::operator=(stream_socket &&other)
{
	socket::transfer_socket_file(*this, other);
	return *this;
}

stream_socket::~stream_socket()
{
}

status_return<status,std::unique_ptr<stream_socket>> stream_socket::connect(const std::string &address, const std::string &port, socket_protocol_family protocol_family)
{
	// we need to make sure the socket library is set up, so allocate the stream_socket object with an empty file
	auto connect_socket = std::unique_ptr<stream_socket>( new stream_socket(std::unique_ptr<file>(new file())) );

	int result = {};
	addrinfo hints = {};
	addrinfo* address_info = {};
	const char *node_name = address.empty() ? nullptr : address.c_str();

	// set up the servinfo structure, use getaddrinfo. it will possibly return multiple matches
	hints.ai_family = protocol_family_to_AF(protocol_family);
	hints.ai_socktype = SOCK_STREAM;
	result = getaddrinfo(node_name, port.c_str(), &hints, &address_info);
	ctValidate(result == 0, status::not_found ) << "Could not find the address using the specified protocol family or families." << ctValidateEnd;

	// to to connect using possible protocols
	for(addrinfo* p = address_info; p != nullptr; p = p->ai_next)
	{
		if( connect_socket->socket_file->create(*p) )
		{
			if( connect_socket->socket_file->connect(*p) )
			{
				// found, created and connected
				break;
			}
		}

		// not possible to connect, make sure the file is closed
		connect_socket->socket_file->close();
	}

	// dont need the address info anymore
	freeaddrinfo(address_info);

	ctValidate( connect_socket->socket_file->is_valid(), status::cant_open ) << "Could not connect to the remote address." << ctValidateEnd;
	return connect_socket;
}

status_return<status,std::unique_ptr<stream_socket>> stream_socket::connect(const std::string &address, uint16_t port, socket_protocol_family protocol_family )
{
	return std::move( stream_socket::connect( address, std::to_string(port), protocol_family) );
}

status stream_socket::send(const void* buf, size_t buflen, size_t& sent)
{
	return this->socket_file->send(buf,buflen,sent);
}

status stream_socket::recv(void* buf, size_t buflen, size_t& received)
{
	return this->socket_file->recv(buf,buflen,received);
}

/////////////////////////////////////////

struct server_socket::internal_data
{
	std::atomic<ctle::server_socket::server_state> _server_state = { server_state::stopped };

	std::string server_port;
	socket_protocol_family server_protocol_family = {};
};

server_socket::server_socket()
	: socket()
	, data(new server_socket::internal_data())
{
}

server_socket::~server_socket()
{
}

status server_socket::run_internal(const std::string& port, const serve_func& serve_function, socket_protocol_family protocol_family, size_t backlog_size)
{
	ctLogInfo << "server_socket::start(): running server, setting up listen socket" << ctLogEnd;

	int result = {};
	addrinfo hints = {};
	addrinfo* servinfo = {};

	// set up the servinfo structure, use getaddrinfo
	hints.ai_family = protocol_family_to_AF(protocol_family);
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	result = getaddrinfo(nullptr, port.c_str(), &hints, &servinfo);
	ctValidate(result == 0, status::not_found) << "Could not find the specified protocol and set up a local address on port " << port << ctValidateEnd;

	// find a socket type to bind to, use first successful
	for(addrinfo* p = servinfo; p != nullptr; p = p->ai_next)
	{
		if( this->socket_file->create( *p ) )
		{
			if( this->socket_file->bind( *p ) )
			{
				// successfully bound
				ctLogInfo << "Socket successfully bound for family: " << p->ai_family << ", protocol: " << p->ai_protocol << ctLogEnd;
				this->data->server_port = port;
				this->data->server_protocol_family = protocol_family;
				break;
			}
		}

		// make sure the socket is closed
		ctStatusCall( this->socket_file->close() );
	}

	// dont need the address info anymore
	freeaddrinfo(servinfo);

	ctValidate(this->socket_file->is_valid(), status::not_found) << "Could not match the selected protocol and bind successfully to a socket." << ctValidateEnd;

	// start listening to the bound socket
	ctStatusCall( this->socket_file->listen(backlog_size) );

	ctLogInfo << "Waiting for connections, listening on port: " << port << ctLogEnd;
	this->data->_server_state = server_state::running;
	
	// blocking accept loop
	while( this->data->_server_state == server_state::running )
	{
		// accept a connection to the listening socket
		sockaddr_storage remote_addr = {};
		socklen_t remote_addr_size = sizeof( remote_addr );

		// accept an incoming connection. this call is blocking, and the incoming call may be the stop() method just waking us up to shut down.
		std::unique_ptr<socket::file> remote_file;
		ctStatusReturnCall( remote_file, this->socket_file->accept((sockaddr*)&remote_addr, remote_addr_size) );
		if( this->data->_server_state != server_state::running )
		{
			ctLogInfo << "Server signaled to stop" << ctLogEnd;
			break;
		}

		// get the address of the remote process, log it
		char remote_address[INET6_ADDRSTRLEN];
		inet_ntop(
			remote_addr.ss_family,
			get_inet_addr_pointer((sockaddr*)&remote_addr),
			remote_address,
			sizeof(remote_address)
		);
		ctLogInfo << "Accepted incoming connection from: " << remote_address << ctLogEnd;

		// call the provided function, to handle the incoming socket
		ctStatusCall(serve_function(stream_socket(std::move(remote_file))));
	}

	// we are done, close the socket and return
	ctLogInfo << "Closing down server listen socket" << ctLogEnd;
	this->socket_file->close();

	return status::ok;
}

status server_socket::start(const std::string& port, const serve_func& serve_function, socket_protocol_family protocol_family, size_t backlog_size)
{
	// make sure the server is not already running, and change state to running
	if (this->data->_server_state != server_state::stopped)
		return status::already_initialized;
	this->data->_server_state = server_state::started;

	auto result = this->run_internal(port, serve_function, protocol_family, backlog_size);

	// clean up, change state to stopped, and make sure the socket is closed
	ctStatusCall(this->socket_file->close());
	this->data->_server_state = server_state::stopped;

	return result;
}
	
status server_socket::start(uint16_t port, const serve_func & serve_function, socket_protocol_family protocol_family, size_t backlog_size)
{
	return this->start(std::to_string(port), serve_function, protocol_family, backlog_size);
}

status server_socket::stop()
{
	if (this->data->_server_state != server_state::running)
		return status::not_initialized;

	ctLogInfo << "Signaling server to shut down" << ctLogEnd;

	// signal server, and do a local connect to the listen socket, so the server wakes up from a blocking accept() call
	this->data->_server_state = server_state::stopping;
	std::unique_ptr<stream_socket> wake_connect;
	ctStatusReturnCall( wake_connect, stream_socket::connect("",this->data->server_port,this->data->server_protocol_family) );

	return status::ok;
}

server_socket::server_state server_socket::get_server_state() const
{
	return this->data->_server_state;
}

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_SOCKETS_H_
