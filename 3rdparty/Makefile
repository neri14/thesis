all: boost_1_55_0 gmock-1.7.0

boost_1_55_0:
	tar -xzf boost_1_55_0.tar.gz

gmock-1.7.0:
	unzip gmock-1.7.0.zip

clean: clean_boost clean_gmock

clean_boost:
	rm -rf boost_1_55_0

clean_gmock:
	rm -rf gmock-1.7.0

.PHONY: all clean clean_boost clean_gmock
