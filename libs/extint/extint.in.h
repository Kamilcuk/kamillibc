/*
 * @file
 * @author Kamil Cukrowski <kamil@leonidas>
 * @date 2022-03-26
 * SPDX-License-Identifier: MIT AND Beerware
 */
#ifndef EXTINT_H_
#define EXTINT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ------------------------------------------------------------------------- */

{% macro THEBODY(U, I, N) %}
	{% set STYPE = "extint_s" + I|string %}
	{% set UTYPE = "extint_u" + I|string %}
	{% set TRIM = " & " + (2**I - 1)|string + "u" %}
/// Create a variable of type extint_u{{I}} with the given value.
#define extint_u{{I}}(v)  (extint_u{{I}}){v{{TRIM}}}
/// Create a variable of type extint_s{{I}} with the given value.
#define extint_s{{I}}(v)  (extint_s{{I}}){v{{TRIM}}}
static inline {{U}} _extint_get_u{{I}}({{UTYPE}} a) { return a.v; }
static inline {{U}} _extint_get_s{{I}}({{STYPE}} a) { return a.v; }
{% for OP in [
	["add", "+"],
	["sub", "-"],
	["div", "/"],
	["mul", "*"]
] %}
static inline {{UTYPE}} extint_{{OP.0}}_u{{I}}_v({{U}} a, {{U}} b) {
	return ({{UTYPE}}){ (a {{OP.1}} b){{TRIM}} };
}
{% endfor %}
	{% for OP in ["add", "sub", "div", "mul"] %}
static inline {{STYPE}} extint_{{OP}}_s{{I}}_v({{U}} a, {{U}} b) {
	return ({{STYPE}}){ extint_{{OP}}_u{{I}}_v(a, b).v };
}
static inline {{UTYPE}} extint_{{OP}}_u{{I}}({{UTYPE}} a, {{UTYPE}} b) {
	return ({{UTYPE}}){ extint_{{OP}}_u{{I}}_v(a.v, b.v).v };
}
static inline {{STYPE}} extint_{{OP}}_s{{I}}({{STYPE}} a, {{STYPE}} b) {
	return ({{STYPE}}){ extint_{{OP}}_u{{I}}_v(a.v, b.v).v };
}
	{% endfor %}
{% endmacro %}

{% for I in range(1, 65) %}
	{% set N = 8 if I <= 8 else 16 if I <= 16 else 32 if I <= 32 else 64 %}
typedef struct { uint_fast{{N}}_t v; } extint_u{{I}};
typedef struct {  int_fast{{N}}_t v; } extint_s{{I}};
{{ THEBODY("uint_fast" + N|string + "_t", I, N) }}
{% endfor %}

/* ------------------------------------------------------------------------- */

#if 0 && defined(__SIZEOF_INT128__)
#ifndef INT128_MAX
#define INT128_MAX   ((__int128)(((unsigned __int128) 1 << ((__SIZEOF_INT128__ * __CHAR_BIT__) - 1)) - 1))
#endif
#ifndef INT128_MIN
#define INT128_MIN   (-INT128_MAX - 1)
#endif
#ifndef UINT128_MAX
#define UINT128_MAX  ((2 * (unsigned __int128) INT128_MAX) + 1)
#endif
{% for I in range(65, 129) %}
typedef struct {          __int128 v; } extint_s{{I}};
typedef struct { unsigned __int128 v; } extint_u{{I}};
{{ THEBODY("unsigned __int128", I, 128) }}
{% endfor %}
#define _extint_get_func128(OP, a) \
	{% for I in range(65, 129) %}
		, extint_u{{I}}: extint_##OP##_u{{I}} \
		, extint_s{{I}}: extint_##OP##_s{{I}} \
	{% endfor %}
	/**/
#define _extint_type_to_idx128(a) \
	{% for I in range(65, 129) %}
		, extint_u{{I}}: {{I * 2}} \
		, extint_s{{I}}: {{I * 2 + 1}} \
	{% endfor %}
	/**/
#else
#define _extint_get_func128(OP, a)  /**/
#define _extint_type_to_idx128(a)  /**/
#endif  // __SIZEOF_INT128__

/* ------------------------------------------------------------------------- */

{% set TYPES = [
	["c", "char"],
	["sc", "signed char"],
	["uc", "unsigned char"],
	["s", "short"],
	["us", "unsigned short"],
	["i", "int"],
	["ui", "unsigned int"],
	["l", "long"],
	["ul", "unsigned long"],
	["ll", "long long"],
	["ull", "unsigned long long"]
] %}
{% for T in TYPES %}
static inline {{T.1}} _extint_get_{{T.0}}({{T.1}} a) { return a; }
{% endfor %}
#define extint_get(a) \
		_Generic((a) \
	{% for T in TYPES %}
		, {{T.1}}: _extint_get_{{T.0}} \
	{% endfor %}
	{% for I in range(1, 65) %}
		, extint_u{{I}}: _extint_get_u{{I}} \
		, extint_s{{I}}: _extint_get_s{{I}} \
	{% endfor %}
		_extint_get_func128(OP, a) \
		)(a)

/* ------------------------------------------------------------------------- */

#define _extint_get_func(OP, a) \
		_Generic((a) \
	{% for I in range(1, 65) %}
		, extint_u{{I}}: extint_##OP##_u{{I}}_v \
		, extint_s{{I}}: extint_##OP##_s{{I}}_v \
	{% endfor %}
		_extint_get_func128(OP, a) \
		)


{% for OP in ["add", "sub", "div", "mul"] %}
#define extint_{{OP}}(a, b)  _extint_get_func({{OP}}, a)(extint_get(a), extint_get(b))
{% endfor %}

/* ------------------------------------------------------------------------- */

#if 1
#define _extint_type_to_idx(a) \
		_Generic((a)\
	{% for I in range(1, 65) %}
		, extint_s{{I}}: {{I +  0}} \
		, extint_u{{I}}: {{I + 64}} \
	{% endfor %}
		_extint_type_to_idx128(a) \
		)

#define _extint_MAX(a, b)  (a > b ? a : b)

#define _extint_promotefunc(OP, a, b)  \
		_Generic((int(*)[_extint_MAX(_extint_type_to_idx(a), _extint_type_to_idx(b))])0 \
	{% for A in range(1, 65) %}
		, int(*)[{{A +  0}}]: extint_##OP##_s{{A}}_v \
		, int(*)[{{A + 64}}]: extint_##OP##_u{{A}}_v \
	{% endfor %}
		)

{% for OP in ["add", "sub", "div", "mul"] %}
#define extint_{{OP}}_p(a, b)  _extint_promotefunc({{OP}}, a, b)(extint_get(a), extint_get(b))
{% endfor %}
#endif

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
#endif /* EXTINT_H_ */
