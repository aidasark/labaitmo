#include <iostream>
#include <cstring>
#include <fstream>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#include <unistd.h>
#include <cstdlib>

const char socket_net_addr[] = "127.0.0.1";
const size_t net_port = 8080;

class Socket
{
	int socket_handler;

public:
	Socket(const char* socket_addr, size_t port)
	{
		socket_handler = socket(AF_INET, SOCK_STREAM, 0);

		if (socket_handler == -1)
		{
			throw std::runtime_error("Error: failed to create socket");
		}

		sockaddr_in client_addr;
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(port); 

		if (inet_pton(AF_INET, socket_addr, &client_addr.sin_addr) == -1)
		{
			throw std::runtime_error("Error: failed to convert address");
		}

		if (connect(socket_handler, (sockaddr*) &client_addr, sizeof(client_addr)) == -1)
		{
			std::cout << "Cannot connect to the server. Trying again..." << std::endl;

			while (connect(socket_handler, (sockaddr*) &client_addr, sizeof(client_addr)) == -1) {}

			std::cout << "Connected!" << std::endl;
		}
	}

	~Socket()
	{
		close(socket_handler);
	}
	
	void write(size_t msg)
	{
		if (send(socket_handler, &msg, sizeof(size_t), 0) == -1)
		{
			throw std::runtime_error("Error: cannot write to the socket");
		}
	}

	int read(size_t* output)
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

		return bytes_read;
	}
};

void sigpipe_handler(int t)
{
	std::cerr << "Fatal error: cannot write to socket. Terminated" << std::endl;
	std::exit(1);
}

enum ServerResponce
{
	OK_NEW,
	OK_PREV,
	OVER_LIMITS,
	INVALID_KEY,
	INTERNAL_ERROR,
};

void ask_user(std::string& f_name, std::string& s_name, std::string& t_name)
{
	std::cout << "Please, enter first, second and third names" << std::endl;

	std::cin >> f_name >> s_name >> t_name;
}

bool check_data(std::fstream& file, const std::string& data)
{
	std::string line;

	while (std::getline(file, line))
	{
		if (line == data)
		{
			return true;
		}
	}

	// because we've reached end of file
	// https://stackoverflow.com/questions/5343173/returning-to-beginning-of-file-after-getline
	file.clear();

	return false;
}

std::string concat_names(std::string& f_name, std::string& s_name, std::string& t_name)
{
	return f_name + " " + s_name + " " + t_name;
}

void process_work()
{
	const std::string db_name = "db";

	std::string f_name, s_name, t_name;

	ask_user(f_name, s_name, t_name);

	std::string full_record = concat_names(f_name, s_name, t_name);

	try
	{
		std::fstream file;
		file.open(db_name);

		if (check_data(file, full_record))
		{
			std::cout << "Name already in database" << std::endl;
		}
		else
		{
			file << full_record + "\n";
		}

		file.close();
	}
	catch(...)
	{
		throw std::runtime_error("Cannot open database");
	}
}

void match_answer(size_t answ)
{
	switch (answ)
	{
		case OK_NEW:
			try
			{
				process_work();
			}
			catch(std::runtime_error e)
			{
				throw e;
			}

			break;
		case OK_PREV:
			std::cout << "Programm was previously installed" << std::endl;

			try
			{
				process_work();
			}
			catch(std::runtime_error e)
			{
				throw e;
			}

			break;
		case OVER_LIMITS:
			throw std::runtime_error("Sorry, you are out of limit. Please buy full version");
		case INVALID_KEY:
			throw std::runtime_error("Invalid key. Terminated");
		case INTERNAL_ERROR:
			throw std::runtime_error("Internal server error. Terminated");
	}
}

void run(size_t key)
{
	try
	{
		Socket sock(socket_net_addr, net_port);

		// Write key

		sock.write(key);

		// Read the answer and check it (print additional info)

		size_t answ;

		sock.read(&answ);

		match_answer(answ);

		// write exit code (0, however programm may exit with another code)

		sock.write(0);

		sock.read(&answ);

		std::cout << "Received new limit: " << std::to_string(answ) << std::endl;
	}
	catch(std::runtime_error e)
	{
		throw e;
	}
}

int main(int argc, char const *argv[])
{
	signal(SIGPIPE, sigpipe_handler);

	// for test purpose only
	// in real programm must be only one key
	// if key is even then it is time-limited programm, attempts-limited otherwise
	size_t key = 1;

	if (argc == 2 && std::string(argv[1]) == "-t")
	{
		key = 0;
	}

	try
	{
		run(key);
	}
	catch(std::runtime_error e)
	{
		std::cerr << e.what() << std::endl;

		return 1;
	}

	return 0;
}
