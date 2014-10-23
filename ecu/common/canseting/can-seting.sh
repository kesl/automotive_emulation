#sudo apt-get update; sudo apt-get upgrade

cd /tmp; wget http://lnxpps.de/rpie/rpi-can-3.12.28+.tar.bz2

cd /; sudo tar jxvf /tmp/rpi-can-3.12.28+.tar.bz2 

sudo depmod -a

sudo modprobe spi-config devices=bus=0:cs=0:modalias=mcp2515:speed=10000000:gpioirq=25:pd=20:pds32-0=16000000:pdu32-4=0x2002:force_release

sudo modprobe mcp251x
cd ~/Desktop/canseting
sudo cp interfaces /etc/network
sudo cp modules /etc
sudo cp raspi-blacklist.conf /etc/modprobe.d 


sudo ip link set can0 up type can bitrate 250000

sudo ip -s -d link show can0






