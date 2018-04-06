#!/bin/bash

args="$(for i in $(seq 0 255); do printf "s/0b%08d/0x%02x/g;" $(bc <<<"obase=2;$i") $i; done)"
sed "$args" "$@" 