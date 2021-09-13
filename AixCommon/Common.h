#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <array>

namespace aix {
	
	const int USER_NAME_MAX_SIZE = 256;
	
	enum class MsgTypes : uint32_t
	{
		//Messages from Server to Clients
		SetClientId,
		UserInsertOk,
		UserDuplicate,
		//Messages from Client to Server
		InsertUser
	};

	struct user_info {
		uint8_t id;
		char name[USER_NAME_MAX_SIZE];
	};
	
}



