
function(generateAixCommon)
	set(headers "Common.h" "Message.h"  "TsMap.h")
	set(srcs "Common.cpp")
	add_library(AixCommon ${headers} ${srcs})
	target_link_libraries(AixCommon
		asio
		cxx_settings
	)


endfunction()


generateAixCommon()