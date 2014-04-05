#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

#include <my_logger.h>

#include "../world/world_description.h"
#include "cell.h"

namespace simulator {
namespace simulation {

class simulation
{
public:
	simulation(const std::string& desc_filename_);

	bool prepare();

private:
	bool translate_to_cell_representation(world::world_description_handle desc);

	common::my_logger logger;
	std::string desc_filename;

	std::set<cell_handle> cells;
	std::map<std::string, cell_handle> cell_names;
};

} // namespace simulation
} // namespace simulator

#endif /* SIMULATION_H */
