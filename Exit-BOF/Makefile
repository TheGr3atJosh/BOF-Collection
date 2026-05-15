CC64 = x86_64-w64-mingw32-gcc
CC86 = i686-w64-mingw32-gcc
STRIP64 = x86_64-w64-mingw32-strip --strip-unneeded
STRIP86 = i686-w64-mingw32-strip --strip-unneeded
CFLAGS = -I ../_include -w -Wno-incompatible-pointer-types -Os -DBOF -c

all: bof

bof: clean
	@(mkdir _bin 2>/dev/null) && echo 'creating _bin directory' || echo '_bin directory exists'
	@($(CC64) $(CFLAGS) exitprocess/exitprocess.c -o _bin/exitprocess.x64.o && $(STRIP64) _bin/exitprocess.x64.o) && echo '[+] exitprocess x64' || echo '[!] exitprocess x64'
	@($(CC86) $(CFLAGS) exitprocess/exitprocess.c -o _bin/exitprocess.x32.o && $(STRIP86) _bin/exitprocess.x32.o) && echo '[+] exitprocess x32' || echo '[!] exitprocess x32'
	@($(CC64) $(CFLAGS) exitthread/exitthread.c -o _bin/exitthread.x64.o && $(STRIP64) _bin/exitthread.x64.o) && echo '[+] exitthread x64' || echo '[!] exitthread x64'
	@($(CC86) $(CFLAGS) exitthread/exitthread.c -o _bin/exitthread.x32.o && $(STRIP86) _bin/exitthread.x32.o) && echo '[+] exitthread x32' || echo '[!] exitthread x32'

clean:
	@(rm -rf _bin)
