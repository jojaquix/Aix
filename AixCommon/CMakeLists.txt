
function(generateAixCommon)
	set(headers "Common.h" "message.h"  "TsMap.h" "AbsServer.h" "conn.h" "AbsServer (2).h")
	set(srcs "Common.cpp")
	add_library(AixCommon ${headers} ${srcs})
	target_link_libraries(AixCommon
		asio
		cxx_settings
	)


endfunction()


generateAixCommon()