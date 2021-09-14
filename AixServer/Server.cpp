// AixClient.cpp: define el punto de entrada de la aplicación.
//

#include "Server.h"
#include <AixCommon/Common.h>

using namespace std;
using namespace aix;

int main(int argn, char* argv[])
{
	//default value for listening port
	int port = 8765;
	int argIndex = 1;
	char* sport;
	if (argn > 1) {
		sport = argv[argIndex++];
		port = atoi(sport);
	}

	cout << "Starting server on port " << port << endl;

	Server<MsgTypes> server(port);
	server.start();
	server.resetSecsFromLastMessage();
	
	//wait up to 60 seconds for new messages
	while (server.secsFromLastMessage() < 60) {		
		server.update(-1, 2);
	}

	cout << "Stopping server due no messages!! \n";	
	server.stop();
	server.saveUsersList("users_list.txt");

	return 0;
}
