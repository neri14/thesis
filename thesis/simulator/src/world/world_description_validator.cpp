#include "world_description_validator.h"
#include <boost/foreach.hpp>

namespace simulator {
namespace world {

world_description_validator::world_description_validator(world_description_handle desc_) :
	logger("world_description_validator"),
	desc(desc_)
{}

bool world_description_validator::validate()
{
	bool result = true;
	result = check_loose_ends() && result;

	if (result) {
		logger.info()() << "world description is correct";
	}
	return result;
}

bool world_description_validator::check_loose_ends()
{
	typedef std::pair<std::string, world_node_handle> node_pair_t;
	typedef std::pair<int, world_connection_handle> conn_pair_t;

	bool result = true;
	BOOST_FOREACH(node_pair_t pair, desc->nodes) {
		BOOST_FOREACH(conn_pair_t end, pair.second->entrances) {
			if (!end.second) {
				logger.error()() << "loose end in node: " << pair.second->name;
				result = false;
			}
		}
		BOOST_FOREACH(conn_pair_t end, pair.second->exits) {
			if (!end.second) {
				logger.error()() << "loose end in node: " << pair.second->name;
				result = false;
			}
		}
	}

	if (result) {
		logger.debug()() << "no loose ends";
	}
	return result;
}

} // namespace world
} // namespace simulator
