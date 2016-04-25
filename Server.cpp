#include "Server.h"

struct command 
{
	const string maxi;
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
	cout << "Server:" << endl;

#ifdef _MSC_VER

	hNamedPipe = CreateNamedPipe(lpszPipeName, PIPE_ACCESS_DUPLEX, 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, BUF_SIZE, BUF_SIZE, 5000,	NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE) 
	{
		cerr << "CreateNamedPipe error" << endl;
		return 1;
	}

	if (!ConnectNamedPipe(hNamedPipe, NULL)) 
	{
		cerr << "ConnectNamedPipe error" << endl;
		return 1;
	}

	communication();
	
	DisconnectNamedPipe(hNamedPipe);
	CloseHandle(hNamedPipe);

#else
	
	if (mkfifo(NAMEDPIPE_NAME, 0777))
	{
		cerr << "Make fifo error" << endl;
		return 1;
	}
	if ((fd = open(NAMEDPIPE_NAME, O_RDWR)) <= 0) 
	{
		cerr << "Open fifo error" << endl;
		return 1;
	}

	communication();

#endif

	return 0;
}

char get_symbol_equv(string long_command)
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

string parse_command(char *buffer, int type)
{
	string command;

	if (type == 1) 
	{
		stringstream ss;
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

#ifdef _MSC_VER

void communication()
{
	while (true) 
	{
		if (ReadFile(hNamedPipe, buffer, BUF_SIZE, &cbRead, NULL)) 
		{
			cout << buffer << endl;

			if (!WriteFile(hNamedPipe, buffer, strlen(buffer) + 1, 
				&cbWritten, NULL)) 
			{
				cerr << "Pipe connection error" << endl;
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
				cout << "Inavalid command, try again" << endl;
			}
			}
		}
		else 
		{
			cerr << "Pipe connection error(reading)" << endl;
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
		cerr << "Write fifo error" << endl;
	}
	CloseHandle(hFind);
}

void get_command() 
{
	string filefrom = PUBLIC_FOLDER + parse_command(buffer, 2);
	string filefor = CLIENT_FOLDER + parse_command(buffer, 2);
	CopyFile(filefrom.c_str(), filefor.c_str(), TRUE);
	if (remove(filefrom.c_str()) != 0) 
	{
		cerr << "No searching file" << endl;
	}
}

void put_command() 
{
	string filefor = PUBLIC_FOLDER + parse_command(buffer, 2);
	string filefrom = CLIENT_FOLDER + parse_command(buffer, 2);
	CopyFile(filefrom.c_str(), filefor.c_str(), TRUE);
	if (remove(filefrom.c_str()) != 0) 
	{
		cerr << "No searching file" << endl;
	}
}

void rm_command() 
{
	string filename = PUBLIC_FOLDER + parse_command(buffer, 2);
	if (remove(filename.c_str()) != 0) 
	{
		cerr << "No searching file" << endl;
	}
}

#else

void communication() 
{
	while (true) 
	{
		memset(readbuffer, '\0', BUF_SIZE);
		memset(buffer, '\0', BUF_SIZE);

		if ((len = read(fd, readbuffer, BUF_SIZE - 1)) <= 0) 
		{
			cerr << "Read fifo error" << endl;
			close(fd);
			remove(NAMEDPIPE_NAME);
			return;
		}
		cout << readbuffer << endl;

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
			cout << "Inavalid command, try again" << endl;
		}
		}
	}
}

void ls_command() 
{
	DIR *dir;
	struct dirent *de;
	string dirname;
	dirname = (string)PUBLIC_FOLDER_UNIX;

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
		cerr << "Opendir error";
	}
}

void get_command() 
{
	string filefrom = PUBLIC_FOLDER_UNIX + parse_command(readbuffer, 2);
	string filefor = CLIENT_FOLDER_UNIX + parse_command(readbuffer, 2);
	rename(filefrom.c_str(), filefor.c_str());
}

void put_command()
{
	string filefor = PUBLIC_FOLDER_UNIX + parse_command(readbuffer, 2);
	string filefrom = CLIENT_FOLDER_UNIX + parse_command(readbuffer, 2);
	rename(filefrom.c_str(), filefor.c_str());
}

void rm_command() 
{
	string filename = PUBLIC_FOLDER_UNIX + parse_command(readbuffer, 2);
	remove(filename.c_str());
}


#endif
