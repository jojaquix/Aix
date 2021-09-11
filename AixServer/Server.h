#pragma once

#include <AixCommon/AbsServer.h>
#include <iostream>

namespace aix {
	

	
	class Server: public AbsServer<int> {
		public:

		Server(int port): AbsServer(port) {		
		};

	};



}