Openpipe creates a non-blocking pipe from stdin.

There may be a simpler way to accomplish this, but I wanted to get audio from
pianobar (https://github.com/PromyLOPh/pianobar) into forked-daapd
(https://github.com/ejurgensen/forked-daapd), and this works.

To use, set up an ALSA alias in ~/.asoundrc or /etc/asound.conf:

	pcm.writePipe {
        type file
		slave {
			pcm null
		}
		file "|openpipe /srv/music/Pandora"
		format "raw"
	}

Create a named pipe with mkfifo and make sure the write permissions are
appropriate.

Then, send the ALSA audio to "writePipe". For pianobar, add a line to
/etc/libao.conf:
	
	dev=writePipe

To build:
	
	$ git clone "https://github.com/b-fitzpatrick/openpipe"
	$ cd openpipe
	$ make openpipe
	$ sudo cp ./openpipe /usr/local/bin/ (optional)
