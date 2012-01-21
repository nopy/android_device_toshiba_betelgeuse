/*
 * Simple hotplug tool
 *
 * by Chih-Wei Huang <cwhuang@linux.org.tw>  2009/08/03
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define HOTPLUG_LOG	"/data/hotplug.log"
#define FIRMWARE_PATH	"/system/lib/firmware"

static int load_firmware()
{
	char buf[4096];
	const char *firmware;
	int ret, cnt, firmware_fd, loading_fd, data_fd;

	ret = -1;
	if (!(firmware = getenv("FIRMWARE")))
		goto clean0;
	chdir(FIRMWARE_PATH);
	firmware_fd = open(firmware, O_RDONLY);
	if (firmware_fd < 0)
		goto clean0;

	chdir("/sys");
	chdir(getenv("DEVPATH") + 1);
	if ((loading_fd = open("loading", O_WRONLY)) < 0)
		goto clean1;
	if (write(loading_fd, "1", 1) != 1)
		goto clean2;
	if ((data_fd = open("data", O_WRONLY)) < 0)
		goto clean2;
	while ((cnt = read(firmware_fd, buf, sizeof(buf))) > 0) {
		int off, nw;
		for (off = 0; cnt > 0; cnt -= nw, off += nw)
			if ((nw = write(data_fd, buf + off, cnt)) < 0) {
				fprintf(stderr, "load_firmware: write failed\n");
				write(loading_fd, "-1", 2);
				goto clean3;
			}
	}
	if (write(loading_fd, "0", 1) == 1)
		ret = 0;

  clean3:
	close(data_fd);
  clean2:
	close(loading_fd);
  clean1:
	close(firmware_fd);
  clean0:
	return ret;
}

int main(int argc, char **argv)
{
	int ret = -1;
	const char *action;
	struct stat s;

	/* to debug hotplug, touch HOTPLUG_LOG */
	if (stat(HOTPLUG_LOG, &s) == 0) {
		FILE *log;
		if ((log = fopen(HOTPLUG_LOG, "a"))) {
			int i;
			for (i = 0; i < argc; ++i)
				fprintf(log, "%s ", argv[i]);
			fputc('\n', log);
			fclose(log);
		}
	}

	if (argc > 1 && (action = getenv("ACTION"))) {
		ret = 0;
		if (strcmp(action, "add") == 0) {
			if (strcmp(argv[1], "firmware") == 0)
				ret = load_firmware();
		}
	}
	return ret;
}
