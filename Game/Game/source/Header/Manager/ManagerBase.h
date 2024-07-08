#pragma once
#include "dxlib.h"
#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
class ManagerBase
{
	public:
	virtual ~ManagerBase() = default;
	virtual bool Terminate() = 0;
	virtual bool UpdateInit();
	virtual bool Update();
	virtual bool UpdateEnd();
	virtual bool Draw();

	template<typename T>
	void Add(T* value) {
		void* ptr = static_cast<void*>(value);
		AddInput(ptr);
	};

	template<typename U>
	void Delete(U* value) {
		void* ptr = static_cast<void*>(value);
		DeleteInput(ptr);
	};

	virtual void AddInput(void* value){};

	virtual void DeleteName(std::string){};
	virtual void DeleteInput(void* value){};
	virtual void DelAll(){};

	std::string GetName() { return _name; }

protected:
	std::string _name;
};

