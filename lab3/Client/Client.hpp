#include <iostream>
#include <fstream>

const int BUF_SIZE = 512;
const std::string COM_LS = "ls";
const std::string COM_EXIT = "exit";
const std::string COM_RMFILE = "rm";
const std::string COM_GETFILE = "get";
const std::string COM_PUTFILE = "put";
const std::string COM_HISTORY = "hist";
bool is_first_command = true;

char	buffer[BUF_SIZE];
char	readbuffer[BUF_SIZE];
std::string	buf;

bool	input_file(const char*, std::fstream&);
bool 	output_file(std::fstream&);
void	showFiles(char*, std::string);
void	showMenu();
void	ñommunication();