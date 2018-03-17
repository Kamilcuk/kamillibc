#!/bin/bash

# curl -sS http://en.cppreference.com/w/c/header + copy and save the file

file=${1:-stdall.h}

C95='199409L'
C99='199901L'
C11='201112L'
cmp="__STDC_VERSION__ >="
C95="#if $cmp $C95"
C99="#if $cmp $C99"
C11="#if $cmp $C11"

curl -sS http://en.cppreference.com/w/c/header | xmllint -html -xpath '//table[@class="t-dsc-begin"]' - | w3m -dump -cols 9999 -T text/html | grep -v 'C++ documentation for C++ Standard Library header files' \
| while read -r file r1 r2 r3; do 
	if [ -z "$file" ]; then continue; fi
	file="#include $file"
	if [ "$r1" = "(since" ]; then
		case "$r2" in
		C11*) c="$C11"; ;;
		C99*) c="$C99"; ;;
		C95*) c="$C95"; ;;
		*) echo "ERROR" >&2; exit 1; ;;
		esac

		echo "$c"
		echo "/* $r3 */"
		echo "$file"
		echo "#endif"
	else
		echo "/* $r1 $r2 $r3 */"
		echo "$file"
	fi
	echo
done

