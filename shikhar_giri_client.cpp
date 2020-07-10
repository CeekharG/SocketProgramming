#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

using namespace std;

#define IPADDRESS	"127.0.0.1"
#define BUF_SIZE	4096

void print_server_reply(char *buf)
{
	string sFilled;
	stringstream ss;
	ss.str(buf);
	if (!strncmp(buf, "No data", strlen("No data")))
		cout << "No data" << endl;
	else {
		getline(ss, sFilled, ',');	
		cout << "Tomorrow's maximum temperature is " << sFilled << endl; 
		getline(ss, sFilled, ',');	
		cout << "Tomorrow's sky condition is " << sFilled << endl;
	}			
}

int main()
{
	int sockfd, portno, ret = 0, sres = 0, nrecv, len = BUF_SIZE;
	sockaddr_in	addr, *result;
	char buf[BUF_SIZE] = {0};
	string host, port, str;

	cout << "Enter the server host name: " << endl;
	cin >> host;
	cout << "Enter Server Port Number" << endl; 
	cin >> port;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "Unable to create socket" << endl;
		return -1;
	}
	bzero(&addr, sizeof(addr));

	portno = atoi(port.c_str());

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(portno);

	getaddrinfo(host.c_str(), port.c_str(), (addrinfo *)&addr, (addrinfo **)&result); 

	if ((ret = connect(sockfd, (sockaddr *)&addr, sizeof(addr))) < 0) {
		cout << "Unable to Connect to server with port number: "<< port << endl;
		close(sockfd);
		return -1;
	}

	cout << endl << "Client Connected sucessfully with Server at port number: " << port <<endl;
	cout << endl;

	getline(cin, str);
	do {
		cout << "Enter a city name: ";
		getline(cin, str);
		if ((sres = write(sockfd, str.c_str(), len)) < 0) {
			cout << "unable to send data!" << endl;
			continue;
		}
		memset(buf, 0, BUF_SIZE);	
                nrecv = read(sockfd, buf, BUF_SIZE);
		print_server_reply(buf);
		cout << endl;
	} while (true);	

	close(sockfd);	
	return 0;
}
