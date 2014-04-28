#ifndef VEHICLE_H
#define VEHICLE_H

#include <boost/shared_ptr.hpp>
#include "path.h"

namespace simulator {
namespace simulation {

class vehicle
{
public:
	vehicle(path_handle p) {};
};
typedef boost::shared_ptr<vehicle> vehicle_handle;

}
}

#endif /* VEHICLE_H */