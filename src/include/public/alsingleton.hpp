#pragma once

#include <mutex>

template<typename T>
class CALSingleton {
public:
	CALSingleton(const CALSingleton&) = delete;
	CALSingleton& operator=(const CALSingleton&) = delete;

	template<typename... Args>
	static T* instance(Args&&... args) {
		static std::once_flag flag;
		std::call_once(flag, [&]() { _instance = new T(std::forward<Args>(args)...); });
		return _instance;
	}

private:
	static T* _instance;
};

template<typename T>
T* CALSingleton<T>::_instance = nullptr;