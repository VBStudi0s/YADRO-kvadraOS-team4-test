.PHONY: build

build:
	mkdir build && cd build && cmake .. && make

run:
	./build/media_list

clean:
	rm -rf ./build
