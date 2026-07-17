/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"

const char *
run_command(const char *cmd)
{
	char *p;
	FILE *fp;

	if (!(fp = popen(cmd, "r"))) {
		warn("popen '%s':", cmd);
		return NULL;
	}
	p = fgets(buf, sizeof(buf) - 1, fp);
	/* A parent (e.g. dwm) may set SIGCHLD to SIG_IGN / SA_NOCLDWAIT, which is
	 * inherited here and makes the kernel auto-reap the popen child before
	 * pclose() can wait() for it. pclose() then fails with ECHILD even though
	 * the command ran fine and its output was already read above -- so treat
	 * ECHILD as success rather than discarding valid output. */
	if (pclose(fp) < 0 && errno != ECHILD) {
		warn("pclose '%s':", cmd);
		return NULL;
	}
	if (!p) {
		return NULL;
	}
	if ((p = strrchr(buf, '\n'))) {
		p[0] = '\0';
	}

	return buf[0] ? buf : NULL;
}
