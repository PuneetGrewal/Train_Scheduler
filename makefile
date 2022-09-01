.phony all:
all: train_scheduler

mts: train_scheduler.c
	gcc train_scheduler.c -pthread -o mts

.PHONY clean:
clean:
	-rm -rf *.0 *.exe
