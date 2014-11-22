/*
 * Copyright (C) 2014 Brian Fitzpatrick <brian@froxen.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "lcthw/dbg.h" //http://c.learncodethehardway.org/book/ex20.html

int main(int argc, char *argv[])
{
	int fd = 0;
	char *fifo;
	struct stat status;
	char *buf;
	int bytes = 8192; //number of bytes to read/write at a time
	int rc;
	int usec = 1E6 * bytes / (44100 * 4); //44.1kHz, 16-bit stereo

	// Ignore pipe signals
    signal(SIGPIPE, SIG_IGN);

    // Check arguments
	check(argc = 2, "Usage: %s FIFO\n", argv[0]);
	fifo = argv[1];

    // Open and check the FIFO
    fd = open(fifo, O_WRONLY);
    check(fd > 0, "Failed to open FIFO: %s", fifo);
	check(fstat(fd, &status) == 0, "fstat error");
	check(S_ISFIFO(status.st_mode), "%s is not a FIFO!\n", fifo);
	
	// Continuously read from stdin and write to fifo
	buf = malloc(bytes);
	while (1) {
		rc = read(STDIN_FILENO, buf, bytes); 
		if (rc <= 0) { //No data, exit
			close(fd);
			exit(EXIT_SUCCESS);
		}
		rc = write(fd, buf, rc);
		// If nothing is reading from the FIFO, the write won't
		// block, so we must sleep for a while.
		if (rc <= 0) usleep(usec);
	}
	return 0;
	
error:
	if (fd > 0) close(fd);
	exit(EXIT_FAILURE);
}
