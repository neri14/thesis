#include "config.h"
#include <my_logger.h>

#include <boost/program_options.hpp>

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

	//TODO load file
	return true;
}

config& get_config()
{
	static config instance;
	return instance;
}

} // namespace common
