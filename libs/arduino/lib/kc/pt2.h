#pragma once

#include "sbuffer.h"
#include <functional>
#include <pt.h>

namespace kc {

#ifdef ARDUINO
#include <Arduino.h>
static inline unsigned long pt_millis() { return millis(); }
#endif

#define PT_DELAY(pt, var, time)                                                \
	do {                                                                   \
		*(var) = pt_millis();                                          \
		PT_WAIT_UNTIL(pt, pt_millis() - *(var) > (time));              \
	} while (0)

#define PT_DELAY_MS PT_DELAY

class Pt {
	struct pt _pt;

public:
	Pt() { PT_INIT(pt()); }

	struct pt *pt() {
		return &_pt;
	}

	const struct pt *pt() const { return &_pt; }
	void thread_restart() { PT_INIT(pt()); }

	virtual int loop() = 0;
};

struct PtTimed : Pt {
	unsigned long now;
};

class PtPeriodic {
	unsigned long now;
	unsigned int _period_ms : 15;
	bool _enabled : 1;

public:
	PtPeriodic(unsigned int period_ms = 1000, bool enabled = 1)
	    : _period_ms(period_ms), _enabled(enabled) {}

	void set_period_ms(unsigned period_ms) { _period_ms = period_ms; }
	unsigned get_period_ms() const { return _period_ms; }

	void set_enabled(bool enabled) { _enabled = enabled; }
	bool is_enabled() const { return _enabled; }

	virtual void periodic() = 0;

	int loop() {
		if (!is_enabled()) {
			return PT_ENDED;
		}
		unsigned long next;
		if ((next = pt_millis()) - now > get_period_ms()) {
			now = next;
			periodic();
		}
		return PT_WAITING;
	}

};

template <std::size_t SIZE> class PtManager {
	SBuffer<std::function<int()>, SIZE> loops;

public:
	void manage(std::function<int()> f) { loops.push_back(f); }
	void loop() {
		for (auto &i : loops) {
			i();
		}
	}
};

} // namespace kc
