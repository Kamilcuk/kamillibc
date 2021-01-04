
#include <cstddef>

namespace kc {

template<typename TYPE, std::size_t SIZE> class aligned_storage {
	alignas(TYPE) unsigned char buf[SIZE][sizeof(TYPE)];

	static constexpr TYPE *cast(unsigned char (*p)[sizeof(TYPE)]) {
		return reinterpret_cast<TYPE *>(*p);
	}
	static constexpr const TYPE *
	cast(const unsigned char (*p)[sizeof(TYPE)]) {
		return reinterpret_cast<const TYPE *>(*p);
	}

	constexpr aligned_storage() = default;
	~aligned_storage() = default;
	constexpr aligned_storage(const aligned_storage&) = default;
	aligned_storage &operator=(const aligned_storage&) = default;
	constexpr aligned_storage(aligned_storage&&) = default;
	aligned_storage &operator=(aligned_storage&&) = default;
	

	TYPE *begin() { return cast(&buf[0]); }
	constexpr const TYPE *begin() const { return cast(&buf[0]); }
	TYPE *end() { return cast(&buf[SIZE]); }
	constexpr const TYPE *end() const { return cast(&buf[SIZE]); }

	unsigned char (&operator[](std::size_t idx))[sizeof(TYPE)] {
		return buf[idx];
	}
	constexpr const unsigned char (&operator[](std::size_t idx) const)[sizeof(TYPE)] {
		return buf[idx];
	}
	TYPE &at(std::size_t idx) {
		return *cast(buf[idx]);
	}
	constexpr const TYPE &at(std::size_t idx) const {
		return *cast(buf[idx]);
	}
};

}

