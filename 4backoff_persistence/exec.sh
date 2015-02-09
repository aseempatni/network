#!/bin/bash

rm data

out="16"
rm $out
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=2"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

out="32"
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=4"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

out="64"
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=8"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

out="128"
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=16"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

out="256"
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=32"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

out="512"
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=64"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

out="1024"
for i in {1..10}
do
	./waf --run "second --interval=0.125 --packetSize=128"
	cat trace.tr | grep UdpHeader > trace-new.tr
	python parse.py trace-new.tr >> $out
done
python computestat.py $out >> data
rm $out

# python plot.py