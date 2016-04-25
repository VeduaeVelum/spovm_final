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
#include <iostream>

using namespace std;

const int BUF_SIZE = 512;
const int NUM_OF_COMMANDS = 5;
const string COM_LS = "ls";
const string COM_EXIT = "exit";
const string COM_RMFILE = "rm";
const string COM_GETFILE = "get";
const string COM_PUTFILE = "put";

char buffer[BUF_SIZE];
char readbuffer[BUF_SIZE];

string	parse_command(char*, int);
void	communication();
void	ls_command();
void	put_command();
void	get_command();
void	rm_command();
char	get_symbol_equv(string);
