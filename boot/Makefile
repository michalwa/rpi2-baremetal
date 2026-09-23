# Inspired by https://github.com/rsta2/circle/blob/c776b3c614c2cc66ee4007a761d8786b9402db44/boot/Makefile
# Cut down to the bare essentials for the RaspberryPi 2 model B v1.1

FIRMWARE_REV = 6f0881cba8bea8ec24956a5718714730e9936ec5
FIRMWARE_BASEURL = https://github.com/raspberrypi/firmware/raw/$(FIRMWARE_REV)/boot
FIRMWARE_FILES = \
	bootcode.bin \
	start.elf \
	fixup.dat

all: firmware

firmware: clean
	@for file in $(FIRMWARE_FILES); do \
		echo downloading $$file; \
		wget -q -O $$file $(FIRMWARE_BASEURL)/$$file; \
		echo $$file >> .gitignore; \
	done

clean:
	cp .gitignore.base .gitignore
	rm -f $(FIRMWARE_FILES)
