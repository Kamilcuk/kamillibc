#!/bin/bash
set -euo pipefail

for v in f0 f1 f2 f4 l0 l1 l4; do
	V=$(tr '[:lower:]' '[:upper:]' <<<"$v")
	l=( STM32$V STM32${v}xx STM32${V}XX )

	echo -n "#if "; for i in "${l[@]}"; do echo -n "defined($l) || "; done | sed 's/ || $//'; echo;
	for i in "${l[@]}"; do
		cat <<EOF
# ifndef $i
#  define $i 1
# endif
EOF
	done
	cat <<EOF
# include "stm32${v}xx.h"
# include "stm32${v}xx_hal.h"
#endif

EOF
done
	
