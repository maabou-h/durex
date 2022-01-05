#include "durex.h"

t_ctx					durex;

int     _servlog(char *s, struct s_client *client) {
	return send(client->fd, s, strlen(s), 0);
}

void	_exit(int errcd) {
	int i = 0;
	if (durex.msock > 0)
		close(durex.msock);
	while (i < MAXCLIENTS)
	{
		if (durex.client[i].fd > 0)
			close(durex.client[i].fd);
		i++;
	}
	exit(errcd);
}

void	_log(char *s, int fd) {
	if (durex.io < 0)
		exit (1);
	write(durex.io, s, strlen(s));
}