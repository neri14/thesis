all: print archive debug stats

print:
	sh ./build/print.sh

archive:
	sh ./build/archive.sh

debug:
	sh ./build/debug.sh

stats: print archive debug
	sh ./build/stats.sh

clean:
	rm -rf ./bin/printmode/*
	rm -rf ./bin/archivemode/*
	rm -rf ./bin/debugmode/*
	rm -rf ./bin/out/*

.PHONY: all print archive stats clean
