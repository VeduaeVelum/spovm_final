#ifdef _WIN32

#include <windows.h>

#define HIST_FILE_NAME "C:\\spovmProjects\\lab3\\history_file.txt"

HANDLE hNamedPipe;
LPSTR  szPipeName = "\\\\.\\pipe\\$MyPipe$";
DWORD cbWritten;
DWORD cbRead;

#endif

#ifdef __linux__

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <cstdio>
#include <fcntl.h>

#define HIST_FILE_NAME "/home/erick_vb/spo/lab3/history_file.txt"

int fd, len;

#endif

#include <cstring>
#include <string>
#include <sstream>
#include <ctime>
#include "Client.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Client:" << std::endl;
	showMenu();

#ifdef _WIN32

	hNamedPipe = CreateFile(szPipeName,	GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE) 
	{
		std::cerr << "CreateNamedPipe error" << std::endl;
		return 1;
	}

	ñommunication();

	CloseHandle(hNamedPipe);

#endif

#ifdef __linux__

	if (mkfifo(NAMEDPIPE_NAME, 0777)) 
	{
		std::cerr << "CreateNamedPipe error" << std::endl;
	}
	if ((fd = open(NAMEDPIPE_NAME, O_RDWR)) <= 0) 
	{
		std::cerr << "Open file error" << std::endl;
		return 1;
	}

	ñommunication();

#endif

	return 0;
}

void showFiles(char *buffer, std::string folder)
{
	std::string file;
	std::stringstream ss;

	ss << buffer;

	while (getline(ss, file, '/'))
	{
		std::cout << folder << "/" << file << std::endl;
	}
}

void showMenu() 
{
	std::cout << "Comands:" << std::endl;
	std::cout << COM_LS << " - print files on server" << std::endl;
	std::cout << COM_GETFILE << " - get file from server(+input name)" << std::endl;
	std::cout << COM_PUTFILE << " - add file to server(+input name)" << std::endl;
	std::cout << COM_RMFILE << " - delete file(+input name)" << std::endl;
	std::cout << COM_HISTORY << " - show history of requests" << std::endl;
	std::cout << COM_EXIT << " - exit from program" << std::endl;
}

bool input_file(const char* command, std::fstream& file)
{
	file.open(HIST_FILE_NAME, std::ios_base::out | std::ios_base::app);
	if (!file.is_open())
	{
		std::cerr << "File can't be opened for writing" << std::endl;
		return false;
	}
	if (is_first_command)
	{
		time_t t;
		t = time(0);
		file << "-------------Starting session-----------\n";
		file << "Time of starting: " << ctime(&t) << "\n";
		is_first_command = false;
	}
	file << command << '\n';
	if (!strcmp(command, COM_EXIT.c_str()))
	{
		time_t t;
		t = time(0);
		file << "--------------Ending session-----------\n";
		file << "Time of ending: " << ctime(&t) << "\n";
	}
	file.close();
	return true;
}

bool output_file(std::fstream& file)
{
	char fileread[BUF_SIZE];
	file.open(HIST_FILE_NAME, std::ios_base::in);
	std::cout << "--------History of requests-----------" << std::endl;

	while (!file.eof())
	{
		if (file.eof())
		{
			break;
		}
		file.getline(fileread, BUF_SIZE);
		std::cout << fileread << std::endl;
	}

	std::cout << "--------End of history-----------" << std::endl;
	return true;
}

#ifdef _WIN32

void ñommunication() 
{
	std::fstream hist_file;
	const char* char_buf;
	while (true) 
	{
		getline(std::cin, buf);
		strcpy_s(buffer, buf.c_str());
		char_buf = buf.c_str();
		if (!strcmp(buffer, COM_HISTORY.c_str()))
		{
			if (!output_file(hist_file))
			{
				std::cerr << "Error reading history" << std::endl;
			}
			continue;
		}
		if (!WriteFile(hNamedPipe, buffer, strlen(buffer) + 1,
			&cbWritten, NULL))
		{
			std::cerr << "Server connection error" << std::endl;
		}
		
		input_file(char_buf, hist_file);

		if (ReadFile(hNamedPipe, buffer, BUF_SIZE, &cbRead, NULL)) 
		{
			if (!strcmp(buffer, COM_LS.c_str())) 
			{
				if (ReadFile(hNamedPipe, buffer, BUF_SIZE, 
					&cbRead, NULL))
				{
					showFiles(buffer, "public");
				}
				else 
				{
					std::cerr << "Pipe connection error(reading)"
						<< std::endl;
				}
			}
			else if (!strcmp(buffer, COM_EXIT.c_str())) 
			{
				break;
			}
		}
		else
		{
			std::cerr << "Pipe connection error(reading)" << std::endl;
		}
	}
}

#endif

#ifdef __linux__

void ñommunication() 
{
	std::fstream hist_file;
	const char* char_buf;
	while (true) 
	{
		memset(readbuffer, '\0', BUF_SIZE);
		memset(buffer, '\0', BUF_SIZE);

		getline(std::cin, buf);
		strcpy(buffer, buf.c_str());
		if (!strcmp(buffer, COM_HISTORY.c_str()))
		{
			if (!output_file(hist_file))
			{
				std::cerr << "Error reading history" << std::endl;
			}
			continue;
		}

		write(fd, buffer, strlen(buffer));

		input_file(char_buf, hist_file);

		if (!strcmp(buffer, COM_EXIT.c_str())) 
		{
			break;
		}
		else if (!strcmp(buffer, COM_LS.c_str())) 
		{
			std::cout << "Files on server:" << std::endl;
			if ((len = read(fd, readbuffer, BUF_SIZE - 1)) > 0) 
			{
				showFiles(readbuffer, "public");
			}
			else
			{
				std::cerr << "Pipe connection error(reading)" 
					<< std::endl;
				close(fd);
				remove(NAMEDPIPE_NAME);
				break;
			}
		}
	}
}

#endif
