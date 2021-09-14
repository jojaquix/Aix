#pragma once
#include <AixCommon/Common.h>
#include <AixCommon/AbsClient.h>
#include <iostream>
#include <random>
#include <string>
#include <chrono>

namespace aix {
	
	template<typename T>
	class Client : public AbsClient<T> {
	public:
		const int IDS_CAP = 235;

		Client(std::string name): name(name) {
			initIds();			
		}

		void initIds() {
			ids.resize(IDS_CAP, 0);
			uint8_t value = 1;
			for (auto& val : ids) {
				val = value++;				
			}		
			std::shuffle(ids.begin(), ids.end(), default_random_engine(time(0)));
		}
			
		void sendUserAndCheckResponse() {			
			if (isConnected()) {
				std::stringstream ss;
				ss << name;
				ss << std::to_string(usedIndex);

				message<MsgTypes> msg;
				msg.header.mtype = MsgTypes::InsertUser;
				user_info usr = { ids[usedIndex], "\0" };
				std::strncpy(usr.name, ss.str().c_str(), USER_NAME_MAX_SIZE);

				std::cout << "[AixClient] Sending this user to insert " << usr << std::endl;
				usedIndex++;
				msg << usr;

				auto tick = std::chrono::high_resolution_clock::now();

				send(msg);

				//wait for server response
				incomingQueue().wait();
				auto responseMsg = incomingQueue().pop_front().msg;

				switch (responseMsg.header.mtype)
				{
				case MsgTypes::UserInsertOk:
				{
					// Server has responded to a ping request				
					std::cout << "Server said user inserted Ok\n";
				}
				break;
				case MsgTypes::UserDuplicate:
				{
					// Server has responded to a ping request				
					std::cout << "Server said user NOT inserted \n";
				}
				break;
				default:
					std::cout << "Server said i dont know what you want \n";
				}

				auto tock = std::chrono::high_resolution_clock::now();
				std::cout << "time elapsed for request: " << (tock - tick) / std::chrono::milliseconds(1) << " ms" << std::endl;
			}
		}


		bool hasMoreIds() {
			return usedIndex < IDS_CAP;
		}

	private:
		std::vector<uint8_t> ids;		
		uint8_t usedIndex = 0;	
		std::string name;
	};

}