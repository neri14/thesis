all: print archive

print:
	sh ./build/print.sh

archive:
	sh ./build/archive.sh

clean:
	rm -rf ./bin/printmode/*
	rm -rf ./bin/archivemode/*
	rm -rf ./bin/out/*

.PHONY: all print archive clean
