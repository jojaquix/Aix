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

		//run on io thread
		virtual bool onConnect(std::shared_ptr<connection<T>>) {
			return false;
		}

		bool connect(int port, std::string address) {
			try
			{
				asio::ip::tcp::resolver resolver(asioContext);
				//asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(address), port);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
				
				conn = std::make_unique<connection<T>>(asioContext, asio::ip::tcp::socket(asioContext));
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
	
	protected:
		asio::io_context asioContext;
		std::thread theThread;
		std::unique_ptr<connection<T>> conn;
	};
}