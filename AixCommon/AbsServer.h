#pragma once

#include <AixCommon/Common.h>
#include <AixCommon/conn.h>

namespace aix {
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

		//run on server thread
		virtual bool onClientConnect(std::shared_ptr<connection<T>>) {
			return false;
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

		// [async]
		void asyncWaitForConnection() {
			asioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket) {
					if (!ec)
					{					
						std::cout << "[AixServer::Trace] New Connection Ok: " << socket.remote_endpoint() << "\n";
						// Socket Wrapper conn to handle the client
						auto newconn = std::make_shared<connection<T>>(asioContext, std::move(socket));
						if (onClientConnect(newconn)) {
							//todo store new connection in some place
							//container to allow us 
							std::cout << "[AixServer::Trace] Connection Approved\n";
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
	

		//Server receive a message and send message as response
		//virtual message<T> onMessageIn(message<T>& msg) { return msg }
	protected:
		asio::io_context asioContext;
		std::thread theThread;
		asio::ip::tcp::acceptor asioAcceptor;
	};
}