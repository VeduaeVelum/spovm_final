#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>

bool outFlag = false;

#ifdef __linux__
#include <unistd.h>
#include <wait.h>
#define MASSIVESFILEPATH "/home/erick_vb/spo/lab2/massives.txt"
#define CHILD_TERM_ACTION	struct sigaction sa_term;\
	set_sig_mask(); \
	sa_term.sa_handler = term_handler; \
	sigaction(SIGTERM, &sa_term, 0);

#define SLEEP(X) sleep(X);
void term_handler(int i)
{
	outFlag = true;
}
void set_sig_mask()
{
	sigset_t newset;
	sigemptyset(&newset);
	sigaddset(&newset, SIGHUP);
	sigprocmask(SIG_BLOCK, &newset, 0);
}

#endif

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#include <process.h>
#include <WinBase.h>
#define MASSIVESFILEPATH "C:\spovmProjects\lab2\massives.txt"
#define SLEEP(X) Sleep(X);
#define CHILD_TERM_ACTION ch_term_ev1 = CreateEvent(NULL, TRUE, \
	FALSE, (LPCWSTR)"term_handler"); \
	ch_term_ev2 = CreateEvent(NULL, TRUE, FALSE, NULL); \
	_beginthread(child_term_handler, 0, NULL);

HANDLE ch_term_ev1, ch_term_ev2; 

void child_term_handler(void *pParams)
{
	outFlag = true;
}
#endif

const int MASSIVELENGTH = 10;
const int NUMBERWIGHT = 6;

int* get_massive(int*, std::fstream&);
bool save_sort_massive(int*, std::fstream&);
void show_massive(int*);