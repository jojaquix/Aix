#pragma once

#include <AixCommon/AbsServer.h>
#include <AixCommon/TsMap.h>
#include <iostream>
#include <fstream>
#include <atomic>

namespace aix {
	template<typename T>
	class Server: public AbsServer<T> {
		public:

		Server(int port): AbsServer(port) {		
		};

		~Server() {
			usersCollection.clear();
		}

		bool onClientConnect(std::shared_ptr<connection<T>> conn) override {
			return true;
		}

		void onMessage(std::shared_ptr<connection<T>> client, message<T>& msg) override {
			std::cout << "[AixServer::onMessage]: msg from " << client->getId() << std::endl;
			resetSecsFromLastMessage();			
			
			switch (msg.header.mtype)
			{
			case MsgTypes::InsertUser :
				user_info aux;
				msg >> aux;
				std::cout << "[AixServer::onMessage]: trying to insert "<< aux  << std::endl;
				if (!usersCollection.existsKey(aux.id)) {
					usersCollection.insert(aux.id, std::make_pair(aux.name, client->getId()));
					message<MsgTypes> respMsg;
					respMsg.header.mtype = MsgTypes::UserInsertOk;
					std::cout << "[AixServer::onMessage]: insertion ok " << std::endl;										
					sendMessageToClient(client, respMsg);
				}
				else {
					message<MsgTypes> respMsg;
					respMsg.header.mtype = MsgTypes::UserDuplicate;
					std::cout << "[AixServer::onMessage]: insertion fail " << std::endl;
					sendMessageToClient(client, respMsg);
				}

				break;
			}
		}


		time_t secsFromLastMessage() {
			auto aux = lastMessageTime.load();
			return (time(0) - aux);
		}

		void resetSecsFromLastMessage() {
			lastMessageTime.store(time(0));
		}

		void saveUsersList(std::string fileName) {
			std::ofstream ofs(fileName, std::ios::binary);
			//this is done only whe server is shutting down
			//does not affect in runtime
			auto usersData = usersCollection.getCopyMap();
			for (auto& el : usersData) {
				ofs << el.first << " "<< el.second.first <<" "<<el.second.second << std::endl;
			}
		}

	private:
		//map id as key and values are {user name, client name}
		TsMap<uint8_t, std::pair<std::string, std::string>> usersCollection;
		std::atomic<time_t> lastMessageTime = 0;
	};



}