all: generate stats

generate:
	sh ./build/generate.sh

stats: generate
	sh ./build/stats.sh

clean:
	rm -rf ./bin/tmp/*
	rm -rf ./bin/out/*

.PHONY: all generate stats clean
