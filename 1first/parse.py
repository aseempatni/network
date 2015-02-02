import sys, getopt
import pprint as pp
import re

import subprocess
# subprocess.call(["./waf", "--run", "udp-echo","--maxPackets=1000 ","--interval=1","--packetSize=128"])

if __name__ == "__main__":
   	inputfile = sys.argv[1]
   	lines = []
	with open(inputfile,'r') as f:
		for line in f:
			line = line.split()
			lines.append(line)

	# packet = {}
	parameter = {}
	plus = []
	minus = []
	rec = []
	drop = []

	# Partitioning the lines into different categories
	for line in lines:
		if line[0] == 'd':
			drop.append(line)
		elif line[0] == '+':
			plus.append(line)
		elif line[0] == '-':
			minus.append(line)
		elif line[0] == 'r':
			rec.append(line)


	# when the first packet in the flow was transmitted
	parameter['timeFirstTPacket'] = plus[0][1]

	# when the last packet in the flow was transmitted;
	parameter['timeLastTPacket'] = plus[-1][1]

	#  when the first packet in the flow was received by an end node
	parameter['timeFirstRPacket'] = rec[0][1]

	# when the last packet in the flow was received
	parameter['timeLastRPacket'] = rec[-1][1]

	# the sum of all end-to-end delays for all received packets of the flow
	delaySum = 0
	# total number of packets that are assumed to be lost (not reported over 10 seconds)
	lostPackets = 0

	for i in range(len(rec)):
		for j in range(len(minus)):
			# if the node and id match calculate delay
			if (rec[i][18] == minus[j][18] and rec[i][-13] == minus[j][-13]):
				delaySum = delaySum +  float(rec[i][1]) - float(minus[j][1])
				break
		else:
			lostPackets = lostPackets+1
	parameter['delaySum'] = delaySum
	parameter['lostPackets'] = lostPackets

	# total number of transmitted bytes / packets for the flow
	tBytes = 0
	for i in range(len(minus)):
		byte = int (re.findall(r"\d+", line[-3])[0])
		tBytes = tBytes + byte
	parameter['tBytes'] = tBytes
	parameter['tPackets'] = len(minus)

	# total number of received bytes / packets for the flow
	rBytes = 0
	for i in range(len(rec)):
		byte = int (re.findall(r"\d+", line[-3])[0])
		rBytes = rBytes + byte
	parameter['rBytes'] = rBytes
	parameter['rPackets'] = len(rec)

	# the number of times a packet has been reportedly forwarded
	# ie, number of packets having more than one '-' after '+'
	timesForwarded = 0
	for i in range(len(plus)):
		count=0
		for j in range(len(minus)):
			# count the number of minus which matches with plus's node and id 
			if (plus[i][18] == minus[j][18] and plus[i][-13] == minus[j][-13]):
				count+=1
		if(count>1):
			timesForwarded +=1
	parameter['timesForwarded'] = timesForwarded

	# the number of lost packets and bytes
	bytesDropped = 0
	for i in range(len(drop)):
		byte = int (re.findall(r"\d+", line[-3])[0])
		bytesDropped = bytesDropped + byte
	parameter['bytesDropped'] = bytesDropped
	parameter['packetsDropped'] = len(drop)

	# the throughput of the transmitter, calculated as amount of bytes transmitted divided by total time
	parameter ['transmitterThroughput'] = tBytes / float(parameter['timeLastTPacket']) - float(parameter['timeFirstTPacket'])
	# the throughput of the receiver, calculated as amount of bytes received divided by total time
	parameter ['recieverThroughput'] = rBytes / float(parameter['timeLastRPacket']) - float(parameter['timeFirstRPacket'])

	pp.pprint(parameter)

'''
		if int(line[18]) in packet.keys():
			packet[int(line[18])].append(line)
		else:
			packet[int(line[18])] = [line]
'''

'''
	delaySum = 0
	for key, value in packet.iteritems():
		for pa in value:
			if pa[0] == '+':
				delay -= float(pa[1])
			elif pa[0] == 'r':
				delay += float(pa[1])
'''

'''
#extracting the total delay in receiving the packets
delaySum = 0.0
for i in range(len(rec)):
for j in range(len(minus)):
if (rec[i][18] == minus[j][18] and rec[i][len(rec[i]) - 13] == minus[j][len(minus[j]) - 13]):
delaySum = delaySum +  float(rec[i][1]) - float(minus[j][1])
break

print("delaySum = " + str(delaySum))
'''
