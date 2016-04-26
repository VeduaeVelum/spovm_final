#include <iostream>

const int BUF_SIZE = 512;
const int NUM_OF_COMMANDS = 5;
const std::string COM_LS = "ls";
const std::string COM_EXIT = "exit";
const std::string COM_RMFILE = "rm";
const std::string COM_GETFILE = "get";
const std::string COM_PUTFILE = "put";

char buffer[BUF_SIZE];
char readbuffer[BUF_SIZE];

std::string	parse_command(char*, int);
void	communication();
void	ls_command();
void	put_command();
void	get_command();
void	rm_command();
char	get_symbol_equv(std::string);
