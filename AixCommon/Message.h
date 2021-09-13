#pragma once

#include<AixCommon/Common.h>
#include <AixCommon/conn.h>

namespace aix {
	template <typename T>
	struct mheader
	{
		T mtype{};
		uint32_t size = 0; //size of message including header and body
	};

	template <typename T>
	struct message
	{
		mheader<T> header{};
		std::vector<uint8_t> body;
		
		template<typename DataType>
		friend message<T>& operator << (message<T>& msg, const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Only simple for messages types")
			size_t i = msg.body.size();
			msg.body.resize(msg.body.size() + sizeof(DataType));
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
			msg.header.size = msg.size();
			return sg;
		}

		template<typename DataType>
		friend message<T>& operator >> (message<T>& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Only simple datatypes supported");
			size_t i = msg.body.size() - sizeof(DataType);
			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
			msg.body.resize(i);
			msg.header.size = msg.size();
			return msg;
		}
	};

  /*
	struct owned_message
	{
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		friend std::ostream& operator<<(std::ostream& os, const owned_message<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};

	*/

}
