#ifndef CREATOR_H
#define CREATOR_H

#include "cell.h"

namespace simulator {
namespace simulation {

class creator
{
public:
	creator(cell_handle cell_, int max_create_rate_);

	//TODO method "create" - creates vehicles according to saved path flow and max_create_rate

private:
	cell_handle cell;
	int max_create_rate;
};
typedef boost::shared_ptr<creator> creator_handle;

} // namespace simulation
} // namespace simulator

#endif /* CREATOR_H */