// AixClient.cpp: define el punto de entrada de la aplicación.
//

#include "Client.h"
#include <AixCommon/Common.h>

using namespace std;
using namespace aix;

int main(int argn, char* argv[])
{	
	int port = 8765;
	string name = "user";
	int argIndex = 1;
	char* sport;
	if (argn > 1) {
		sport = argv[argIndex++];
		port = atoi(sport);
	}
	if (argn > 2) {
		name = argv[argIndex++];		
	}

	Client<MsgTypes> client(name);
	client.connect(port, "localhost");

	while (client.hasMoreIds()) {
		client.sendUserAndCheckResponse();
	}
	cout << "Stopping client due no more ids\n";



	client.disconnect();
	return 0;
}
