#pragma once


#include <AixCommon/Common.h>

namespace aix {
	
	
	/* this is thread safe basic dictionary */
	template<typename T>
	class TsMap {
	public:
		TsMap() = default;
		TsMap(const TsMap<T>&) = delete;
		virtual ~TsMap() { Clear(); };

 
	
	
	};


}