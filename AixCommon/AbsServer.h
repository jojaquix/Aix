#pragma once

#include <AixCommon/Common.h>
#include <AixCommon/conn.h>
#include <random>
#include <sstream>


namespace aix {
	static std::random_device              rd;
	static std::mt19937                    gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);

	// base clase for asio based server
	// for messages<T>
	template<typename T>
	class AbsServer {

	public:
		AbsServer(int port) :
			asioAcceptor(asioContext,
				asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
		}

		virtual ~AbsServer() {
			stop();
		}



		bool start() {
			try
			{
				asyncWaitForConnection();
				// todo i want to change this to use a thread pool 
				theThread = std::thread([this]() { asioContext.run(); });
			}
			catch (std::exception& e)
			{
				// Something prohibited the server from listening
				std::cerr << "[AixServer::Error] Exception: " << e.what() << "\n";
				return false;
			}
			std::cout << "[AixServer::Trace] Started!\n";
			return true;
		}

		std::string getNextId() {
			std::stringstream ss;
			int i;
			ss << std::hex;
			for (i = 0; i < 8; i++) {
				ss << dis(gen);
			}
			ss << "-";
			for (i = 0; i < 4; i++) {
				ss << dis(gen);
			}
			ss << "-4";
			for (i = 0; i < 3; i++) {
				ss << dis(gen);
			}
			ss << "-";
			ss << dis2(gen);
			for (i = 0; i < 3; i++) {
				ss << dis(gen);
			}
			ss << "-";
			for (i = 0; i < 12; i++) {
				ss << dis(gen);
			};
			return ss.str();
		}

		// [async]
		void asyncWaitForConnection() {
			asioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket) {
					if (!ec)
					{					
						std::cout << "[AixServer::Trace] New Connection Ok: " << socket.remote_endpoint() << "\n";
						// Socket Wrapper conn to handle the client
						auto newconn = std::make_shared<connection<T>>(false, 
							asioContext, 
							std::move(socket),
							qMessagesIn);

						if (onClientConnect(newconn)) {
							auto id = getNextId();
							newconn->setId(id);
							deqConnections.push_back(std::move(newconn));
							deqConnections.back()->connectToClient(id);

							std::cout << "[AixServer::Trace] Connection Approved for " << id << std::endl;
						}
						else {
							std::cout << "[AixServer::Trace] Connection Denied\n";
						}
					}
					else {
						// Error has occurred during acceptance
						std::cout << "[AixServer::Error] New Connection Error: " << ec.message() << "\n";
					}

					// Prime the asio context with more work - again simply wait for
					// another connection...
					asyncWaitForConnection();
				});
		}

		void stop() {
			if (!asioContext.stopped()) {
				asioContext.stop();
				if (theThread.joinable()) theThread.join();
				std::cout << "[AixServer::Trace] Stopped!\n";
			}
		}

		void messageToClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
		{
			// Check client is legitimate...
			if (client && client->isConnected())
			{
				// ...and post the message via the connection
				client->send(msg);
			}
			else
			{
				onClientDisconnect(client);
				client.reset();
				deqConnections.erase(
					std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
			}
		}

		// Force server to respond to incoming messages
		void update(size_t nMaxMessages = -1, bool bWait = false)
		{
			if (bWait) qMessagesIn.wait();

			size_t nMessageCount = 0;
			while (nMessageCount < nMaxMessages && !qMessagesIn.empty())
			{
				// Grab the front message
				auto msg = qMessagesIn.pop_front();
				// Pass to message handler
				onMessage(msg.remote, msg.msg);
				nMessageCount++;
			}
		}



		//Api for 
		//run on server thread
		
		virtual bool onClientConnect(std::shared_ptr<connection<T>>) {
			return false;
		}
		//run on server thread
		virtual void onMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
		{

		}

	protected:

		TsQueue<owned_message<T>> qMessagesIn;
		std::deque<std::shared_ptr<connection<T>>> deqConnections;

		asio::io_context asioContext;
		std::thread theThread;
		asio::ip::tcp::acceptor asioAcceptor;
	};
}