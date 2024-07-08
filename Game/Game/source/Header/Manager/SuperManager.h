#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <utility>

class SuperManager 
{
public:
	SuperManager();
	~SuperManager();
	bool Add(std::string name, int id,class ManagerBase* manager);
	bool DeleteName(std::string);
	bool DelAll();
	class ManagerBase* GetManager(std::string name);
	bool Update();
	bool Draw();

	void Sort();
	static SuperManager* GetInstance() { return _instance;}
	void Skip() { _isProcessSkip = true; }
protected:
	static SuperManager* _instance;
	bool _isSort;
	std::vector<std::tuple<std::string,int,class ManagerBase*>> _superManager;
	std::vector<std::string> _delSuperManager;
	bool _isProcessSkip;
};

