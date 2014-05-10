#ifndef PATH_H
#define PATH_H

#include "cell.h"
#include "path_cell.h"

namespace simulator {
namespace simulation {

class path
{
public:
	path(const std::string& name_);

	path_cell* add_cell(cell_handle c, int entrance, int exit);
	void set_flow(int start_time, int flow);

	std::queue<path_cell> get_cells() const;
	const std::string& get_name() const;
	int get_flow(int time_tick) const;
	int get_offset() const;

private:
	mutable common::my_logger logger;
	std::string name;

	std::queue<path_cell> cells;
	std::map<int, int> flows;

	int offset;
	int max_flow;
};
typedef boost::shared_ptr<path> path_handle;

} // namespace simulation
} // namespace simulator

#endif /* PATH_H */