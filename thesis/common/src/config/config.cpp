#include "config.h"
#include <my_logger.h>

#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace common {

bool config::load(int argc, char** argv)
{
	boost::program_options::options_description desc("Options");
	desc.add_options()
		("config", boost::program_options::value<std::string>(),"Config file location");

	try {
		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

		if (vm.count("config")) {
			return load_file(vm["config"].as<std::string>());
		}
	} catch (std::exception&) {
		std::cerr << desc << std::endl;
		return false;
	}
	std::cerr << desc << std::endl;
	return false;
}

std::string config::config_filename()
{
	return filename;
}

bool config::load_file(const std::string& filename_)
{
	filename = filename_;

	boost::property_tree::ptree pt;

	try {
		boost::property_tree::ini_parser::read_ini(filename, pt);
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		return false;
	}

	load_tree(pt);

	return true;
}

void config::add(const std::string& key, const std::string& value)
{
	cfg_map.insert(std::make_pair(key, value));
}

void config::load_tree(const boost::property_tree::ptree& pt)
{
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, pt) {
		if (v.second.data().length()) {
			add(v.first, v.second.data());
			std::cout << "config: " << v.first << " = " << v.second.data() << std::endl;
		}
		load_tree(v.second);
	}
}

config& get_config()
{
	static config instance;
	return instance;
}

} // namespace common
