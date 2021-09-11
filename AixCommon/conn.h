#pragma once


#include <AixCommon/Common.h>
#include <AixCommon/message.h>
#include <asio.hpp>

/**
 wrapper internal asio socket etc.
*/

namespace aix{

	/*	
		this is a low level data type
		that is reason is in lowercase
		is a handler for connections
	*/
	template<typename T>
	class connection {		
	public:		
		connection(asio::io_context& asio_context, asio::ip::tcp::socket socket)
			: asio_context(asio_context), socket(std::move(socket))
		{
		}

		void connectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
			asio::async_connect(socket, endpoints, 
				[this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
					if (!ec) {
						//ReadHeader();
					}
					else {
						std::cout << "Client connected" << std::endl;
					}
				});
		}


		bool isConnected() {
			return socket.is_open();
		}

		void disconnect() {
			if (isConnected()) {
				asio_context.post(
					[this]() {
						socket.close();
					}
				);
			}
		}


	private:
		asio::io_context& asio_context;
		asio::ip::tcp::socket socket;
		message<T> tempInMsg;
	};
}