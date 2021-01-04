#!/bin/bash
set -euo pipefail

flags=(
	-Wall
	-Wextra
	-Wno-format-zero-length
	-Wno-format-extra-args
	-DKC_TEST
	-std=c++11
	-ggdb3
	-fdiagnostics-color
)

f() {
	( set -x
	,c++run "$1" -fsanitize=address "${flags[@]}"
	#,c++run ---G "$1" "${flags[@]}"
	,c++run ---V "$1" -fno-exceptions "${flags[@]}"
	,c++run ---V "$1" "${flags[@]}"
	)
}

if ((!$#)); then
	set -- sbuffer.h ringbuffer.h
fi

for i; do
	f "$i"
done
