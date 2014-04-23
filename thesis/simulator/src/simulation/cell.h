#ifndef CELL_H
#define CELL_H

#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

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
	cell();

	void add_prev(int entrance, boost::weak_ptr<cell> c);
	void add_next(int exit, boost::weak_ptr<cell> c);
	bool is_exit_allowed(int exit);

	//TODO void set_exit_state // mutex protected
	//TODO EExitState get_exit_state //mutex protected

	void increment_vehicle_counter();
	int get_vehicle_counter_value();

	bool is_occupied();

	std::map<int, boost::weak_ptr<cell> > prev;
	std::map<int, boost::weak_ptr<cell> > next;

	std::map<boost::weak_ptr<cell>, EExitState> exit_states;
	//FIXME exit_state - private

	int priority_entrance_number;

	bool occupied;//FIXME replace bool with ptr to vehicle

private:
	int vehicle_counter;
};
typedef boost::shared_ptr<cell> cell_handle;

} // namespace simulation
} // namespace simulator

#endif /* CELL_H */
