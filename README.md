# simple-time-server
output nanosecond time quickly and simply
*******

WARNING: All versions so far have a fatal flaw.  I think it was triggered when IPv6 requests came in.  I think what happened 
was that the read / write failed but the infinite loop went into a 100% CPU spin.  The moral of the story is to check return values.
Perhaps the other moral is to check IPv6 locally.  The other other (sic) moral is to always be careful of infinite loops.

kwynn.com's CPU pegged at 100% starting around 1am my time.  It took a bit over an hour to drain the CPU credits and almost 
crash kwynn.com.  

My warnings on CPU usage worked just fine.  I got the email at 1:30 warning of 122 CPU credits where it should be close to 144.  I 
was so busy trying to get IPv6 working locally that I didn't read the email.  I assumed it was just an Ubuntu update needed.  

Specifically, this worked just fine: https://github.com/kwynncom/sysadmin-email-alerts-for-aws

I didn't notice the problem until 2:40 - 3am my time.

I will analyze the problem much later, most likely.  Even for me, it's nearing bedtime.

*********
The following test uses other-than the 'r' / raw option.  Any input other than r printf()s the time as a string.  The r option 
outputs it as a raw long integer--a little endian 8 byte unsigned integer on the current kwynn.com and other x86_64 machines.

With the server running:

# UDP
$ echo -n d | nc -u -W 1 localhost 8123
1624689747668562616

# TCP
$ echo -n d | nc    -W 1 localhost 8123
1624689753104457654

Note that "-static" compilation does seem to speed things up by roughly 8 - 15us (microseconds).  The binary is on the order 
of 800K rather than 17K, though.

echo -n d | nc    -6 -W 1 localhost 8123
echo -n d | nc -u -6  -W 1 localhost 8123

***************
build / release methodology:

date; make server ; openssl sha512 ./simptime ; openssl sha512 /usr/bin/simptime ; ls -ltr

/usr/bin/simptime should be a symbolic link to my local dev
