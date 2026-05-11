.PHONY: build

build:
	mkdir -p build && cd build && cmake .. && make

run:
	./build/media_list --period 1000 --path ./test

clean:
	rm -rf ./build
