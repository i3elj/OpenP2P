.PHONY: build release clean

build: build/Desktop_Debug
	cmake --build build/Desktop_Debug --target openp2p_app

release: build/Desktop_Release
	cmake --build build/Desktop_Release --target openp2p_app

build/Desktop_Debug:
	cmake -S . -B build/Desktop_Debug -DCMAKE_BUILD_TYPE=Debug

build/Desktop_Release:
	cmake -S . -B build/Desktop_Release -DCMAKE_BUILD_TYPE=Release

clean:
	rm -rf build/Desktop* .cache
