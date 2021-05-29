all:
	cmake -DCMAKE_C_FLAGS=-fsanitize=address -S. -B_build -GNinja
	cmake --build _build
	( cd _build && ctest -V )
testop:
	cmake -DCMAKE_C_FLAGS="-fsanitize=address -Wno-multichar" -S. -B_build -GNinja
	cmake --build _build --target kuint128_testop
	( cd _build && ctest -V -R testop )
clean:
	rm -rf _build
avr: mcu=atmega128
avr: all
	cmake -DCMAKE_C_COMPILER_WORKS=1 \
		-DCMAKE_C_COMPILER=avr-gcc -S . -B _build/avr \
		-DCMAKE_C_FLAGS="-mmcu=$(mcu) -DF_CPU=1000000 -Os -ffunction-sections -fdata-sections -Wl,-gc-sections -ggdb3" \
		-DBUILD_TESTING=1
	cmake --build _build/avr --verbose
	avr-nm -td --size-sort _build/avr/how_to_split_double_multiplication  | tail -n 20
	avr-size _build/avr/how_to_*

