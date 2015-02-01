/*
 * console.h - A simple console framework
 *
 *  Create On: Feb 1, 2015
 *     Author: Oliver <rootkitsecurity@gmail.com>
 *
 */
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CMDSIZE 20
#define DESSIZE 1024

	typedef struct _cmd_info_ {
		char cmd[CMDSIZE];        /* command name        */
		int (*func)(void);        /* command function   */
		char des[DESSIZE];        /* command description */
		struct _cmd_info_ *next;
	} cmd_info_s;

	typedef struct _cmd_ {
		cmd_info_s *head;
		int nr_cmds;
	} cmd_s;

	int console_init(const char *prompt);
	int console_add_cmd(const char *cmdname, int (*func)(void), const char *des);
	int console_del_cmd(const char *cmdname);
	int console_modify_cmd(const char *cmdname, int (*func)(void), const char *ndes);

#ifdef __cplusplus
}
#endif

#endif
