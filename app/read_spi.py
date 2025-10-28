# read_spi.py
# I used this to test photoresistor functonality in SPI
# from chat
# is on /home/ on my beagle and needs to be run with sudo 
# sudo python3 read_spi.py

import spidev, time

spi = spidev.SpiDev()
spi.open(0, 0)          # /dev/spidev0.0 → bus 0, CS0communication
spi.max_speed_hz = 1000000
spi.mode = 0

def read_adc(ch=0):
    # Build command: start bit, single-ended, channel
    cmd = [0x06 | ((ch & 0x04) >> 2), (ch & 0x03) << 6, 0x00]
    resp = spi.xfer2(cmd)
    val = ((resp[1] & 0x0F) << 8) | resp[2]
    return val

while True:
    value = read_adc(0)
    volts = 3.3 * value / 4096
    print(f"CH0: {value:4d}  ({volts:0.3f} V)", end="\r")
    time.sleep(0.1)

