#include <iostream>
#include <utility>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <chrono>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>

const char socket_net_addr[] = "127.0.0.1";
const size_t net_port = 8080;

class Socket
{
	int socket_handler;
	int conn_socket_handler;
	int opt = 1;
	sockaddr_in serv_addr;
	size_t socket_port;

public:
	Socket(const char* socket_addr, size_t port)
	{
		socket_port = port;

		conn_socket_handler = socket(AF_INET, SOCK_STREAM, 0);

		if (conn_socket_handler == -1)
		{
			throw std::runtime_error("Error: failed to create socket");
		}

		if (setsockopt(conn_socket_handler, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		{
			throw std::runtime_error("Error: failed to create socket port");
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);

		size_t addrlen = sizeof(serv_addr);

		if (inet_pton(AF_INET, socket_net_addr, &serv_addr.sin_addr) == -1)
		{
			throw std::runtime_error("Error: failed to convert address");
		}

		if (bind(conn_socket_handler, (struct sockaddr *) &serv_addr, addrlen) == -1) 
	    { 
	        throw std::runtime_error("Error: failed to bind socket");
	    }

	    if (listen(conn_socket_handler, 1) == -1)
	    {
	    	throw std::runtime_error("Error: failed to listen incoming connection");
	    }

	    socket_handler = accept(conn_socket_handler, (struct sockaddr *) &serv_addr,  (socklen_t*) &addrlen);

	    if (socket_handler == -1)
	    {
	    	throw std::runtime_error("Error: failed to establish connection");
	    }
	}

	~Socket()
	{
		close(socket_handler);
	}
	
	void read(size_t* output)
	{
		int bytes_read = recv(socket_handler, output, sizeof(size_t), 0);

		if (bytes_read == 0)
		{
			throw std::runtime_error("Receive 0 bytes, probably client was terminated");
		}

		if (bytes_read < 0)
		{
			throw std::runtime_error("Error: cannot read the socket");
		}		
	}

	void write(size_t msg)
	{
		if (send(socket_handler, &msg, sizeof(size_t), 0) == -1)
		{
			throw std::runtime_error("Error: cannot write to the socket");
		}
	}

	void rebind()
	{
		size_t addrlen = sizeof(serv_addr);

		socket_handler = accept(conn_socket_handler, (struct sockaddr *) &serv_addr,  (socklen_t*) &addrlen);
	}
};

enum ServerResponce
{
	OK_NEW,
	OK_PREV,
	OVER_LIMITS,
	INVALID_KEY,
	INTERNAL_ERROR,
};

size_t read_limit(const std::string& filename)
{
	std::ifstream file;

	try
	{
		file.open(filename);
	}
	catch(...)
	{
		throw std::runtime_error("Internal error: cannot read limits file");
	}

	std::string line;

	std::getline(file, line);

	return std::stoll(line);
}

void run(Socket& s)
{
	const std::string file_dir = "db/";
	const std::string exp_dir = "exp_keys/";

	try
	{
		while (true)
		{
			// read Programm key
			size_t key;

			s.read(&key);

			// check key
			if (!std::filesystem::exists(file_dir + std::to_string(key)))
			{
				s.write(INVALID_KEY);

				s.rebind();

				continue;
			}

			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

			// check limits
			size_t current_limit = read_limit(file_dir + std::to_string(key));;

			if (current_limit == 0)
			{
				s.write(OVER_LIMITS);

				s.rebind();

				continue;
			}

			if (std::filesystem::exists(exp_dir + std::to_string(key)))
			{
				s.write(OK_PREV);

				std::filesystem::remove(exp_dir + std::to_string(key));
			}
			else
			{
				s.write(OK_NEW);
			}

			size_t dummy;

			// wait for end of work of programm
			s.read(&dummy);

			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			std::ofstream output(file_dir + std::to_string(key), std::ofstream::trunc);

			size_t new_limit;

			if (key % 2)
			{
				output << current_limit - 1;

				new_limit = current_limit - 1;
			}
			else
			{
				size_t time_delta = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

				new_limit = time_delta >= current_limit ? 0 : current_limit - time_delta;

				output << new_limit;
			}

			output.close();

			// write updated limit
			s.write(new_limit);

			s.rebind();
		}
	}
	catch (std::runtime_error e)
	{
		std::cerr << e.what() << std::endl;

		s.write(INTERNAL_ERROR);

		throw e;
	}
}

int main(int argc, char const *argv[])
{
	try
	{
		Socket s(socket_net_addr, net_port);
		run(s);
	}
	catch (std::runtime_error e)
	{
		std::cerr << e.what() << std::endl;

		return 1;
	}

	return 0;
}
