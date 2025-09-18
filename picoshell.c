

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>

int picoshell(char **cmds[])
{
	pid_t pid;
	int pipefd[2];
	int prev_fd = -1;
	int i = 0;
	while(cmds[i])
	{
		if(cmds[i + 1] && pipe(pipefd))
			return(1);
		pid = fork();
		if(pid == -1)
		{
			if(cmds[i + 1])
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return(1);
		}
		if(pid == 0)
		{
			if(prev_fd != -1)
			{
				if(dup2(prev_fd, STDIN_FILENO) == -1)
					exit(1);
				close(prev_fd);
			}
			if(cmds[i + 1])
			{
				close(pipefd[0]);
				if(dup2(pipefd[1], STDOUT_FILENO) == -1)
					exit(1);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if(prev_fd != -1)
			close(prev_fd);
		if(cmds[i + 1])
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		i++;
	}
	while(wait(NULL) > 0);
	return(0);
}

int main(int argc, char **argv)
{
	int cmds_size = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
			cmds_size++;
	}
	char ***cmds = calloc(cmds_size + 1, sizeof(char **));
	if (!cmds)
	{
		dprintf(2, "Malloc error: %m\n");
		return 1;
	}
	cmds[0] = argv + 1;
	int cmds_i = 1;
	for (int i = 1; i < argc; i++)
		if (!strcmp(argv[i], "|"))
		{
			cmds[cmds_i] = argv + i + 1;
			argv[i] = NULL;
			cmds_i++;
		}
	int ret = picoshell(cmds);
	if (ret)
		perror("picoshell");
	free(cmds);
	return ret;
}