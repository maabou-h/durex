#include "durex.h"

void		_sighandler(void)
{
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP, _exit);
	signal(SIGINT, _exit);
	signal(SIGQUIT, _exit);
	signal(SIGABRT, _exit);
	signal(SIGILL, _exit);
	signal(SIGFPE, _exit);
	signal(SIGPIPE, _exit);
	signal(SIGSEGV, _exit);
	signal(SIGALRM, _exit);
	signal(SIGUSR1, _exit);
	signal(SIGUSR2, _exit);
	signal(SIGCONT, _exit);
	signal(SIGTERM, _exit);
}