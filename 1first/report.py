import dataInterval as di
import dataPacket as dp
from pprint import pprint
from prettytable import PrettyTable

print "Data for varying interval"
print "Max Packet = 1000\tPacket Size = 128 KB"
x = PrettyTable(["Parameter \ Interval", "0.02 sec", "0.05 sec", "0.1 sec","1 sec"])
x.align["Parameter \ Interval"] = "l" # Left align city names
x.padding_width = 1 # One space between column edges and contents (default)
for key in di.data100:
	x.add_row([key,di.data2[key],di.data5[key],di.data10[key],di.data100[key]])
print x

print "Data for varying packetsize"
print "Max Packet = 1000\tPInterval = 0.01"
x = PrettyTable(["Parameter \ packetsize", "64", "128", "256","512","1024"])
x.align["Parameter \ packetsize"] = "l" # Left align city names
x.padding_width = 1 # One space between column edges and contents (default)
for key in dp.data1024:
	x.add_row([key,dp.data64[key],dp.data128[key],dp.data256[key],dp.data512[key],dp.data1024[key]])
print x



# pprint(di.data100)

table = []
