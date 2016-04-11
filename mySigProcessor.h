#include "parentFile.h"

bool addProsFlag = false;
bool killProsFlag = false;
bool outFlag = false;
bool menuFlag = true;

#ifdef __linux__
#define ADD_PROS_ACTION	struct sigaction sa_add;\
	sa_add.sa_flags = 0; \
	sa_add.sa_restorer = 0; \
	sigemptyset(&sa_add.sa_mask); \
	set_sig_mask(); \
	sa_add.sa_handler = add_pros_handler; \
	sigaction(SIGUSR1, &sa_add, 0);

#define KILL_PROS_ACTION struct sigaction sa_kill;\
	sa_kill.sa_flags = 0; \
	sa_kill.sa_restorer = 0; \
	sigemptyset(&sa_kill.sa_mask); \
	set_sig_mask(); \
	sa_kill.sa_handler = kill_pros_handler; \
	sigaction(SIGUSR2, &sa_kill, 0);

#define TERM_ACTION	struct sigaction sa_term;\
	sa_term.sa_flags = 0; \
	sa_term.sa_restorer = 0; \
	sigemptyset(&sa_term.sa_mask); \
	set_sig_mask(); \
	sa_term.sa_handler = term_handler; \
	sigaction(SIGTERM, &sa_term, 0);

void add_pros_handler(int i)
{
	addProsFlag = true;
	std::cout << "addProsFlag = true" << std::endl;
}
void kill_pros_handler(int i)
{
	killProsFlag = true;
	std::cout << "killProsFlag = true" << std::endl;
}
void term_handler(int i)
{
	outFlag = true;
	std::cout << "outFlag = true" << std::endl;
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

#define ADD_PROS_ACTION	 add_pros_ev1 = CreateEvent(NULL, TRUE, \
	FALSE, (LPCWSTR)"add_pros_handler"); \
	add_pros_ev2 = CreateEvent(NULL, TRUE, FALSE, NULL); \
	_beginthread(add_pros_handler, 0, NULL); 

#define KILL_PROS_ACTION kill_pros_ev1 = CreateEvent(NULL, TRUE, \
	FALSE, (LPCWSTR)"kill_pros_handler"); \
	kill_pros_ev2 = CreateEvent(NULL, TRUE, FALSE, NULL); \
	_beginthread(kill_pros_handler, 0, NULL);

#define TERM_ACTION	 out_pros_ev1 = CreateEvent(NULL, TRUE, \
	FALSE, (LPCWSTR)"term_handler"); \
	out_pros_ev2 = CreateEvent(NULL, TRUE, FALSE, NULL); \
	_beginthread(term_handler, 0, NULL);

void add_pros_handler(void* pParams)
{
	while (TRUE)
	{
		WaitForSingleObject(add_pros_ev2, INFINITE);
		addProsFlag = true;
		std::cout << "addProsFlag = true" << std::endl;
	}
}
void kill_pros_handler(void* pParams)
{
	while (TRUE)
	{
		WaitForSingleObject(kill_pros_ev2, INFINITE);
		killProsFlag = true;
		std::cout << "killProsFlag = true" << std::endl;
	}
}
void term_handler(void* pParams)
{
	while (TRUE)
	{
		WaitForSingleObject(out_pros_ev2, INFINITE);
		outFlag = true;
		std::cout << "outFlag = true" << std::endl;
	}
}
#endif