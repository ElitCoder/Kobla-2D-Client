#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <sstream>

struct NoConfigException {
};

class Config {
public:
	void parse(const std::string& filename);
	void clear();
	
	std::map<std::string, std::string>& internal();
	
	template<class T>
	T get(const std::string& key) {
		auto iterator = configs_.find(key);
		
		if (iterator == configs_.end())
			throw NoConfigException();
			
		std::istringstream stream(iterator->second);
		T value;
		stream >> value;
		
		return value;
	}
	
private:
	void add(const std::pair<std::string, std::string>& config);
	
	std::map<std::string, std::string> configs_;
};

#endif