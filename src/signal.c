#include "durex.h"

void		_sighandler(void)
{
	signal(SIGHUP, _exit);
	signal(SIGINT, _exit);
	signal(SIGQUIT, _exit);
	signal(SIGILL, _exit);
	signal(SIGTRAP, _exit);
	signal(SIGABRT, _exit);
	signal(SIGBUS, _exit);
	signal(SIGFPE, _exit);
	signal(SIGSEGV, _exit);
	signal(SIGPIPE, _exit);
	signal(SIGALRM, _exit);
	signal(SIGTERM, _exit);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCONT, _exit);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGUSR1, _exit);
	signal(SIGUSR2, _exit);
}