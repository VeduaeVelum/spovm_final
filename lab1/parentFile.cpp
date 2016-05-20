#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#include <wait.h>
#define SYSTEMPAUSE getchar()
#define MASSIVESFILEPATH "/home/erick_vb/spo/lab1/massives.txt"
bool 	create_process(int status)
{
	pid_t pid = fork();
	if (pid < 0)
	{
		std::cout << "Error creation process, program will stop" << std::endl;
		return false;
	}
	else if (pid == 0)
	{
		execl("/home/erick_vb/spo/lab1/ch", "", NULL, NULL);
	}
	else if (waitpid(pid, &status, 0) != pid)
	{
		status = -1;
	}
	return true;
}
#endif

#ifdef _WIN32
#include <Windows.h>
#define MASSIVESFILEPATH "C:\spovmProjects\lab1\massives.txt"
#define SYSTEMPAUSE system("pause");
bool	create_process(int status)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	TCHAR ChildProcess[] = TEXT("child.exe");
	if (CreateProcess(NULL, ChildProcess,
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == FALSE)
	{
		std::cout << "Error creation process" << std::endl;
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}
#endif

using namespace std;

const int MASSIVELENGTH = 10;
const int NUMBERWIGHT = 6;
const int DOWNBOARD = 1;
const int UPBOARD = 300;

int** 	memAlloc_matrix(int**, int, int);
int** 	rand_matrix(int**, int, int);
void 	show_matrix(int**, int, int);
void 	memFree_matrix(int**, int);
bool 	input_file(int*, fstream&);
int* 	output_file(int*, fstream&);
bool 	create_process(int);
bool 	check_loops(int);

int main(int argc, char* argv[])
{
	int loops = atoi(argv[1]), status = 0;
	int** massives = NULL;
	fstream file;

	srand(time(NULL));

	if (check_loops(loops) == false) return 1;

	massives = memAlloc_matrix(massives, loops, MASSIVELENGTH);
	massives = rand_matrix(massives, loops, MASSIVELENGTH);
	cout << "Original massives: " << endl;
	show_matrix(massives, loops, MASSIVELENGTH);
	for (int i = 0; i < loops; i++)
	{
		if (input_file(massives[i], file) == false)
		{
			memFree_matrix(massives, loops);
			return 1;
		}

		if (create_process(status) == false)
		{
			file.close();
			memFree_matrix(massives, loops);
			return 1;
		}

		massives[i] = output_file(massives[i], file);
		if (massives[i] == NULL)
		{
			memFree_matrix(massives, loops);
			return 1;
		}
	}
	cout << "Sorted massives: " << endl;
	show_matrix(massives, loops, MASSIVELENGTH);
	memFree_matrix(massives, loops);
	SYSTEMPAUSE;
	return 0;
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
	catch (bad_alloc)
	{
		cout << "Memory couldn't be allocated, program will stop" << endl;
		if (matrix != NULL)
		{
			for (j = 0; j < i; j++)
			{
				delete[] matrix[j];
			}
			delete matrix;
		}
		exit(-3);
	}
	return matrix;
}
int** 	rand_matrix(int** matrix, int width, int height)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			matrix[i][j] = -500 + rand() % 1000;
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
			cout.width(NUMBERWIGHT);
			cout << matrix[i][j];
		}
		cout << endl;
	}
	cout << endl;
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
		cout << "Incorrect arguments, repeat program(use argument from 1 to 10000)" << endl;
		return false;
	}
	return true;
}
bool 	input_file(int* massive, fstream& file)
{
	file.open(MASSIVESFILEPATH, ios::out | ios::trunc);
	if (!file.is_open())
	{
		cout << "File can't be opened for writing. Maybe it doesn't exist" << endl;
		return false;
	}
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file << massive[j] << ' ';
	}
	file.close();
	return true;
}
int* 	output_file(int* massive, fstream& file)
{
	file.open(MASSIVESFILEPATH, ios::in);
	if (!file.is_open())
	{
		cout << "File can't be opened for reading" << endl;
		return NULL;
	}
	for (int j = 0; j < MASSIVELENGTH; j++)
	{
		file >> massive[j];
	}
	file.close();
	return massive;
}