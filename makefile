all:
	#Make lib for broadsword
	mkdir -p /usr/lib/broadsword
	
	#Copy tcp_flood.h and udp_flood.h to usr/lib/broadsword
	cp lib/tcp_flood.h /usr/lib/broadsword
	cp lib/udp_flood.h /usr/lib/broadsword
	
	#Compile broadsword.c
	gcc src/broadsword.c -o broadsword -lnet
	
	#Copy broadsword to /usr/bin
	cp src/broadsword /usr/bin
