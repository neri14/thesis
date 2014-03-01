all: print archive stats

print:
	sh ./build/print.sh

archive:
	sh ./build/archive.sh

stats: print archive
	sh ./build/stats.sh

clean:
	rm -rf ./bin/printmode/*
	rm -rf ./bin/archivemode/*
	rm -rf ./bin/out/*

.PHONY: all print archive stats clean
