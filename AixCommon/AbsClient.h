#pragma once

#include <AixCommon/Common.h>
#include <AixCommon/conn.h>

namespace aix {
	// base clase for asio based server
	// for messages<T>
	template<typename T>
	class AbsClient {

	public:
		AbsClient() {
		}

		virtual ~AbsClient() {
			disconnect();
		}

		bool connect(int port, std::string address) {
			try
			{
				asio::ip::tcp::resolver resolver(asioContext);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
				
				conn = std::make_unique<connection<T>>(true, 
					asioContext, 
					asio::ip::tcp::socket(asioContext),
					qMessagesIn);

				conn->connectToServer(endpoints);
				theThread = std::thread([this]() { asioContext.run(); });
			}
			catch (std::exception& e)
			{				
				std::cerr << "[AixClient::Error] Exception: " << e.what() << "\n";
				return false;
			}
			std::cout << "[AixClient::Trace] Connected!\n";
			return true;
		}

		void disconnect() {			
			//disconect connection
			if (conn) {
				conn->disconnect();
				if (!asioContext.stopped()) {
					asioContext.stop();
					if (theThread.joinable()) theThread.join();
					std::cout << "[AixClient::Trace] Disconnected!\n";
				}
				//connection release
				conn.release();
			}
		}

		bool isConnected()
		{
			if (conn)
				return conn->isConnected();
			else
				return false;
		}

		void send(const message<T>& msg)
		{
			if (isConnected())
				conn->send(msg);
		}
		 
		TsQueue<owned_message<T>>& incomingQueue() {
			return qMessagesIn;
		}
		
	
	protected:
	
		asio::io_context asioContext;
		std::thread theThread;
		std::unique_ptr<connection<T>> conn;

	private:
		TsQueue<owned_message<T>> qMessagesIn;

	};
}