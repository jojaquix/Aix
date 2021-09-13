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
		connection(	bool is_connection_to_server, 
					asio::io_context& asio_context,	
					asio::ip::tcp::socket socket,
					TsQueue<owned_message<T>>& qMessageIn
					)
			: asio_context(asio_context), 
			socket(std::move(socket)),
			qMessagesIn(qMessageIn),			
			is_connection_to_server(is_connection_to_server)
		{
		}

		std::string getId() const
		{
			return client_id;
		}

		void setId(std::string id) {
			client_id = id;
		}



		void connectToClient(std::string client_id) {
			if (!this->is_connection_to_server) {
				if (socket.is_open())
				{
					client_id = client_id;
					asyncReadHeader();
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
							asyncReadHeader();
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

		void send(const message<T>& msg)
		{
			asio::post(asio_context,
				[this, msg]()
				{
					bool writingMessage = !qMessagesOut.empty();
					qMessagesOut.push_back(msg);
					if (!writingMessage)
					{
						asyncWriteHeader();
					}
				});
		}



	private:
		
		void asyncReadHeader()
		{
			asio::async_read(socket, asio::buffer(&tempInMsg.header, sizeof(mheader<T>)),
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
							enqueueIncomingMessage();
						}
					}
					else
					{
						std::cout << "[" << client_id << "] fail reading header" << std::endl;
						socket.close();
					}
				});
		}

		
		void asyncReadBody()
		{
			asio::async_read(socket, asio::buffer(tempInMsg.body.data(), tempInMsg.body.size()),
				[this](std::error_code ec, std::size_t length)
				{
					if (!ec)
					{
						enqueueIncomingMessage();
					}
					else
					{
						
						std::cout << "[" << client_id << "] fail reading body" << std::endl;
						socket.close();
					}
				});
		}

		void enqueueIncomingMessage() {			
			//store incomming message and wait for more
			
			if (this->is_connection_to_server)
				qMessagesIn.push_back({ nullptr, tempInMsg });
			else
				qMessagesIn.push_back({ this->shared_from_this(), tempInMsg });
			
		
			asyncReadHeader();
		}

		void asyncWriteHeader()
		{
			asio::async_write(socket, asio::buffer(&qMessagesOut.front().header, sizeof(mheader<T>)),
				[this](std::error_code ec, std::size_t length)
				{
					if (!ec)
					{
						//if the message has body
						if (qMessagesOut.front().body.size() > 0)
						{
							asyncWriteBody();
						}
						else
						{
							qMessagesOut.pop_front();
							if (!qMessagesOut.empty())
							{
								asyncWriteHeader();
							}
						}
					}
					else
					{
						std::cout << "[" << client_id << "] fail write header." << std::endl;
						socket.close();
					}
				});
		}

		void asyncWriteBody()
		{
			asio::async_write(socket, asio::buffer(qMessagesOut.front().body.data(), qMessagesOut.front().body.size()),
				[this](std::error_code ec, std::size_t length)
				{
					if (!ec)
					{
						qMessagesOut.pop_front();
						if (!qMessagesOut.empty())
						{
							asyncWriteHeader();
						}
					}
					else
					{
						std::cout << "[" << client_id << "] fail write body." << std::endl;
						socket.close();
					}
				});
		}
		

		asio::io_context& asio_context;
		asio::ip::tcp::socket socket;
		bool is_connection_to_server;

		std::string client_id;
		TsQueue<message<T>> qMessagesOut;
		TsQueue<owned_message<T>>& qMessagesIn;
		message<T> tempInMsg;
	};
}