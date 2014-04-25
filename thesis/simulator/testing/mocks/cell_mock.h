#ifndef CELL_MOCK_H
#define CELL_MOCK_H

#include <gmock/gmock.h>
#include "../../src/simulation/cell.h"

namespace simulator {
namespace simulation {

class cell_mock : public cell
{
public:
	cell_mock() : cell() {}
	MOCK_METHOD1(is_exit_allowed, bool(int));
	MOCK_METHOD2(set_exit_state, bool(int, EExitState));
	MOCK_METHOD1(get_exit_state, EExitState(int));
	MOCK_METHOD0(increment_vehicle_counter, void());
	MOCK_METHOD0(get_vehicle_counter_value, int());
	MOCK_METHOD0(is_occupied, bool());
	MOCK_METHOD2(add_prev, void(int, boost::weak_ptr<cell>));
	MOCK_METHOD2(add_next, void(int, boost::weak_ptr<cell>));
	MOCK_METHOD1(get_prev, boost::weak_ptr<cell>(int));
	MOCK_METHOD1(get_next, boost::weak_ptr<cell>(int));

};

} // namespace simulation
} // namespace simulator

#endif /* CELL_MOCK_H */