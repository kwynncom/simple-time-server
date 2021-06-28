# simple-time-server
output nanosecond time quickly and simply

USAGE

From anywhere:

echo -n d | nc  -W 1 -u -6 kwynn.com 8123
echo -n d | nc  -W 1    -6 kwynn.com 8123 
echo -n d | nc  -W 1 -u -4 kwynn.com 8123
echo -n d | nc  -W 1    -4 kwynn.com 8123

Locally:

1. make and start server:

make server
sudo -u nobody -g nogroup ./simptime


2. test / use server / get time:

echo -n d | nc  -W 1 -u -6 localhost 8123
echo -n d | nc  -W 1    -6 localhost 8123 
echo -n d | nc  -W 1 -u -4 localhost 8123
echo -n d | nc  -W 1    -4 localhost 8123

-u is for UDP ; -4 and -6 are for IPv4 and v6

For example:

$ echo -n d | nc -u -W 1 localhost 8123
1624689747668562616

Which was at that time the number of nanoseconds since the UNIX Epoch's start.

The above test uses other-than the 'r' / raw option; it uses 'd'.  Any input other than 'r' printf()s the time as a string.  
The r option outputs it as a raw long integer--a little endian 8 byte unsigned integer on the current kwynn.com and other 
x86_64 machines.

BUILD NOTES:

The "-static" compilation does seem to speed things up by roughly 8 - 15us (microseconds).  The binary is on the order 
of 800K rather than 17K, though.
