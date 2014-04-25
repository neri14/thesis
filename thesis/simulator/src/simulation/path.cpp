#include "path.h"

namespace simulator {
namespace simulation {

path_cell::path_cell(cell_handle cell_h_, int entrance_, int exit_) :
	cell_h(cell_h_),
	entrance(entrance_),
	exit(exit_)
{
	//TODO register for time tick
	// on time tick create vehicle if should according to current flow
	// uses creator::create
}

path::path(const std::string& name_, creator_handle creator_h_) :
	name(name_),
	creator_h(creator_h_)
{}

void path::add_cell(cell_handle c, int entrance, int exit)
{
	cells.push(path_cell(c, entrance, exit));
}

void path::set_flow(int start_time, int flow)
{
	flows.insert(std::make_pair(start_time, flow));
}

std::queue<path_cell> path::get_cells() const
{
	return cells;
}

const std::string& path::get_name() const
{
	return name;
}

} // namespace simulation
} // namespace simulator