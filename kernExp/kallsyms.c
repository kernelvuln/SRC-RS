#include <stdio.h>
#include <string.h>

typedef int __attribute__((regparm(3))) (* commit_creds_fn)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* prepare_kernel_cred_fn)(unsigned long cred);

commit_creds_fn commit_creds;
prepare_kernel_cred_fn prepare_kernel_cred;

/* Thx Spender. 
 * https://grsecurity.net/~spender/
 * */
unsigned long get_symbol(char *target_name) {
	FILE *f;
	unsigned long addr;
	char dummy;
	char name[256];
	int ret = 0;

	f = fopen("/proc/kallsyms", "r");
	if (f == NULL)
		return 0;

	while (ret != EOF) {
		ret = fscanf(f, "%p %c %s\n", (void **)&addr, &dummy, name);
		if (ret == 0) {
			fscanf(f, "%s\n", name);
			continue;
		}

		if (!strcmp(name, target_name)) {
			printf("[+] Resolved %s: %p\n", target_name, (void *)addr);

			fclose(f);
			return addr;
		}
	}

	printf("[-] Couldn't resolve \"%s\"\n", name);

	fclose(f);
	return 0;
}

int main(int argc, char **argv)
{
	commit_creds = (commit_creds_fn)get_symbol("commit_creds");
	prepare_kernel_cred = (prepare_kernel_cred_fn)get_symbol("prepare_kernel_cred");

	printf("[+] commit_creads: %p\n", commit_creds);
	printf("[+] prepare_kernel_cred: %p\n", prepare_kernel_cred);

	return 0;
}
