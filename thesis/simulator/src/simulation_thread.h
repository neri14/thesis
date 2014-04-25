
//TODO simulation_thread::start
// - start up timer - tick event (1 second) - for event to be given (second to pass) -
//               1second timer needs to pass AND new states need to be calculated
// - creators/destroyers should work based on tick event
// - tick event should cause new state calculation - after actuators are finished and sensors sent their events

//TODO simulation_thread::stop
// - simulation stop is based on stopping tick event generator or stops itself if duration is exceeded