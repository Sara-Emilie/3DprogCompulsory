#pragma once
#include <map>
#include <iostream>
#include <stdexcept>

template <typename T>
class ComponentManager {
private:
	std::map<int, T> components;
public:
	void AddComponent(int ID, T component) 
	{
		components[ID] = component;
	}

	 T GetComponent(int ID) 
	{
		 if (components.find(ID) != components.end()) 
		 {
			 return components[ID];
		 }
		 throw std::runtime_error("Component not found");
	 }

	 bool HasComponent(int ID) 
	 {
		 return components.find(ID) != components.end();
	 }

	 void RemoveComponent(int ID) 
	 {
		 components.erase(ID);
	 }

	 std::map<int, T>GetAllComponents() 
	 {
		 return components;
	 }
};

