/*
 * console.c - A simple console framework
 *
 *  Create On: Feb 1, 2015
 *     Author: Oliver <rootkitsecurity@gmail.com>
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "console.h"

#define PROMPT 11

static int help(void);
static int quit(void);

static char inited = 0;
static pthread_t conpid;
static char g_prompt[PROMPT] = {0};

static cmd_s *command_list = NULL;

static int help(void)
{
	int idx = 1;
	cmd_info_s *cmd_info = command_list->head;

	while (cmd_info) {
		fprintf(stdout, "%4d: %s  -  %s\n", idx++, cmd_info->cmd, cmd_info->des);
		cmd_info = cmd_info->next;
	}

	return 0;
}

static int quit(void)
{
	exit(EXIT_SUCCESS);
}

static void *console_deal(void *arg)
{
	char input_cmd[20];
	cmd_info_s *cmd_info;

	pthread_detach(pthread_self());
	memcpy(g_prompt, (char *)arg, PROMPT - 1);
	
	while (printf("%s", g_prompt) && fgets(input_cmd, sizeof(input_cmd), stdin)) {
		cmd_info =  command_list->head;
		input_cmd[strlen(input_cmd) - 1] = '\0';

		if (input_cmd[0] == '\0')
			continue;

		while (cmd_info) {
			if (!strcmp(cmd_info->cmd, input_cmd))
				break;
			cmd_info = cmd_info->next;
		}

		if (cmd_info == NULL) {
			fprintf(stderr, "%s: command not found\n", input_cmd);
			continue;
		}

		(*cmd_info->func)();
	}

	return 0;
}

int console_modify_cmd(const char *cmdname, int (*nfunc)(void), const char *ndes)
{
	cmd_info_s *cmd_info = command_list->head;

	while (cmd_info) {
		if (strcmp(cmd_info->cmd, cmdname) == 0) {
			cmd_info->func = nfunc;
			memcpy(cmd_info->des, ndes, strlen(ndes) + 1);
		}
		cmd_info = cmd_info->next;
	}
	return 0;
}

int console_add_cmd(const char *cmdname, int (*func)(void), const char *des)
{
	cmd_info_s *cmd_info = NULL;

	if (!inited) {
		fprintf(stderr, "console not init\n");
		return -1;
	}

	cmd_info = (cmd_info_s *)calloc(1, sizeof(cmd_info_s));
	if (cmd_info == NULL) {
		fprintf(stderr, "calloc failed!\n");
		return -1;
	}

	memcpy(cmd_info->cmd, cmdname, CMDSIZE);
	memcpy(cmd_info->des, des, DESSIZE);
	cmd_info->func = func;
	cmd_info->next = NULL;

	if (command_list != NULL)
		cmd_info->next = command_list->head;

	command_list->head = cmd_info;
	command_list->nr_cmds++;

	return 0;
}

int console_del_cmd(const char *cmdname)
{
	cmd_info_s *cmd_info, *cmd_pre;

	cmd_info = cmd_pre = command_list->head;

	if (!strcmp(cmdname, "help") || !strcmp(cmdname, "quit")) {
		fprintf(stderr, "You cannot delete command of help or quit.\n");
		return -1;
	}

	while (cmd_info) {
		if (strcmp(cmdname, command_list->head->cmd) == 0) {
			command_list->head = cmd_info->next;
			free(cmd_info);
			cmd_info = cmd_pre = command_list->head;
			continue;
		}

		if (strcmp(cmdname, cmd_info->cmd) == 0) {
			cmd_pre->next = cmd_info->next;
			free(cmd_info);
			cmd_info = cmd_info->next;
			continue;
		}

		cmd_pre = cmd_info;
		cmd_info = cmd_info->next;
	}

	return 0;
}

int console_init(const char *prompt) 
{
	if (inited)
		return 0;

	command_list = (cmd_s *)calloc(1, sizeof(cmd_s));
	if (command_list == NULL) {
		fprintf(stderr, "calloc failed\n");
		return -1;
	}

	command_list->nr_cmds = 0;
	command_list->head = NULL;
	
	if (pthread_create(&conpid, NULL, console_deal, (void *)prompt) < 0)
		return -1;

	inited = 1;

	console_add_cmd("help", help, "display help information");
	console_add_cmd("quit", quit, "quit console");

	return 0;
}
