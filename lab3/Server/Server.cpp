#ifdef _WIN32

#include <windows.h>

#define PUBLIC_DIR "..\\Debug\\public\\*"
#define PUBLIC_FOLDER "..\\Debug\\public\\"
#define CLIENT_FOLDER "..\\Debug\\client\\"

HANDLE hNamedPipe;
LPSTR  lpszPipeName = "\\\\.\\pipe\\$MyPipe$";
DWORD  cbRead;
DWORD  cbWritten;
WIN32_FIND_DATA FindFileData;
HANDLE hFind = FindFirstFile(PUBLIC_DIR, &FindFileData);

#endif

#ifdef __linux__

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>

#define PUBLIC_FOLDER_UNIX "./Debug/public/"
#define CLIENT_FOLDER_UNIX "./Debug/client/"
#define NAMEDPIPE_NAME "/tmp/server"

int fd, len;

#endif

#include <cstring>
#include <string>
#include <sstream>
#include "Server.hpp"

struct command 
{
	const std::string maxi;
	char mini;
};

struct command Command[] = {
	{ COM_LS, 'l'},
	{ COM_GETFILE, 'g' },
	{ COM_PUTFILE, 'p' },
	{ COM_RMFILE, 'r' },
	{ COM_EXIT, 'e' }
};

int main(int argc, char *argv[]) 
{
	std::cout << "Server:" << std::endl;

#ifdef _WIN32

	hNamedPipe = CreateNamedPipe(lpszPipeName, PIPE_ACCESS_DUPLEX, 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, BUF_SIZE, BUF_SIZE, 5000,	NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE) 
	{
		std::cerr << "CreateNamedPipe error" << std::endl;
		return 1;
	}

	if (!ConnectNamedPipe(hNamedPipe, NULL)) 
	{
		std::cerr << "ConnectNamedPipe error" << std::endl;
		return 1;
	}

	communication();
	
	DisconnectNamedPipe(hNamedPipe);
	CloseHandle(hNamedPipe);

#endif

#ifdef __linux__
	
	if (mkfifo(NAMEDPIPE_NAME, 0777))
	{
		std::cerr << "Make fifo error" << std::endl;
		return 1;
	}
	if ((fd = open(NAMEDPIPE_NAME, O_RDWR)) <= 0) 
	{
		std::cerr << "Open fifo error" << std::endl;
		return 1;
	}

	communication();

#endif

	return 0;
}

char get_symbol_equv(std::string long_command)
{
	char short_command;

	for (int i = 0; i < NUM_OF_COMMANDS; i++)
	{
		if (!strcmp(long_command.c_str(), Command[i].maxi.c_str())) 
		{
			short_command = Command[i].mini;
			return short_command;
		}
	}

	return 'z';
}

std::string parse_command(char *buffer, int type)
{
	std::string command;

	if (type == 1) 
	{
		std::stringstream ss;
		ss << buffer;
		getline(ss, command, ' ');
	}
	else if (type == 2)
	{
		command = buffer;
		command = command.substr(command.find(" ") + 1,
			sizeof(command) - command.find(" ") - 1);
	}

	return command;
}

#ifdef _WIN32

void communication()
{
	while (true) 
	{
		if (ReadFile(hNamedPipe, buffer, BUF_SIZE, &cbRead, NULL)) 
		{
			std::cout << buffer << std::endl;

			if (!WriteFile(hNamedPipe, buffer, strlen(buffer) + 1, 
				&cbWritten, NULL)) 
			{
				std::cerr << "Pipe connection error" << std::endl;
			}

			switch (get_symbol_equv(parse_command(buffer, 1).c_str())) {
			case 'l': {
				ls_command();
				break;
			}
			case 'r': {
				rm_command();
				break;
			}
			case 'g': {
				get_command();
				break;
			}
			case 'p': {
				put_command();
				break;
			}
			case 'e': {
				return;
			}
			default: {
				std::cout << "Inavalid command, try again" << std::endl;
			}
			}
		}
		else 
		{
			std::cerr << "Pipe connection error(reading)" << std::endl;
		}
	}
}

void ls_command()
{
	hFind = FindFirstFile(PUBLIC_DIR, &FindFileData);
	strcpy_s(buffer, "");

	do {
		if (strcmp(FindFileData.cFileName, ".") &&
			strcmp(FindFileData.cFileName, "..")) 
		{
			strcat_s(buffer, FindFileData.cFileName);
			strcat_s(buffer, "/");
		}

	} while (FindNextFile(hFind, &FindFileData));

	if (!WriteFile(hNamedPipe, buffer, strlen(buffer) + 1, &cbWritten, NULL))
	{
		std::cerr << "Write fifo error" << std::endl;
	}
	CloseHandle(hFind);
}

void get_command() 
{
	std::string filefrom = PUBLIC_FOLDER + parse_command(buffer, 2);
	std::string filefor = CLIENT_FOLDER + parse_command(buffer, 2);
	CopyFile(filefrom.c_str(), filefor.c_str(), TRUE);
	if (remove(filefrom.c_str()) != 0) 
	{
		std::cerr << "No searching file" << std::endl;
	}
}

void put_command() 
{
	std::string filefor = PUBLIC_FOLDER + parse_command(buffer, 2);
	std::string filefrom = CLIENT_FOLDER + parse_command(buffer, 2);
	CopyFile(filefrom.c_str(), filefor.c_str(), TRUE);
	if (remove(filefrom.c_str()) != 0) 
	{
		std::cerr << "No searching file" << std::endl;
	}
}

void rm_command() 
{
	std::string filename = PUBLIC_FOLDER + parse_command(buffer, 2);
	if (remove(filename.c_str()) != 0) 
	{
		std::cerr << "No searching file" << std::endl;
	}
}

#endif

#ifdef __linux__

void communication() 
{
	while (true) 
	{
		memset(readbuffer, '\0', BUF_SIZE);
		memset(buffer, '\0', BUF_SIZE);

		if ((len = read(fd, readbuffer, BUF_SIZE - 1)) <= 0) 
		{
			std::cerr << "Read fifo error" << std::endl;
			close(fd);
			remove(NAMEDPIPE_NAME);
			return;
		}

		switch (get_symbol_equv(parse_command(readbuffer, 1).c_str()))	{
		case 'l': {
			ls_command();
			break;
		}
		case 'r': {
			rm_command();
			break;
		}
		case 'g': {
			get_command();
			break;
		}
		case 'p': {
			put_command();
			break;
		}
		case 'e': {
			close(fd);
			remove(NAMEDPIPE_NAME);
			return;
		}
		default: {
			std::cout << "Inavalid command, try again" << std::endl;
		}
		}
	}
}

void ls_command() 
{
	DIR *dir;
	struct dirent *de;
	std::string dirname;
	dirname = (std::string)PUBLIC_FOLDER_UNIX;

	if (dir = opendir(dirname.c_str()))
	{
		while (de = readdir(dir))
		{
			if (strcmp(de->d_name, ".") &&
				strcmp(de->d_name, ".."))
			{
				strcat(buffer, de->d_name);
				strcat(buffer, "/");
			}
		}
		write(fd, buffer, strlen(buffer));
		closedir(dir);
	}
	else
	{
		std::cerr << "Opendir error" << std::endl;
	}
}

void get_command() 
{
	std::string filefrom = PUBLIC_FOLDER_UNIX + parse_command(readbuffer, 2);
	std::string filefor = CLIENT_FOLDER_UNIX + parse_command(readbuffer, 2);
	rename(filefrom.c_str(), filefor.c_str());
}

void put_command()
{
	std::string filefor = PUBLIC_FOLDER_UNIX + parse_command(readbuffer, 2);
	std::string filefrom = CLIENT_FOLDER_UNIX + parse_command(readbuffer, 2);
	rename(filefrom.c_str(), filefor.c_str());
}

void rm_command() 
{
	std::string filename = PUBLIC_FOLDER_UNIX + parse_command(readbuffer, 2);
	remove(filename.c_str());
}


#endif
