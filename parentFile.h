#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>
#include <stdio_ext.h>
#include <wait.h>
#define SYSTEMPAUSE std::cout << "Нажмите любую клавишу для выхода..." \
	<< std::endl;\
	getchar();
#define CLEARSTDIN __fpurge(stdin);
#define MASSIVESFILEPATH "/home/erick_vb/spo/lab2/massives.txt"
#define SLEEP(X) sleep(X);
#define ADD_SIG_PULSE raise(SIGUSR1);
#define KILL_SIG_PULSE raise(SIGUSR2);
#define OUT_SIG_PULSE raise(SIGTERM);
#define KILL_CHILD_SIG_PULSE kill(pid_massive[i], SIGTERM);
#define SHOW_PID(X) std::cout << pid_massive[X] << std::endl;

typedef pid_t PID_T;
PID_T *pid_massive;

bool 	create_process(int status, int current_pros_num)
{
	pid_massive[current_pros_num] = fork();
	if (pid_massive[current_pros_num] < 0)
	{
		std::cout << "Error creation process" << std::endl;
		return false;
	}
	else if (pid_massive[current_pros_num] == 0)
	{
		execl("/home/erick_vb/spo/lab2/ch", "", NULL, NULL);
	}
	return true;
}
#endif

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#include <process.h>
#include <WinBase.h>
#define MASSIVESFILEPATH "C:\spovmProjects\lab2\massives.txt"
#define SYSTEMPAUSE system("pause");
#define CLEARSTDIN fflush(stdin);
#define SLEEP(X) Sleep(X);
#define ADD_SIG_PULSE HANDLE add_pros_ev2 = \
	OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"add_pros_handler"); \
	PulseEvent(add_pros_ev2);
#define KILL_SIG_PULSE HANDLE kill_pros_ev2 = \
	OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"kill_pros_handler"); \
	PulseEvent(kill_pros_ev2);
#define OUT_SIG_PULSE HANDLE out_pros_ev2 = \
	OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"term_handler"); \
	PulseEvent(out_pros_ev2);
#define KILL_CHILD_SIG_PULSE HANDLE kill_child_ev = 
	OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPCWSTR)"child_term_handler"); \
	PulseEvent(kill_child_ev);
#define SHOW_PID(X) std::cout << pid_massive[X].dwProcessId << std::endl;

typedef PROCESS_INFORMATION PID_T;
PID_T *pid_massive;
HANDLE add_pros_ev1, add_pros_ev2;
HANDLE kill_pros_ev1, kill_pros_ev2;
HANDLE out_pros_ev1, out_pros_ev2;
HANDLE kill_child_ev;

bool	create_process(int status, int current_pros_num)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	TCHAR ChildProcess[] = TEXT("C:\\spovmProjects\\lab2\\child\\Debug\\child.exe");
	if (CreateProcess(NULL, ChildProcess,
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == FALSE)
	{
		std::cout << "Error creation process" << std::endl;
		return false;
	}
	pid_massive[current_pros_num] = pi;
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}
#endif

const int MASSIVELENGTH = 10;
const int NUMBERWIGHT = 6;
const int DOWNBOARD = 1;
const int UPBOARD = 300;
const int RAND_BOUND = 1000;
const int RAND_SUB = -500;

void 	print_menu_and_massives(int**, int);
int** 	memAlloc_matrix(int**, int, int);
int** 	rand_matrix(int**, int, int);
void 	show_matrix(int**, int, int);
void 	memFree_matrix(int**, int);
bool 	input_file(int*, std::fstream&);
int* 	output_file(int*, std::fstream&);
bool 	create_process(int, int);
bool 	check_loops(int);

void 	user_menu(int, int);
int** 	flag_divider(int**, std::fstream&, int);
int* 	call_massive_sort(int*, std::fstream&, int);
void 	kill_working_processes(int);