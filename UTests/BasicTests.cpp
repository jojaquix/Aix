#include <gtest/gtest.h>
#include <AixServer/Server.h>


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

TEST(BasicTests, BasicServerTest) {
	EXPECT_EQ(7 * 6, 42);
	Server server(8765);
	server.start();
	getchar();
	server.stop();

}