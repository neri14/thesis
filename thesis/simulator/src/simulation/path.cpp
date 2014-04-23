#include "path.h"

namespace simulator {
namespace simulation {

path_cell::path_cell(cell_handle cell_h_, int entrance_, int exit_) :
	cell_h(cell_h_),
	entrance(entrance_),
	exit(exit_)
{}

path::path(const std::string& name_, creator_handle creator_h_) :
	name(name_),
	creator_h(creator_h_)
{}

void path::add_cell(cell_handle c, int entrance, int exit)
{
	cells.push(path_cell(c, entrance, exit));
}

std::queue<path_cell> path::get_cells() const
{
	return cells;
}

} // namespace simulation
} // namespace simulator