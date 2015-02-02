clear

./waf --run "udp-echo --maxPackets=1000 --interval=0.01 --packetSize=64"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 64
./waf --run "udp-echo --maxPackets=1000 --interval=0.01 --packetSize=128"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 128
./waf --run "udp-echo --maxPackets=1000 --interval=0.01 --packetSize=256"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 256
./waf --run "udp-echo --maxPackets=1000 --interval=0.01 --packetSize=512"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 512
./waf --run "udp-echo --maxPackets=1000 --interval=0.01 --packetSize=1024"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 1024

./waf --run "udp-echo --maxPackets=1000 --interval=0.02 --packetSize=128"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 0.02
./waf --run "udp-echo --maxPackets=1000 --interval=0.05 --packetSize=128"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 0.05
./waf --run "udp-echo --maxPackets=1000 --interval=0.1 --packetSize=128"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 0.1
./waf --run "udp-echo --maxPackets=1000 --interval=1 --packetSize=128"
cat udp-echo.tr | grep UdpHeader > udp-echo-new.tr
python  parse.py udp-echo-new.tr > 1

