#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <my_logger.h>

namespace simulator {
namespace simulation {

class simulation
{
public:
	simulation(const std::string& desc_filename_);

	bool prepare();

private:
	common::my_logger logger;
	std::string desc_filename;
};

} // namespace simulation
} // namespace simulator

#endif /* SIMULATION_H */
