#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "console.h"

#define MPTPRO "sexy>> "

static int test(void)
{
	printf("this is a test command.\n");

	return 0;
}

static int fuck(void)
{
	printf("WTF!\n");

	return 0;
}

static void banner(void)
{
	fprintf(stdout, "=====================================\n"
			"=                                   =\n"
			"=    A simple console framework     =\n"
			"=                                   =\n"
			"=====================================\n");

}


int main(int argc, char **argv)
{
	banner();
	console_init(MPTPRO);

	console_add_cmd("test", test, "a test command");
	console_modify_cmd("test", fuck, "modify function of test command");
	console_del_cmd("test");

	console_add_cmd("fuck", fuck, "a sexy command");

	pause();
	return 0;
}
