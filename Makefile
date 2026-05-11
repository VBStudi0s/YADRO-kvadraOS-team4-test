.PHONY: build

PERIOD=1000
PATH=/home

build:
	mkdir -p build && cd build && cmake .. && make

run:
	./build/media_list --period ${PERIOD} --path ${PATH}

clean:
	rm -rf ./build
