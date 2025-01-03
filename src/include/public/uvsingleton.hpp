#pragma once

#include <mutex>

template<typename T>
class CUVSingleton {
public:
	CUVSingleton(const CUVSingleton&) = delete;
	CUVSingleton& operator=(const CUVSingleton&) = delete;

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
T* CUVSingleton<T>::_instance = nullptr;