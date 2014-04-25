#ifndef PATH_H
#define PATH_H

#include "cell.h"
#include "creator.h"

namespace simulator {
namespace simulation {

struct path_cell
{
	path_cell(cell_handle cell_h_, int entrance_, int exit_);

	cell_handle cell_h;
	int entrance;
	int exit;
};

class path
{
public:
	path(const std::string& name_, creator_handle creator_h_);

	void add_cell(cell_handle c, int entrance, int exit);
	void set_flow(int start_time, int flow);

	std::queue<path_cell> get_cells() const;
	const std::string& get_name() const;

private:
	std::string name;
	creator_handle creator_h;

	std::queue<path_cell> cells;
	std::map<int, int> flows;
};
typedef boost::shared_ptr<path> path_handle;

} // namespace simulation
} // namespace simulator

#endif /* PATH_H */