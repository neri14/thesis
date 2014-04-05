#include "simulation.h"

#include "../world/world_description_parser.h"
#include "../world/world_description_validator.h"

#include <config/config.h>

namespace simulator {
namespace simulation {

simulation::simulation(const std::string& desc_filename_) :
	logger("simulation"),
	desc_filename(desc_filename_)
{}

bool simulation::prepare()
{
	logger.info()() << "Parsing world description file";
	world::world_description_parser parser(common::get_config().get<std::string>("world_description_file"));
	if (!parser.parse()) {
		return false;
	}

	logger.info()() << "Validating world description";
	world::world_description_validator validator(parser.get_world_description());
	if (!validator.validate()) {
		return false;
	}

	logger.info()() << "Translating graph representation of world description to cellular representation";
	//TODO simulation::prepare cellular automata representation
	return true;
}

} // namespace simulation
} // namespace simulator
