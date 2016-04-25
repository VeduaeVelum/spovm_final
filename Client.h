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

#define NAMEDPIPE_NAME "/tmp/server"

int fd, len;

#endif

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

const int BUF_SIZE = 512;
const string COM_LS = "ls";
const string COM_EXIT = "exit";
const string COM_RMFILE = "rm";
const string COM_GETFILE = "get";
const string COM_PUTFILE = "put";
const string COM_HISTORY = "hist";
bool is_first_command = true;

char	buffer[BUF_SIZE];
char	readbuffer[BUF_SIZE];
string	buf;

bool	input_file(const char*, fstream&);
bool 	output_file(fstream&);
void	showFiles(char*, string);
void	showMenu();
void	ñommunication();