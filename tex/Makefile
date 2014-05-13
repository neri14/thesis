all: generate stats

generate:
	sh ./build/generate.sh

stats: generate
	sh ./build/stats.sh

spelling: generate stats
	ispell -dpolish ./bin/out/thesis.txt

clean:
	rm -rf ./bin/tmp/*
	rm -rf ./bin/out/*

.PHONY: all generate stats clean
