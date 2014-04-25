#ifndef CELL_H
#define CELL_H

#include <boost/assign.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

#include "vehicle.h"

namespace simulator {
namespace simulation {

enum EExitState {
	EExitState_Off,
	EExitState_Red,
	EExitState_RedYellow,
	EExitState_Green,
	EExitState_Yellow
};

class cell
{
public:
	cell(int priority_entrance = 0);

	bool is_exit_allowed(int exit);
	bool set_exit_state(int exit, EExitState state);
	EExitState get_exit_state(int exit);

	void increment_vehicle_counter();
	int get_vehicle_counter_value();

	bool is_occupied();

	void add_prev(int entrance, boost::weak_ptr<cell> c);
	void add_next(int exit, boost::weak_ptr<cell> c);
	boost::weak_ptr<cell> get_prev(int ent);
	boost::weak_ptr<cell> get_next(int ex);

private:
	int priority_entrance_number;
	int vehicle_counter;

	vehicle_handle current_vehicle;

	std::map<boost::weak_ptr<cell>, EExitState> exit_states;
	boost::mutex mtx_exit_states;

	std::map<int, boost::weak_ptr<cell> > prev;
	std::map<int, boost::weak_ptr<cell> > next;
};
typedef boost::shared_ptr<cell> cell_handle;

} // namespace simulation
} // namespace simulator

#endif /* CELL_H */
