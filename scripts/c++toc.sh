#!/bin/bash
#set -Eeuo pipefail
#trap 'echo "$LINENUM"' ERR

#curl -SsC - -o /tmp/1 http://www.cplusplus.com/reference/algorithm/adjacent_find/
#cat /tmp/1 \
#| xmllint --html --xpath '//section[@id="description"]//code' - 2>/dev/null \
#| sed 's/<code>/\n<code>/g' \
#| grep -A1000 template \
#| sed 's/<[^>]*>//g;s/&lt;/</g;s/&gt;/>/;' > /tmp/2

echo '
template <class InputIterator, class T>
   T accumulate (InputIterator first, InputIterator last, T init)
{
  while (first!=last) {
    init = init + *first;
    ++first;
  }
  return init;
}
' > /tmp/1


gen() {
	local file="$1" tmp tmplate template_args funcrettype funcname funcargs macro_args
	tmp=$(cat "$file" \
		| grep template \
		| sed \
		's/^\(.*\)[[:space:]]*<\(.*\)>[[:space:]]\(.*\)[[:space:]]\(.\+\)[[:space:]]*\((.*)\)$/\1\n<\2>\n\3\n\4\n\5\n/'
	)
	readarray tmp <<<"$tmp"
	
	template=$(echo "${tmp[0]}" | tr -d '\n')
	template_args=$(echo "${tmp[1]}" | tr -d '\n')
	funcrettype=$(echo "${tmp[2]}" | tr -d '\n')
	funcname=$(echo "${tmp[3]}" | tr -d '\n' | sed 's/[[:space:]]*//g')
	funcargs=$(echo "${tmp[4]}" | tr -d '\n')
	
	IFS=',' template_args=($(echo "$template_args" | sed 's/.*<//;s/>.*//;s/[[:space:]]*class[[:space:]]*/class/g;s/[[:space:]]*typename[[:space:]]*//g;s/[[:space:]]*//g;'))
	template_args=("${template_args[@]##class}")
	macro_args="(N,$(printf "%s," "${template_args[@]}"|sed 's/,$//;s/[[:space:]]*//g'))"
	
	echo -n "#define NAMESPACE_H_${funcname}${macro_args}"
	echo ' \'
	echo $'\t'"${funcrettype} N##${funcname}${funcargs}"
	echo -n "#define NAMESPACE_C_${funcname}${macro_args}"
	echo ' \'
	echo $'\t'"NAMESPACE_H_${funcname}${macro_args} \\"
	in=0
	cat "$file" | grep -v template | sed '/^$/d' | while read line; do 
		case "$line" in 
		*{*) ((in++)); ;;
		*}*) ((in--)); ;;
		esac
		if [ "$in" -eq 0 ]; then
			echo "$line"
			break
		else
			echo "$line" ' \'; 
		fi
	done | sed \
	's/std::/N##/g;/\([a-zA-Z_][a-zA-Z0-9_]*\) == \([a-zA-Z_][a-zA-Z0-9_]*\)/{
	s/++\([a-zA-Z_][a-zA-Z0-9_]*\) == ++\([a-zA-Z_][a-zA-Z0-9_]*\)/(++\1, ++\2, !memcmp(\1, \2, sizeof(*\1)))/g;
	s/\([a-zA-Z_][a-zA-Z0-9_]*\) == ++\([a-zA-Z_][a-zA-Z0-9_]*\)/(++\2, !memcmp(\1, \2, sizeof(*\1)))/g;
	s/++\([a-zA-Z_][a-zA-Z0-9_]*\) == \([a-zA-Z_][a-zA-Z0-9_]*\)/(++\1, N##op_equal(\1, \2))/g;
	s/\([a-zA-Z_][a-zA-Z0-9_]*\) == \([a-zA-Z_][a-zA-Z0-9_]*\)/(!memcmp(\1, \2, sizeof(*\1)))/g;
	}'
}

#gen /tmp/1

f=/usr/include/c++/6/bits/stl_algo.h
nums=($(cat "$f" | grep -n template | sed 's/\([0-9]*\):.*/\1/'))
cat /usr/include/c++/6/bits/stl_algo.h | sed 's/\/\/.*//' | indent -l-1 > /tmp/1
f=/tmp/1
for i in ${nums[@]}; do
	tail -n+$i "$f" > /tmp/2
	gen /tmp/2
done









