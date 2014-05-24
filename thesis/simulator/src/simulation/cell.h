#ifndef CELL_H
#define CELL_H

#include <boost/assign.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

#include <my_logger.h>

namespace simulator {
namespace simulation {

enum EExitState {
	EExitState_Off,
	EExitState_Red,
	EExitState_RedYellow,
	EExitState_Green,
	EExitState_Yellow
};

class cell;
typedef boost::shared_ptr<cell> cell_handle;

class cell
{
public:
	cell(int priority_entrance = 0);
	virtual ~cell();

	virtual bool is_exit_allowed(int exit);
	virtual bool set_exit_state(int exit, EExitState state);
	virtual EExitState get_exit_state(int exit);

	virtual void increment_vehicle_counter();
	virtual int get_vehicle_counter_value();

	virtual void add_prev(int entrance, boost::weak_ptr<cell> c);
	virtual void add_next(int exit, boost::weak_ptr<cell> c);
	virtual boost::weak_ptr<cell> get_prev(int ent);
	virtual boost::weak_ptr<cell> get_next(int ex);

	virtual bool is_occupied();
	virtual void set_occupied(bool occupied_, int speed=0);

	virtual int get_priority_entrance_number() const;
	virtual bool prev_vehicle_moving(int margin, const cell* next_cell, int gap=0);

	virtual void enable_destroyer_hack();
	virtual int get_entrances_count() const;

private:
	common::my_logger logger;

	int priority_entrance_number;
	int vehicle_counter;

	int vehicle_speed;
	bool occupied;
	bool destroyer_hack_enabled;

	std::map<boost::weak_ptr<cell>, EExitState> exit_states;
	boost::mutex mtx_exit_states;

	std::map<int, boost::weak_ptr<cell> > prev;
	std::map<int, boost::weak_ptr<cell> > next;
};

} // namespace simulation
} // namespace simulator

#endif /* CELL_H */
