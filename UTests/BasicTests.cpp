#include <gtest/gtest.h>
#include <AixServer/Server.h>
#include <AixClient/Client.h>
#include <AixCommon/message.h>
#include <AixCommon/TsMap.h>



#include <thread>
//for testing
using namespace aix;
using namespace std;




// Demonstrate some basic assertions.
TEST(BasicTests, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}



TEST(BasicTests, TsMapTest) {

	TsMap<uint8_t, std::string> usersMap;
	usersMap.insert(120, "James");
	usersMap.insert(121, "Carlos");

	EXPECT_EQ(true, usersMap.existsKey(120));

	auto copyMap = usersMap.getCopyMap();
	EXPECT_EQ(120, (*copyMap.begin()).first);

}


TEST(BasicTests, BasicServerTest) {
		
	
	message<MsgTypes> msg;
	msg.header.mtype = MsgTypes::InsertUser;
	user_info usr = { 230, "user 1" };
	msg << usr;
	
	Server<MsgTypes> server(8765);
	server.start();
	server.resetSecsFromLastMessage();
	
	server.update(-1, -1);
	
	std::thread srvThread([&server]{
		while (server.secsFromLastMessage() < 60) {
		//while (true) {
			server.update(-1, 5);			
		}
	});


	//srvThread.join();
	//std::cout << "server finished!! \n";

	Client<MsgTypes> client("user");
	client.connect(8765, "localhost");	
	std::cout << "press key to start sending messages!! \n";
	getchar();
	
	while (client.hasMoreIds()) {
		client.sendUserAndCheckResponse();
	}
		
	getchar();
	server.stop();
}