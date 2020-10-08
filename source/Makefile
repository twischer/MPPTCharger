all:
	# Reboot ESP into bootloader
	#-echo R | netcat -w 1 esp-9d580d.fritz.box 23
	# Flash ESP
	pio run -t upload
	telnet esp-9d580d.fritz.box

clean:
	pio run -t clean

