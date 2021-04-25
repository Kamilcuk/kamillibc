#pragma once

#include "make_array.h"
#include "aligned_storage.h"
#include "ringbuffer.h"
#include "sbuffer.h"
#include "pt2.h"
#include "serialprint.h"
#include "intwrap.h"
#ifdef ARDUINO
#include "bufferedstream.h"

kc::SerialPrint sout();

#ifndef PWMRANGE
#define PWMRANGE 1023
#endif

#endif

#ifndef KC_NOT_USING
using namespace kc;
#endif

