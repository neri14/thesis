#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

namespace common {

typedef std::map<std::string, std::string> cfg_map_type;

class config : public boost::noncopyable
{
public:
	bool load(int argc, char** argv);

	std::string config_filename();

	template <typename T>
	T get(const std::string& key);

private:
	bool load_file(const std::string& filename_);

	std::string filename;
	cfg_map_type cfg_map;
};

config& get_config();

//impl
template <typename T>
T config::get(const std::string& key)
{
	try {
		cfg_map_type::iterator it = cfg_map.find(key);

		if (it != cfg_map.end()) {
			return boost::lexical_cast<T>(it->second);
		}
		return T();
	} catch (std::exception&) {
		return T();
	}
}

} // namespace common

#endif /* CONFIG_H */
