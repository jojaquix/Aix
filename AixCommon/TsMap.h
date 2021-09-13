#pragma once


#include <AixCommon/Common.h>

namespace aix {
	
	
	/* this is thread safe basic dictionary */
	template<typename K, typename V>
	class TsMap {
	public:
		TsMap() = default;
		TsMap(const TsMap <K,V> &) = delete;
		virtual ~TsMap() { clear(); };


		void clear() {
			std::scoped_lock lock(theMutex);
			theMap.clear();
		}

		bool existsKey(K key) {
			std::scoped_lock lock(theMutex);
			if (theMap.find(key) == theMap.end())
				return false;
			return true;
		}

		void insert(const K& key, const V& value) {
			std::scoped_lock lock(theMutex);
			theMap.emplace(std::move(key), std::move(value));
		}
		
		std::unordered_map<K, V> getCopyMap() {
			std::unordered_map<K, V> copyMap;
			std::scoped_lock lock(theMutex);
			copyMap = theMap;
			return std::move(copyMap);			
		}

	private:		
		std::mutex theMutex;
		std::unordered_map<K, V> theMap;
	};


}