1. Build targets
====================

make <target> <options>

<target>	:=	help
				system
				<component>
				test
				<component>_test
				test_run
				<component>_test_run
				clean
				proto
				proto_clean

<component>	:=	common		//only _test and _test_run targets
				controller
				dispatcher_generic_client
				dispatcher_server
				manager
				simulator
				visualiser

<options>   :=  debug=1
				testfilter=<filter>


2. Description
====================

help
	Shows this file

system
	Builds all components

<component>
	Builds specified component

test
	Builds all UTs

<component>_test
	Builds specified component UTs

test_run
	Runs all UTs

<component>_test_run
	Runs UTs of specified component

clean
	Cleans repository

proto
	Prepares proto cpp files, requires installed protobuf

proto_clean
	Cleans proto cpp files

debug=1
	Prepares cmake for compilation with debug flags

testfilter=<filter>
	Runs tests with given filter
