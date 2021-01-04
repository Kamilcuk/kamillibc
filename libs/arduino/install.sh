#!/bin/bash
set -euo pipefail

name=install.sh
usage() {
	cat <<EOF
Usage: $name [options]

Options
   -K --ok
   -q --quiet
   -h --help
EOF
}

fatal() {
	printf "%s\n" "$*" >&2
	exit 1
}

log() {
	if "$verbose"; then
		echo "$*"
	fi
}

###############################################################################

args=$(getopt -n "$name" -o Kqh -l ok,quiet,help -- "$@")
eval set -- "$args"
ok=false
verbose=true
while (($#)); do
	case "$1" in
	-K|--ok) ok=true; ;;
	-q|--quiet) verbose=false; ;;
	-h|--help) usage; exit ;;
	--) shift; break; ;;
	esac
	shift
done
if (($#)); then
	usage
	fatal ""
fi

dryrun() { echo "+" "$@"; }
if "$ok"; then
	dryrun() { "$@"; }
else
	log "DRYRUN: add --ok to really do changes"
fi

cd "$(dirname "$0")"
for i in */; do
	name=$(basename "$i")
	src="$(readlink -f "$i")"
	dest=~/Arduino/libraries/"$name"
	if [[ -L "$dest" ]]; then
		if [[ "$(readlink "$dest")" == "$src" ]]; then
			log "Library $name already installed"
			continue
		else
			log "Library $name already installed somewhere else"
		fi
	else
		log "Library $name will be installed fresh"
	fi
	dryrun ln -nvfs "$src" "$dest"
done

