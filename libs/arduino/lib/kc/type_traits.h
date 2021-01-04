#pragma once

#include <algorithm>
#include <type_traits>

namespace kc {

// https://stackoverflow.com/a/47369227/9072753
template <typename T> constexpr T &as_mutable(T const &value) noexcept {
	return const_cast<T &>(value);
}
template <typename T> constexpr T *as_mutable(T const *value) noexcept {
	return const_cast<T *>(value);
}
template <typename T> constexpr T *as_mutable(T *value) noexcept {
	return value;
}
template <typename T> void as_mutable(T const &&) = delete;

// https://en.cppreference.com/w/cpp/types/add_cv
template <class T> struct add_cv { typedef const volatile T type; };
template <class T> struct add_const { typedef const T type; };
template <class T> struct add_volatile { typedef volatile T type; };

template <class T> using add_cv_t = typename add_cv<T>::type;

template <class T> using add_const_t = typename add_const<T>::type;

template <class T> using add_volatile_t = typename add_volatile<T>::type;

// https://en.cppreference.com/w/cpp/utility/as_const
template <class T> constexpr add_const_t<T> &as_const(T &t) noexcept {
	return t;
}

// https://en.cppreference.com/w/cpp/algorithm/rotate
template <class ForwardIt>
ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last) {
	if (first == n_first)
		return last;
	if (n_first == last)
		return first;

	ForwardIt read = n_first;
	ForwardIt write = first;
	ForwardIt next_read =
	    first; // read position for when "read" hits "last"

	while (read != last) {
		if (write == next_read)
			next_read = read; // track where "first" went
		std::iter_swap(write++, read++);
	}

	// rotate the remaining sequence into place
	(rotate)(write, next_read, last);
	return write;
}

// https://stackoverflow.com/questions/29779825/perfect-forwarding-container-element
namespace detail {
    template<class T, class U>
    using forwarded_type = typename std::conditional<std::is_lvalue_reference<T>::value,
                                              typename std::remove_reference<U>::type&,
                                              typename std::remove_reference<U>::type&&>::type;
}
template<class T, class U>
detail::forwarded_type<T,U> forward_like(U&& u) {
    return std::forward<detail::forwarded_type<T,U>>(std::forward<U>(u));
}

} // namespace kc
