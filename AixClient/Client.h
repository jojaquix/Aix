#pragma once
#include <AixCommon/Common.h>
#include <AixCommon/AbsClient.h>
#include <iostream>
#include <random>
#include <string>
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
			
			std::stringstream ss;
			ss << name;
			ss << std::to_string(usedIndex);

			message<MsgTypes> msg;
			msg.header.mtype = MsgTypes::InsertUser;			
			user_info usr = { ids[usedIndex], "\0"};
			std::strncpy(usr.name, ss.str().c_str(), USER_NAME_MAX_SIZE);
			
			usedIndex++;
			msg << usr;
			conn->send(msg);
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