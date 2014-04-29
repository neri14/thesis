#include "path.h"

#include <boost/foreach.hpp>

namespace simulator {
namespace simulation {

path_cell::path_cell(cell_handle cell_h_, int entrance_, int exit_) :
	cell_h(cell_h_),
	entrance(entrance_),
	exit(exit_)
{}

path::path(const std::string& name_) :
	name(name_)
{}

path_cell* path::add_cell(cell_handle c, int entrance, int exit)
{
	cells.push(path_cell(c, entrance, exit));
	return &cells.back();
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

int path::get_flow(int time_tick) const
{
	int last_tick = 0;
	int flow = 0;

	typedef std::pair<int, int> flow_pair_type;
	BOOST_FOREACH(flow_pair_type p, flows) {
		if (p.first >= last_tick && p.first <= time_tick) {
			flow = p.second;
			last_tick = p.first;
		}
	}

	if (flow < 0) {
		flow = 0;
	}
	return flow;
}

} // namespace simulation
} // namespace simulator