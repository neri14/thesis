#ifndef PATH_CELL_H
#define PATH_CELL_H

#include "cell.h"

namespace simulator {
namespace simulation {

struct path_cell
{
	path_cell(cell_handle cell_h_, int entrance_, int exit_);

	cell_handle cell_h;
	int entrance;
	int exit;
};

}
}

#endif