#pragma once

#include "type_traits.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <utility>
#ifdef __cpp_exceptions
#include <stdexcept>
#endif

namespace kc {

template <typename TYPE, std::size_t SIZE> class RingBuffer {
private:
	alignas(TYPE) unsigned char buf[SIZE][sizeof(TYPE)];
	TYPE *head = bufbegin();
	TYPE *tail = bufbegin();
	bool _full = false;

protected:
	static constexpr TYPE *cast(unsigned char (*p)[sizeof(TYPE)]) {
		return reinterpret_cast<TYPE *>(*p);
	}
	static constexpr const TYPE *
	cast(const unsigned char (*p)[sizeof(TYPE)]) {
		return reinterpret_cast<const TYPE *>(*p);
	}

	TYPE *bufbegin() noexcept { return cast(&buf[0]); }
	constexpr const TYPE *bufbegin() const noexcept {
		return cast(&buf[0]);
	}
	TYPE *bufend() noexcept { return cast(&buf[SIZE]); }
	constexpr const TYPE *bufend() const noexcept {
		return cast(&buf[SIZE]);
	}
	size_t bufsize() const noexcept { return SIZE; }

	constexpr const TYPE *pntinc(const TYPE *p) const noexcept {
		return ++p == bufend() ? bufbegin() : p;
	}
	TYPE *pntinc(TYPE *p) noexcept {
		return as_mutable(as_const(*this).pntinc(p));
	}
	constexpr const TYPE *pntdec(const TYPE *p) const noexcept {
		return p == bufbegin() ? cast(&buf[SIZE - 1]) : (p - 1);
	}
	TYPE *pntdec(TYPE *p) noexcept {
		return as_mutable(as_const(*this).pntdec(p));
	}

public:
	template <typename PTYPE, typename CTYPE> class impl_iterator {
		friend PTYPE;
		PTYPE &p;
		CTYPE *pnt;
		bool at_end;

	protected:
		impl_iterator(PTYPE &p, CTYPE *pnt, bool at_end = 0)
		    : p(p), pnt(pnt), at_end(at_end) {}

	public:
		impl_iterator &operator++() {
			pnt = p.pntinc(pnt);
			if (pnt == p.tail) {
				at_end = true;
			}
			return *this;
		}
		impl_iterator &operator--() {
			at_end = false;
			pnt = p.pntdec(pnt);
			return *this;
		}
		template <typename PTYPE2, typename TYPE2>
		typename std::enable_if<
		    std::is_same<const PTYPE2, PTYPE>::value &&
			std::is_same<const TYPE2, CTYPE>::value,
		    bool>::type
		operator==(const impl_iterator<PTYPE2, TYPE2> &o) const {
			assert(&p == &o.p);
			return o.pnt == pnt && o.at_end == at_end;
		}
		bool operator==(const impl_iterator<PTYPE, CTYPE> &o) const {
			assert(&p == &o.p);
			return o.pnt == pnt && o.at_end == at_end;
		}
		bool operator!=(const impl_iterator<PTYPE, CTYPE> &o) const {
			return !(*this == o);
		}
		CTYPE &operator*() { return *pnt; }
		constexpr const CTYPE &operator*() const { return *pnt; }
	};

	using reference = TYPE &;
	using const_reference = const TYPE &;
	using iterator = impl_iterator<RingBuffer<TYPE, SIZE>, TYPE>;
	using const_iterator =
	    impl_iterator<const RingBuffer<TYPE, SIZE>, const TYPE>;
	friend iterator;

	constexpr RingBuffer() {}

	~RingBuffer() { clear(); }

	RingBuffer(const RingBuffer &o) { *this = o; }

private:
	template <typename U> void impl_assign(U &&o) {
		clear();
		for (auto &i : o) {
			push_back(kc::forward_like<U>(i));
		}
	}

public:
	RingBuffer &operator=(const RingBuffer &o) {
		impl_assign(o);
		return *this;
	}

	RingBuffer(RingBuffer &&o) { *this = o; }

	RingBuffer &operator=(RingBuffer &&o) {
		impl_assign(o);
		o.clear();
		return *this;
	}

	TYPE &front() noexcept {
		assert(!empty());
		return *head;
	}
	const TYPE &front() const noexcept { return *head; }
	TYPE &back() noexcept {
		assert(!empty());
		return *pntdec(tail);
	}
	const TYPE &back() const noexcept { return *pntdec(tail); }

	const_iterator begin() const noexcept {
		return const_iterator(*this, head);
	}
	iterator begin() noexcept { return iterator(*this, head); }
	const_iterator end() const noexcept {
		return const_iterator(*this, tail, 1);
	}
	iterator end() noexcept { return iterator(*this, tail, 1); }

	constexpr bool empty() const noexcept { return head == tail && !_full; }

	constexpr bool full() const noexcept { return _full; }

	size_t free_size() const {
		return full()	      ? 0
		       : tail >= head ? bufsize() - (tail - head)
				      : head - tail;
	}

	constexpr size_t size() const {
		return full()	      ? SIZE
		       : tail >= head ? tail - head
				      : bufsize() - (head - tail);
	}

	void clear() {
		while (!empty()) {
			pop_front();
		}
		head = tail = bufbegin();
	}

private:
	template <typename U> bool impl_push_back(U &&val) {
		TYPE *tmp = pntinc(tail);
		if (full()) {
			return false;
		}
		new (tail) TYPE();
		*tail = std::forward<U>(val);
		tail = tmp;
		if (tail == head) {
			_full = true;
		}
		return true;
	}

public:
	bool push_back(const TYPE &val) { return impl_push_back(val); }
	bool push_back(TYPE &&val) { return impl_push_back(val); }

private:
	template <typename U> bool impl_push_front(U &&val) {
		if (full()) {
			return false;
		}
		head = pntdec(head);
		new (head) TYPE();
		*head = std::forward<U>(val);
		if (head == tail) {
			_full = true;
		}
		return true;
	}

public:
	bool push_front(const TYPE &val) { return impl_push_front(val); }
	bool push_front(TYPE &&val) { return impl_push_front(val); }

	void pop_back() {
		back().~TYPE();
		tail = pntdec(tail);
		_full = false;
	}

	bool pop_back(TYPE &val) {
		if (empty()) {
			return false;
		}
		val = std::move(back());
		pop_back();
		return true;
	}

	void pop_front() {
		head->~TYPE();
		head = pntinc(head);
		_full = false;
	}

	bool pop_front(TYPE &val) {
		if (empty()) {
			return false;
		}
		val = std::move(front());
		pop_front();
		return true;
	}

	const TYPE &operator[](std::size_t idx) const noexcept {
		assert(!empty());
		const TYPE *it = head;
		while (idx--) {
			it = pntinc(it);
			assert(it != tail);
		}
		return *it;
	}
	TYPE &operator[](std::size_t idx) noexcept {
		return as_mutable(as_const(*this)[idx]);
	}

	const TYPE &at(std::size_t idx) const {
#ifdef __cpp_exceptions
		if (empty()) {
			throw std::out_of_range("RingBuffer out of range");
		}
#else
		assert(!empty());
#endif
		const TYPE *it = head;
		while (idx--) {
			it = pntinc(it);
#ifdef __cpp_exceptions
			if (it == tail) {
				throw std::out_of_range("RingBuffer out of range");
			}
#else
			assert(it != tail);
#endif
		}
		return *it;
	}
	TYPE &at(std::size_t idx) {
		return as_mutable(as_const(*this).at(idx));
	}

};

}; // namespace kc

#ifdef KC_TEST
#include "test.h"
using namespace kc;
int main() { test_container<RingBuffer>(); }
#endif
