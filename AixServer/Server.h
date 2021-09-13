#pragma once

#include <AixCommon/AbsServer.h>
#include <iostream>

namespace aix {
	template<typename T>
	class Server: public AbsServer<T> {
		public:

		Server(int port): AbsServer(port) {		
		};

		bool onClientConnect(std::shared_ptr<connection<T>> conn) override {
			return true;
		}

		void onMessage(std::shared_ptr<connection<T>> client, message<T>& msg) override {
			std::cout << "[AixServer::onMessage]: msg from " << client->getId() << std::endl;
			switch (msg.header.mtype)
			{
			case MsgTypes::InsertUser :
				user_info aux;
				msg >> aux;
				std::cout << "[AixServer::onMessage]: trying to insert user "<< aux.name << std::endl;
				break;
			}
		}




	};



}