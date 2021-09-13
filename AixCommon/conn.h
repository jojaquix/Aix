#pragma once


#include <AixCommon/Common.h>
#include <AixCommon/message.h>
#include <AixCommon/TsQueue.h>
#include <asio.hpp>

/**
 wrapper internal asio socket etc.
*/

namespace aix{

	/*	
		this is a low level data type
		that is reason is in lowercase
		it is a handler for connections used by
		server and clients
	*/
	template<typename T>
	class connection : public std::enable_shared_from_this<connection<T>> {
	public:		
		connection(bool is_connection_to_server, asio::io_context& asio_context, asio::ip::tcp::socket socket)
			: asio_context(asio_context), socket(std::move(socket)), is_connection_to_server(is_connection_to_server)
		{
		}


		void connectToClient(std::string client_id) {
			if (!this->is_connection_to_server) {
				if (m_socket.is_open())
				{
					client_id = client_id;
					ReadHeader();
				}
			}
			else {
				std::cout << "[connection::Trace] this is connection to server, it cannot be used to connect to client" << std::endl;
			}
		}

		void connectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
			if (this->is_connection_to_server) {
				asio::async_connect(socket, endpoints,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
						if (!ec) {
							//asyncReadHeader();
						}
					});
			}
			else {
				std::cout << "[connection::Trace] this is not connection to server, cannot be used to connect to server" << std::endl;
			}
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
		/*
		void asyncReadHeader()
		{
			asio::async_read(m_socket, asio::buffer(&tempInMsg.header, sizeof(mheader<T>)),
				[this](std::error_code ec, std::size_t length)
				{
					if (!ec)
					{
						if (tempInMsg.header.size > 0)
						{
							tempInMsg.body.resize(tempInMsg.header.size);
							asyncReadBody();
						}
						else
						{
							processIncomingMessage();
						}
					}
					else
					{
						std::cout << "[" << id << "] fail reading header" << std::endl;
						m_socket.close();
					}
				});
		}

		
		void asyncReadBody()
		{
			asio::async_read(m_socket, asio::buffer(tempInMsg.body.data(), tempInMsg.body.size()),
				[this](std::error_code ec, std::size_t length)
				{
					if (!ec)
					{
						processIncomingMessage();
					}
					else
					{
						
						std::cout << "[" << id << "] fail reading body" << std::endl;
						m_socket.close();
					}
				});
		}


		void processIncomingMessage() {
			
			//store incomming message and wait for more

			asyncReadHeader();
		}
		*/


		asio::io_context& asio_context;
		asio::ip::tcp::socket socket;
		bool is_connection_to_server;

		std::string client_id;
		TsQueue<message<T>> qMessagesOut;
		//TsQueue<owned_message<T>>& qMessagesIn;
		message<T> tempInMsg;
	};
}