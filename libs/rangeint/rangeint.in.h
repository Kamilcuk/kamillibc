/*
 * @file
 * @author Kamil Cukrowski <kamil@leonidas>
 * @date 2022-03-26
 * SPDX-License-Identifier: MIT AND Beerware
 */
#ifndef RANGEINT_H_
#define RANGEINT_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <limits.h>
#include <stdbool.h>

#define _ri_CONCAT(a, b)   a##b
#define _ri_XCONCAT(a, b)  _ri_CONCAT(a, b)
#define _ri_memb(b)        _ri_XCONCAT(RI_NAME, b)

#ifdef __cplusplus
}
#endif
#endif /* RANGEINT_H_ */

#ifdef __cplusplus
extern "C" {
#endif

/* sanitize input ------------------------------------------------------------------------- */

#ifndef RI_MAX
#error  RI_MAX not defined
#endif
#ifndef RI_MIN
#define RI_MIN  0
#endif
#ifndef RI_NAME
#error  RI_NAME not defined
#endif
#if    RI_MIN >= RI_MAX
#error RI_MIN >= RI_MAX
#endif

{% macro N(text) %}{{ "_ri_memb(_" + text + ")" }}{% endmacro %}
{% set T = "RI_NAME" %}
{% set UT = "_ri_memb(__ut)" %}

#if RI_MIN == 0
	#if RI_MAX <= UCHAR_MAX
typedef unsigned char {{T}};
	#elif RI_MAX <= USHRT_MAX
typedef unsigned short {{T}};
	#elif RI_MAX <= UINT_MAX
typedef unsigned int {{T}};
	#elif RI_MAX <= ULONG_MAX
typedef unsigned long {{T}};
	#elif RI_MAX <= ULLONG_MAX
typedef unsigned long long {{T}};
	#else
#error RI_MAX too big
	#endif
typedef {{T}} {{UT}};
#else
	#if SCHAR_MIN <= RI_MIN && RI_MAX <= SCHAR_MAX
typedef signed char {{T}};
typedef unsigned char {{UT}};
	#elif SSHRT_MIN <= RI_MIN && RI_MAX <= SSHRT_MAX
typedef signed short {{T}};
typedef unsigned short {{UT}};
	#elif SINT_MIN <= RI_MIN && RI_MAX <= SINT_MAX
typedef signed int {{T}};
typedef unsigned int {{UT}};
	#elif SLONG_MIN <= RI_MIN && RI_MAX <= SLONG_MAX
typedef signed long {{T}};
typedef unsigned long {{UT}};
	#elif SLLONG_MIN <= RI_MIN && RI_MAX <= SLLONG_MAX
typedef signed long long {{T}};
typedef unsigned long long {{UT}};
	#else
	#error RI_MAX too low or RI_MAX too big
	#endif
#endif

static const {{T}} {{N("min")}} = ({{T}})(RI_MIN);
static const {{T}} {{N("max")}} = ({{T}})(RI_MAX);

/// Check if the value is in range of RI_NAME type.
static inline bool {{N("is_in_range")}}({{T}} a) {
	return RI_MIN <= a && a <= RI_MAX;
}
/// Convert the value to be in range of RI_NAME type
/// by repeadetely adding or substracting the (range + 1)
/// to the value until it is in range.
/// Additionally, if `is` is not NULL, set it to true if
/// the value was not in range.
static inline {{T}} {{N("from_o")}}({{T}} a, bool *is) {
	const {{T}} diff = ({{T}})(RI_MAX) - ({{T}})(RI_MIN) + 1;
	if (a < ({{T}})RI_MIN) {
		if (is) *is = true;
		do {
			a += diff;
		} while (a < ({{T}})RI_MIN);
	} else if (({{T}})RI_MAX < a) {
		if (is) *is = true;
		do {
			a -= diff;
		} while (({{T}})RI_MAX < a);
	} else {
		if (is) *is = false;
	}
	return a;
}
// The same as *_from_o.
static inline {{T}} {{N("from")}}({{T}} a) {
	return {{N("from_o")}}(a, 0);
}
{% for OP in [
	["pos", "+"],
	["neg", "-"],
] %}
static inline {{T}} {{N(OP.0 + "_o")}}({{T}} a, bool *ov) {
	return {{N("from_o")}}( {{OP.1}} ({{UT}})a, ov );
}
static inline {{T}} {{N(OP.0)}}({{T}} a) {
	return {{N("from_o")}}( {{OP.1}} ({{UT}})a, 0 );
}
{% endfor %}
{% for OP in [
	["add", "+"],
	["sub", "-"],
	["mul", "*"],
	["div", "/"],
	["mod", "%"],
	["lshift", "<<"],
	["rshift", ">>"],
	["xor", "^"],
	["and", "&"],
	["or", "|"],
] %}
static inline {{T}} {{N(OP.0 + "_o")}}({{T}} a, {{T}} b, bool *ov) {
	return {{N("from_o")}}( ({{UT}})a {{OP.1}} ({{UT}})b, ov );
}
static inline {{T}} {{N(OP.0)}}({{T}} a, {{T}} b) {
	return {{N("from_o")}}( ({{UT}})a {{OP.1}} ({{UT}})b, 0 );
}
{% endfor %}

#undef RI_MAX
#undef RI_MIN
#undef RI_NAME

#ifdef __cplusplus
}
#endif
