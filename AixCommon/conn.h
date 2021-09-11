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
		is a wrapper to asio socket
	*/
	template<typename T>
	struct conn {
		asio::io_context& asio_context;
		asio::ip::tcp::socket socket;
		message<T> tempInMsg;

		conn(asio::io_context& asio_context, asio::ip::tcp::socket socket)
			: asio_context(asio_context), socket(std::move(socket))
		{
		}
	};

}