#pragma once

#include "type_traits.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <type_traits>
#ifdef __cpp_exceptions
#include <stdexcept>
#endif

namespace kc {

template <typename TYPE, std::size_t SIZE> class SBuffer {
	alignas(TYPE) unsigned char buf[SIZE][sizeof(TYPE)];
	unsigned char (*_i)[sizeof(TYPE)] = &buf[0];

	static constexpr TYPE *cast(unsigned char (*p)[sizeof(TYPE)]) {
		return reinterpret_cast<TYPE *>(*p);
	}
	static constexpr const TYPE *
	cast(const unsigned char (*p)[sizeof(TYPE)]) {
		return reinterpret_cast<const TYPE *>(*p);
	}

	TYPE *i() { return cast(_i); }
	const TYPE *i() const { return cast(_i); }
	void inc_i() { ++_i; }
	void dec_i() { --_i; }

public:
	using value_type = TYPE;

	constexpr SBuffer() {}

	~SBuffer() { clear(); }

	SBuffer(const SBuffer &o) { *this = o; }

private:
	template <typename U> void impl_assign(U &&o) {
		clear();
		for (auto &i : o) {
			if (push_back(kc::forward_like<U>(i)) != true) {
				assert(0);
			}
		}
	}

public:
	SBuffer &operator=(const SBuffer &o) {
		impl_assign(o);
		return *this;
	}

	SBuffer(SBuffer &&o) { *this = o; }

	SBuffer &operator=(SBuffer &&o) {
		impl_assign(o);
		o.clear();
		return *this;
	}

	constexpr const TYPE *begin() const { return cast(&buf[0]); }
	TYPE *begin() { return cast(&buf[0]); }
	constexpr const TYPE *end() const { return i(); }
	TYPE *end() { return i(); }
	constexpr const TYPE *reserved_end() const { return cast(&buf[SIZE]); }
	TYPE *reserved_end() { return cast(&buf[SIZE]); }

	constexpr const TYPE &front() const noexcept { return *begin(); }
	TYPE &front() noexcept {
		assert(size() > 0);
		return *begin();
	}

	constexpr const TYPE &back() const noexcept { return *(i() - 1); }
	TYPE &back() noexcept { return *(i() - 1); }

	constexpr size_t size() const noexcept { return i() - begin(); }
	constexpr bool full() const { return i() == reserved_end(); }
	constexpr bool empty() const { return i() == begin(); }

	constexpr size_t free_size() const noexcept {
		return reserved_end() - i();
	}

	const TYPE &operator[](std::size_t idx) const noexcept {
		assert(!empty());
		return *(begin() + idx);
	}
	TYPE &operator[](std::size_t idx) noexcept {
		return as_mutable(as_const(*this)[idx]);
	}

	const TYPE &at(std::size_t idx) const {
#ifdef __cpp_exceptions
		if (idx >= size()) {
			throw std::out_of_range("SBuffer::at() index");
		}
#else
		assert(idx < size());
#endif
		return this->operator[](idx);
	}
	TYPE &at(std::size_t idx) {
		return as_mutable(as_const(*this).at(idx));
	}

	void clear() noexcept {
		for (auto &i : *this) {
			i.~TYPE();
		}
		_i = &buf[0];
	}

private:
	template <typename U> bool impl_push_back(U &&v) {
		if (full()) {
			return false;
		}
		new (i()) TYPE();
		*i() = std::forward<U>(v);
		inc_i();
		return true;
	}

public:
	bool push_back(const TYPE &v) { return impl_push_back(v); }
	bool push_back(TYPE &&v) { return impl_push_back(v); }

private:
	template <typename U> bool impl_push_front(U &&v) {
		if (full()) {
			return false;
		}
		if (!empty()) {
			std::copy_backward(begin(), end(), begin() + 1);
		} else {
			new (begin()) TYPE();
		}
		front() = std::forward<U>(v);
		return true;
	}

public:
	bool push_front(const TYPE &v) { return impl_push_front(v); }
	bool push_front(TYPE &&v) { return impl_push_front(v); }

	void pop_back() {
		assert(!empty());
		back().~TYPE();
		dec_i();
	}

	bool pop_back(TYPE &v) {
		if (empty()) {
			return false;
		}
		v = std::move(back());
		pop_back();
		return true;
	}

	void pop_front() {
		assert(!empty());
		std::copy(begin() + 1, end(), begin());
		pop_back();
	}

	bool pop_front(TYPE &v) {
		if (empty()) {
			return false;
		}
		v = std::move(front());
		pop_front();
		return true;
	}
};

}; // namespace kc

#ifdef KC_TEST
#include "test.h"
int main() { test_container<SBuffer>(); }
#endif
