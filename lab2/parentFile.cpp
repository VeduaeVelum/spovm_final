#include "mySigProcessor.h"

int main(int argc, char* argv[])
{
	int loops = atoi(argv[1]);
	int** massives = NULL;
	std::fstream file;

	pid_massive = new PID_T[loops];

	srand(time(NULL));

	if (check_loops(loops) == false)
	{
		return 1;
	}

	massives = memAlloc_matrix(massives, loops, MASSIVELENGTH);
	massives = rand_matrix(massives, loops, MASSIVELENGTH);
	print_menu_and_massives(massives, loops);
	massives = flag_divider(massives, file, loops);
	if (massives == NULL)
	{
		return 1;
	}
	memFree_matrix(massives, loops);
	delete[] pid_massive;
	SYSTEMPAUSE
	return 0;
}

void 	print_menu_and_massives(int** massives, int loops)
{
	std::cout << "Menu functions: " << std::endl;
	
	std::cout << "'+' - sort the next massive" << std::endl;
	std::cout << "'-' - delete the last sorting process" << std::endl;
	std::cout << "'q' - close the program" << std::endl;
	std::cout << "Original massives: " << std::endl;

	show_matrix(massives, loops, MASSIVELENGTH);
}
int** 	memAlloc_matrix(int** matrix, int width, int height)
{
	int i, j;
	try
	{
		matrix = new int*[width];
		for (i = 0; i < width; i++)
		{
			matrix[i] = new int[height];
		}
	}
	catch (std::bad_alloc)
	{
		std::cout << "Memory couldn't be allocated, \
					 program will stop" << std::endl;
		if (matrix != NULL)
		{
			for (j = 0; j < i; j++)
			{
				delete[] matrix[j];
			}
			delete matrix;
		}
		exit(EXIT_FAILURE);
	}
	return matrix;
}
int** 	rand_matrix(int** matrix, int width, int height)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			matrix[i][j] = RAND_SUB + rand() % RAND_BOUND;
		}
	}
	return matrix;
}
void 	show_matrix(int** matrix, int width, int height)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			std::cout.width(NUMBERWIGHT);
			std::cout << matrix[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void 	memFree_matrix(int** matrix, int width)
{
	for (int i = 0; i < width; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}
bool 	check_loops(int loops)
{
	if (loops < DOWNBOARD || loops > UPBOARD)
	{
		std::cout << "Incorrect arguments, repeat program \
					 (use argument from 1 to 300)" << std::endl;
		return false;
	}
	return true;
}
bool 	input_file(int* massive, std::fstream& file)
{
	file.open(MASSIVESFILEPATH, std::ios::out | std::ios::trunc);
	if (!file.is_open())
	{
		std::cout << "File can't be opened for writing. \
					 Maybe it doesn't exist" << std::endl;
		return false;
	}
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file << massive[j] << ' ';
	}
	file.close();
	return true;
}
int* 	output_file(int* massive, std::fstream& file)
{
	file.open(MASSIVESFILEPATH, std::ios::in);
	if (!file.is_open())
	{
		std::cout << "File can't be opened for reading" << std::endl;
		delete massive;
		return NULL;
	}
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file >> massive[j];
	}
	file.close();
	return massive;
}

void 	user_menu(int current_pros_num, int loops)
{
	char ch;

	ADD_PROS_ACTION
	KILL_PROS_ACTION
	TERM_ACTION
	CLEARSTDIN
	ch = getchar();
	switch (ch)
	{
	case '+': {
		if (current_pros_num < loops)
		{
			ADD_SIG_PULSE
			addProsFlag = true;
			menuFlag = false;
		}
		else
		{
			std::cout << "You got a limit of processes" << std::endl;
		}
		break;
	}
	case '-': {
		if (current_pros_num > 0)
		{
			KILL_SIG_PULSE
			killProsFlag = true;
			menuFlag = false;
		}
		else
		{
			std::cout << "You have not any process yet" << std::endl;
		}
		break;
	}
	case 'q': {
		OUT_SIG_PULSE
		outFlag = true;
		menuFlag = false;
		break;
	}
	default: {
		std::cout << "Undefined reference, try again" << std::endl;
	}
	}
}

int** 	flag_divider(int** massives, std::fstream& file, int loops)
{
	int i = 0;
	bool trigger = false;

	while (outFlag == false)
	{
		if (menuFlag == true)
		{
			user_menu(i, loops);
		}
		if (addProsFlag == true && i < loops)
		{
			massives[i] = call_massive_sort(massives[i], file, i);
			if (massives[i] == NULL)
			{
				memFree_matrix(massives, loops);
				return NULL;
			}
			std::cout << "Current process is working: ";
			SHOW_PID(i);
			i++;
			addProsFlag = false;
			trigger = true;
			menuFlag = true;
		}
		else if (killProsFlag == true && i > 0)
		{
			if (trigger == true)
			{
				i--;
				trigger = false;
			}
			KILL_CHILD_SIG_PULSE
			std::cout << "Current process was killed: ";
			SHOW_PID(i);
			if (i != 0)
			{
				i--;
			}
			killProsFlag = false;
			menuFlag = true;
		}
		SLEEP(1);
	}
	kill_working_processes(i);
	return massives;
}

int* 	call_massive_sort(int* massive, std::fstream& file, int current_pros_num)
{
	int status = 0;

	if (input_file(massive, file) == false)
	{
		delete massive;
		return NULL;
	}

	else if (create_process(status, current_pros_num) == false)
	{
		file.close();
		delete massive;
		return NULL;
	}

	massive = output_file(massive, file);

	if (massive == NULL)
	{
		return NULL;
	}
	return massive;
}

void 	kill_working_processes(int current_pros_num)
{
	for (int i = 0; i < current_pros_num; i++)
	{
		KILL_CHILD_SIG_PULSE
	}
}