#ifndef WORLD_DESCRIPTION_VALIDATOR_H
#define WORLD_DESCRIPTION_VALIDATOR_H

#include <my_logger.h>

#include "world_description.h"

namespace simulator {
namespace world {

class world_description_validator
{
public:
	world_description_validator(world_description_handle desc_);

	bool validate();

private:
	bool check_loose_ends();

	common::my_logger logger;
	world_description_handle desc;
};

} // namespace world
} // namespace simulator

#endif /* WORLD_DESCRIPTION_VALIDATOR_H */
