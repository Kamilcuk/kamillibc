
#include <climits>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace kc {

template <typename TYPE> struct uintwrap {
	using type = TYPE;
	static constexpr type min = std::numeric_limits<type>::min();
	static_assert(min == 0, "");
	static constexpr type max = std::numeric_limits<type>::max();
	type value;
	constexpr type distance(uintwrap o) const {
		return o.value > value ? o.value - value : value - o.value;
	}
	constexpr operator type() const { return value; }
	constexpr uintwrap operator-(uintwrap o) const {
		return value > o.value ? (value - o.value) : (max - o.value + value);
	}
	constexpr uintwrap operator+(uintwrap o) const { return value + o.value; }
	constexpr uintwrap operator*(uintwrap o) const { return value * o.value; }
	constexpr uintwrap operator/(uintwrap o) const { return value / o.value; }
	constexpr uintwrap operator%(uintwrap o) const { return value % o.value; }
	uintwrap &operator-=(uintwrap o) { return *this = (*this - o); }
	uintwrap &operator+=(uintwrap o) { return *this = (*this + o); }
	uintwrap &operator*=(uintwrap o) { return *this = (*this * o); }
	uintwrap &operator/=(uintwrap o) { return *this = (*this / o); }
	uintwrap &operator%=(uintwrap o) { return *this = (*this % o); }
	constexpr bool operator==(uintwrap o) const { return o.value == value; }
	constexpr bool operator>(uintwrap o) const {
		return distance(o) < max / 2 ? o.value > value : value > o.value;
	}
	constexpr bool operator<(uintwrap o) const {
		return distance(o) < max / 2 ? o.value < value : value > o.value;
	}
	constexpr uintwrap operator<<(unsigned count) const { return value << count; }
	constexpr uintwrap operator>>(unsigned count) const { return value >> count; }
	uintwrap &operator<<=(unsigned count) {
		value <<= count;
		return *this;
	}
	uintwrap &operator>>=(unsigned count) {
		value >>= count;
		return *this;
	}

#define LAZYME(rettype, op) \
	constexpr rettype operator op(type o) const { return (*this op uintwrap(o)).value; }
#define LAZYMEM(op) \
	uintwrap &operator op(type o) { return (*this op uintwrap(o)); }
	LAZYME(type, -)
	LAZYME(type, +)
	LAZYME(type, *)
	LAZYME(type, /)
	LAZYME(type, %)
	LAZYME(bool, >)
	LAZYME(bool, <)
	LAZYME(bool, ==)
	LAZYMEM(-=)
	LAZYMEM(+=)
	LAZYMEM(*=)
	LAZYMEM(/=)
	LAZYMEM(%=)
	LAZYMEM(=)
#undef LAZYME
#undef LAZYMEM
};

} // namespace kc
