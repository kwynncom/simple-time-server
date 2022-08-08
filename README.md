# simple-time-server
output nanosecond time quickly and simply

https://kwynn.com/t/22/08/tsck/ - live demo

USAGE

Make a UDP or TCP connection and enter / send any character.  Any character other than 'r' gives the decimal / string representation of the time as 
the number of nanoseconds since the UNIX Epoch's start.
 
An entry of 'r' outputs the raw 64 bit unsigned integer.  (It's little endian on the current kwynn.com and other x86_64 machines.  I have not tested 
how that comes through the network to other architectures.  My best understanding is it should work fine.)

***
The following contains important newlines that do not show well on the web.  Try the raw view:

https://raw.githubusercontent.com/kwynncom/simple-time-server/main/README.md

I'm adding extra newlines to try to make this look right by default in GitHub:

From anywhere:

telnet kwynn.com 8123

Trying 2600:1f18:23ab:9500:acc1:69c5:2674:8c03...

Connected to kwynn.com.

Escape character is '^]'.

d

1630206989322172987

1630206989322234138

1630206989322236828

3 timestamps show because of the newline and line feed.

echo -n d | nc -u -W 1 kwynn.com 8123

1630207642688411581

-u below is the UDP version -4 is IPv4; -6 is IPv6.

echo -n d | nc  -W 1 -u -6 kwynn.com 8123

echo -n d | nc  -W 1    -6 kwynn.com 8123 

echo -n d | nc  -W 1 -u -4 kwynn.com 8123

echo -n d | nc  -W 1    -4 kwynn.com 8123

Locally:

1. make and start server:

make server
sudo -u nobody -g nogroup ./simptime

2. test / use server / get time:

telnet localhost 8123
(as above)

echo -n d | nc -u -W 1 localhost 8123
1624689747668562616

Testing the client programmatically:

Assuming local server running (as above):

make speedTest
./sp 127.0.0.1
1630208738066724390
1630208738066865336
1630208738066955367
[...]

Defaults to kwynn.com's IPv4 address as of 2021/08, which you can change in config.h

./sp 
1630210323874287443
1630210323973145003
1630210324023415673


BUILD NOTES:

The "-static" compilation does seem to speed up runtime by roughly 8 - 15us (microseconds).  The binary is on the order 
of 800K rather than 17K, though.
