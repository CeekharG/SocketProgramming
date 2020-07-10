#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>

#define DBFILE	"weather20.txt"

using namespace std;

void updateWeatherData(ifstream& file, string **& vec, int c)
{
	string sFiled = "", sLine = "";
		
	for (int tmp = 0; tmp < c; tmp++) 
	{
		getline(file, sLine);	
		stringstream str(sLine);
		
		for (int i = 0; i < 3; i++) 	
		{
			getline(str, sFiled, ',');
			vec[tmp][i] = sFiled;
		}
	}
}

int printWeatherData(string **&vec, int count, char *buf)
{
	for (int i = 0; i < count; i++) {
		if (!strncmp(vec[i][0].c_str(), buf, strlen(vec[i][0].c_str())) ) {
			cout << "Tomorrow's maximum temperature is " << vec[i][1] << endl; 
			cout << "Tomorrow's sky condition is " << vec[i][2] << endl;
			memset(buf, 0, 4096);
			sprintf(buf, "%s,%s", vec[i][1].c_str(), vec[i][2].c_str());	
			return 0;
		} 
	}	
	return -1;
}

int main()
{
	int count = 0, csize = 3, nRecv, result, clientSocket;
	string line;
	char buffer[4096] = {0};
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	int port;

	ifstream inFile;
	inFile.open(DBFILE);

	while (getline(inFile, line)) {
		count++;
	}

	inFile.close();

	string** array = new string*[count];
	for (int i = 0; i < count; i++) {
		array[i] = new string[csize];
	}

	ifstream inFile2;
	inFile2.open(DBFILE);

	updateWeatherData(inFile2, array, count);

	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1) {
		cerr << "Can't create a socket!" << endl;
		return -1;
	}
	
	cout << "Enter server port number: ";
	cin >> port;
	string ipAddress = "127.0.0.1";
	cout << endl <<"Server Initiated........."<<endl;
	cout << "Waiting for Client Connection at port number: " << port <<endl;

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);

	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
		cerr << "Can't bind to IP/port" << endl;
		return -2;
	}

	if (listen(listening, SOMAXCONN) == -1) {
		cerr << "Can't listen" << endl;
		return -3;
	}

	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == -1) {
		cerr << "Problem with client connection" << endl;
		return -4;
	}

	close(listening);

	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);

	result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

	cout << endl;
	cout <<"Server Connected Sucessfully with Client at port number: "<< port << endl;
	cout <<"Waiting for Client Request......"<<endl<<endl;

	inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
	while ((nRecv = recv(clientSocket, buffer, 4096, 0)) > 0) {
		if (nRecv > 0) {
			cout << "Weather report for " << string(buffer, 0, nRecv) << endl;

			if (printWeatherData(array, count, buffer) < 0) {
				memset(buffer, 0, 4096);
				memcpy(buffer, "No data", strlen("No data"));
				cout << "No data" << endl;
			} 
			send(clientSocket, buffer, 4096, 0);
			memset(buffer, 0, 4096);
		}
		cout << endl;
	}
	close(clientSocket);

	return 0;	
}
