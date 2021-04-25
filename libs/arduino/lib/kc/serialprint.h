#pragma once

#include "invoke.hpp/invoke.hpp"
#include "type_traits.h"
#include <Arduino.h>
#include <string>
#include <vector>
#include <string>

namespace kc {

using Print = ::Print;
using Stream = ::Stream;

static inline std::string vpformat(const char *format, va_list va) {
	std::string buffer;
	buffer.resize(64);
	va_list va2;
	va_copy(va2, va);
	size_t len = vsnprintf(&buffer[0], buffer.size(), format, va);
	if (len > buffer.size() - 1) {
		buffer.resize(len + 1);
		vsnprintf(&buffer[0], buffer.size(), format, va2);
	}
	va_end(va2);
	return buffer;
}

static inline std::string pformat(const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	const auto ret = vpformat(format, arg);
	va_end(arg);
	return ret;
}

struct SerialPrint {
	Print &out;

public:
	SerialPrint(Print &out) : out(out) {}

	// What?
	int getWriteError() { return out.getWriteError(); }
	void clearWriteError() { return out.clearWriteError(); }
	size_t write(uint8_t t) { return out.write(t); }
	size_t write(const char *str) { return out.write(str); }
	size_t write(const uint8_t *buffer, size_t size) { return out.write(buffer, size); }
	size_t write(short t) { return out.write(t); }
	size_t write(unsigned short t) { return out.write(t); }
	size_t write(int t) { return out.write(t); }
	size_t write(unsigned int t) { return out.write(t); }
	size_t write(long t) { return out.write(t); }
	size_t write(unsigned long t) { return out.write(t); }
	size_t write(char c) { return out.write(c); }
	size_t write(int8_t c) { return out.write(c); }
	template <class... TS> size_t printf(const char *format, TS &&...ts) {
		return out.printf(format, std::forward<TS>(ts)...);
	}
	template <class... TS> size_t printf_P(PGM_P format, TS &&...ts) {
		return out.printf_P(format, std::forward<TS>(ts)...);
	}
	size_t print(const __FlashStringHelper *t) { return out.print(t); }
	size_t print(const String &t) { return out.print(t); }
	size_t print(const char *t) { return out.print(t); }
	size_t print(char t) { return out.print(t); }
	size_t print(unsigned char t, int d = DEC) { return out.print(t, d); }
	size_t print(int t, int d = DEC) { return out.print(t, d); }
	size_t print(unsigned int t, int d = DEC) { return out.print(t, d); }
	size_t print(long t, int d = DEC) { return out.print(t, d); }
	size_t print(unsigned long t, int d = DEC) { return out.print(t, d); }
	size_t print(double t, int d = 2) { return out.print(t, d); }
	size_t print(const Printable &t) { return out.print(t); }
	size_t println(const __FlashStringHelper *t) { return out.println(t); }
	size_t println(const String &s) { return out.println(s); }
	size_t println(const char *t) { return out.println(t); }
	size_t println(char t) { return out.println(t); }
	size_t println(unsigned char t, int d = DEC) { return out.println(t, d); }
	size_t println(int t, int d = DEC) { return out.println(t, d); }
	size_t println(unsigned int t, int d = DEC) { return out.println(t, d); }
	size_t println(long t, int d = DEC) { return out.println(t, d); }
	size_t println(unsigned long t, int d = DEC) { return out.println(t, d); }
	size_t println(double t, int d = 2) { return out.println(t, d); }
	size_t println(const Printable &t) { return out.println(t); }
	size_t println(void) { return out.println(); }
	void flush() { return out.flush(); }

#define LAZYME(TYPE) \
	SerialPrint &operator<<(TYPE arg) { \
		print(arg); \
		return *this; \
	}
	LAZYME(const __FlashStringHelper *)
	LAZYME(const String &)
	LAZYME(const char *)
	LAZYME(char)
	LAZYME(int)
	LAZYME(unsigned int)
	LAZYME(long)
	LAZYME(unsigned long)
	LAZYME(float)
	LAZYME(double)
	LAZYME(const Printable &)
#undef LAZYME

#define LAZYME_BASE(TYPE) \
	SerialPrint &operator<<(const std::pair<TYPE, unsigned> &arg) { \
		print(arg.first, arg.second); \
		return *this; \
	}
	LAZYME_BASE(unsigned char)
	LAZYME_BASE(int)
	LAZYME_BASE(unsigned int)
	LAZYME_BASE(long)
	LAZYME_BASE(unsigned long)
	LAZYME_BASE(double)
#undef LAZYME_BASE

	SerialPrint &operator<<(const std::string &a) {
		write((const uint8_t *)a.data(), a.size());
		return *this;
	}

	template <typename... Args>
	SerialPrint &operator<<(const std::tuple<const char *, Args...> &t) {
		invoke_hpp::apply(::printf, t);
		return *this;
	}

	template <typename CONT, typename SEP>
	typename std::enable_if<kc::is_container<CONT>::value, SerialPrint &>::type
	operator<<(const std::pair<CONT, SEP> &a) {
		for (auto &i : a.first) {
			this->operator<<(i);
			this->operator<<(a.second);
		}
		return *this;
	}

	template <typename CONT>
	typename std::enable_if<kc::is_container<CONT>::value, SerialPrint &>::type
	operator<<(const CONT &a) {
		return this->operator<<(std::make_pair(a, ' '));
	}
};

} // namespace kc
