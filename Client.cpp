#include "Client.h"

int main(int argc, char *argv[])
{
	cout << "Client:" << endl;
	showMenu();

#ifdef _WIN32

	hNamedPipe = CreateFile(szPipeName,	GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE) 
	{
		cerr << "CreateNamedPipe error";
		return 1;
	}

	ñommunication();

	CloseHandle(hNamedPipe);

#endif

#ifdef __linux__

	if (mkfifo(NAMEDPIPE_NAME, 0777)) 
	{
		perror("mkfifo");
	}
	if ((fd = open(NAMEDPIPE_NAME, O_RDWR)) <= 0) 
	{
		perror("Openfile");
		return 1;
	}

	ñommunication();

#endif

	return 0;
}

void showFiles(char *buffer, string folder) 
{
	string file;
	stringstream ss;

	ss << buffer;

	while (getline(ss, file, '/')) 
	{
		cout << folder << "/" << file << endl;
	}
}

void showMenu() 
{
	cout << "Comands:" << endl;
	cout << COM_LS << " - print files on server" << endl;
	cout << COM_GETFILE << " - get file from server(+input name)" << endl;
	cout << COM_PUTFILE << " - add file to server(+input name)" << endl;
	cout << COM_RMFILE << " - delete file(+input name)" << endl;
	cout << COM_HISTORY << " - show history of requests" << endl;
	cout << COM_EXIT  << " - exit from program" << endl;
}

bool input_file(const char* command, fstream& file)
{
	file.open(HIST_FILE_NAME, ios::out | ios::app);
	if (!file.is_open())
	{
		cerr << "File can't be opened for writing" << endl;
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

bool output_file(fstream& file)
{
	char fileread[BUF_SIZE];
	file.open(HIST_FILE_NAME, ios::in);
	cout << "--------History of requests-----------" << endl;

	while (!file.eof())
	{
		if (file.eof())
		{
			break;
		}
		file.getline(fileread, BUF_SIZE);
		cout << fileread << endl;
	}

	cout << "--------End of history-----------" << endl;
	return true;
}

#ifdef _WIN32

void ñommunication() 
{
	fstream hist_file;
	const char* char_buf;
	while (true) 
	{
		getline(cin, buf);
		strcpy_s(buffer, buf.c_str());
		char_buf = buf.c_str();
		if (!strcmp(buffer, COM_HISTORY.c_str()))
		{
			if (!output_file(hist_file))
			{
				cerr << "Error reading history" << endl;
			}
			continue;
		}
		if (!WriteFile(hNamedPipe, buffer, strlen(buffer) + 1,
			&cbWritten, NULL))
		{
			cerr << "Server connection error" << endl;
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
					cerr << "Pipe connection error(reading)" 
						<< endl;
				}
			}
			else if (!strcmp(buffer, COM_EXIT.c_str())) 
			{
				break;
			}
		}
		else
		{
			cerr << "Pipe connection error(reading)" << endl;
		}
	}
}

#endif

#ifdef __linux__

void ñommunication() 
{
	fstream hist_file;
	const char* char_buf;
	while (true) 
	{
		memset(readbuffer, '\0', BUF_SIZE);
		memset(buffer, '\0', BUF_SIZE);

		getline(cin, buf);
		strcpy(buffer, buf.c_str());
		if (!strcmp(buffer, COM_HISTORY.c_str()))
		{
			if (!output_file(hist_file))
			{
				cerr << "Error reading history" << endl;
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
			cout << "Files on server:" << endl;
			if ((len = read(fd, readbuffer, BUF_SIZE - 1)) > 0) 
			{
				showFiles(readbuffer, "public");
			}
			else
			{
				cerr << "Pipe connection error(reading)" 
					<< endl;
				close(fd);
				remove(NAMEDPIPE_NAME);
				break;
			}
		}
	}
}

#endif
