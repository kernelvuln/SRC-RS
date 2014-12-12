#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

typedef int __attribute__((regparm(3))) (* commit_creds_fn)(unsigned long cred);
typedef unsigned long __attribute__((regparm(3))) (* prepare_kernel_cred_fn)(unsigned long cred);

commit_creds_fn commit_creds;
prepare_kernel_cred_fn prepare_kernel_cred;
commit_creds_fn __commit_creds;
prepare_kernel_cred_fn __prepare_kernel_cred;

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

/* Thx Spender. 
 * https://grsecurity.net/~spender/
 * */
static unsigned long get_kernel_sym(char *name)
{
	FILE *f;
	unsigned long addr;
	char dummy;
	char sname[512];
	struct utsname ver;
	int ret;
	int rep = 0;
	int oldstyle = 0;

	f = fopen("/proc/kallsyms", "r");
	if (f == NULL) {
		f = fopen("/proc/ksyms", "r");
		if (f == NULL)
			goto fallback;
		oldstyle = 1;
	}

repeat:
	ret = 0;
	while(ret != EOF) {
		if (!oldstyle)
			ret = fscanf(f, "%p %c %s\n", (void **)&addr, &dummy, sname);
		else {
			ret = fscanf(f, "%p %s\n", (void **)&addr, sname);
			if (ret == 2) {
				char *p;
				if (strstr(sname, "_O/") || strstr(sname, "_S."))
					continue;
				p = strrchr(sname, '_');
				if (p > ((char *)sname + 5) && !strncmp(p - 3, "smp", 3)) {
					p = p - 4;
					while (p > (char *)sname && *(p - 1) == '_')
						p--;
					*p = '\0';
				}
			}
		}
		if (ret == 0) {
			fscanf(f, "%s\n", sname);
			continue;
		}
		if (!strcmp(name, sname)) {
			fprintf(stdout, "[+] Resolved %s to %p%s\n", name, (void *)addr, rep ? " (via System.map)" : "");
			fclose(f);
			return addr;
		}
	}

	fclose(f);
	if (rep)
		return 0;
fallback:
	/* didn't find the symbol, let's retry with the System.map
	   dedicated to the pointlessness of Russell Coker's SELinux
	   test machine (why does he keep upgrading the kernel if
	   "all necessary security can be provided by SE Linux"?)
	*/
	uname(&ver);
	if (strncmp(ver.release, "2.6", 3) || strncmp(ver.release, "3.", 2))
		oldstyle = 1;
	sprintf(sname, "/boot/System.map-%s", ver.release);
	f = fopen(sname, "r");
	if (f == NULL)
		return 0;
	rep = 1;
	goto repeat;
}

int main(int argc, char **argv)
{
	commit_creds = (commit_creds_fn)get_symbol("commit_creds");
	prepare_kernel_cred = (prepare_kernel_cred_fn)get_symbol("prepare_kernel_cred");
	__commit_creds = (commit_creds_fn)get_kernel_sym("commit_creds");
	__prepare_kernel_cred = (prepare_kernel_cred_fn)get_kernel_sym("prepare_kernel_cred");

	printf("[+] (get_symbol) commit_creads: %p\n", commit_creds);
	printf("[+] (get_symbol) prepare_kernel_cred: %p\n", prepare_kernel_cred);
	printf("[+] (get_kernel_sym) commit_creads: %p\n", __commit_creds);
	printf("[+] (get_kernel_sym) prepare_kernel_cred: %p\n", __prepare_kernel_cred);

	return 0;
}
