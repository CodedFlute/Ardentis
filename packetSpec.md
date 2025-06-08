# Packet Format

The packet format for Ardentis is an extremely simple packet spec. It starts with a 6 byte call-sign, then a > character to signify the start of the packet data, to ensure backwards compatibility the is then a 1 byte character to identify the packet version or type. All the data after this packet header is encoded in byte arrays as defined below.

3 bytes: 24 bit signed int representing the decimal degree longitude multiplied by 10 000.

3 bytes: 24 bit signed integer representing the decimal degree latitude multiplied by 10 000

2 bytes: 16 bit number representing the altitude above sea level in 2 meter units (i.e. altitude in meter is 0.5 * this value).

An example packet is shown below (note spaces are only shown for easy of reading)

VE3FSZ>a 3 Byte Lat 3 Byte Long, 2 Byte Alt