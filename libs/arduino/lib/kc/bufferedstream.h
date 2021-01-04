
#include "ringbuffer.h"
#include <Stream.h>
#include <cstdint>
namespace kc {

template <std::size_t SIZE> struct BufferedStream : public Stream {
	RingBuffer<uint8_t, SIZE> buf;

public:
	void clear() { buf.clear(); }
	virtual size_t write(uint8_t var) {
		return buf.push_back(var);
	}
	virtual int availableForWrite(void) { return buf.free_size(); }
	virtual int available() { return buf.size(); }
	virtual int read() {
		uint8_t v;
		if (!buf.pop_front(v))
			return -1;
		return v;
	}
	virtual int peek() {
		if (buf.empty()) {
			return -1;
		}
		return buf.front();
	}
	virtual void flush() { buf.clear(); }
};

} // namespace kc
