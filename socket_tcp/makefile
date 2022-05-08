Args = -Wall -g

ALL:server.out client.out

server.out: ./src/server.c ./src/warp.c
	gcc $^ -o $@ $(Args) -I ./inc
client.out: ./src/client.c ./src/warp.c
	gcc $^ -o $@ $(Args) -I ./inc

clean:
	-rm server.out client.out

.PHONY: clean ALL

