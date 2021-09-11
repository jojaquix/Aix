// AixClient.cpp: define el punto de entrada de la aplicación.
//

#include "Server.h"
#include <AixCommon/Common.h>

using namespace std;

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
	return 0;
}
