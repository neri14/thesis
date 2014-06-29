all: seminarka_1 seminarka_2 obrona

seminarka_1:
	sh ./build/sem_1.sh

seminarka_2:
	sh ./build/sem_2.sh

obrona:
	sh ./build/obrona.sh

clean:
	rm -rf ./bin/sem_1/*
	rm -rf ./bin/sem_2/*
	rm -rf ./bin/obr/*
	rm -rf ./bin/out/*

.PHONY: all seminarka_1 seminarka_2 obrona clean
