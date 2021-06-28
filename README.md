# simple-time-server
output nanosecond time quickly and simply

With the server running:

# UDP
$ echo -n d | nc -u -W 1 localhost 8123
1624689747668562616

# TCP
$ echo -n d | nc    -W 1 localhost 8123
1624689753104457654

The above test uses other-than the 'r' / raw option.  Any input other than r printf()s the time as a string.  The r option 
outputs it as a raw long integer--a little endian 8 byte unsigned integer on the current kwynn.com and other x86_64 machines.


Note that "-static" compilation does seem to speed things up by roughly 8 - 15us (microseconds).  The binary is on the order 
of 800K rather than 17K, though.

echo -n d | nc  -W 1 -u -6 localhost 8123
echo -n d | nc  -W 1    -6 localhost 8123 
echo -n d | nc  -W 1 -u -4 localhost 8123
echo -n d | nc  -W 1    -4 localhost 8123
