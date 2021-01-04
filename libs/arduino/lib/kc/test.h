#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

#define TEST(expr, ...)                                                        \
	do {                                                                   \
		if (!(expr)) {                                                 \
			fprintf(stderr, "ERR:%d: %s -- ", __LINE__, #expr);    \
			fprintf(stderr, ##__VA_ARGS__, "");                    \
			fprintf(stderr, "\n");                                 \
			abort();                                               \
		}                                                              \
	} while (0)

namespace kc {

template <template <class, std::size_t> class V, std::size_t SIZE>
struct tester {
	std::vector<int> temp;
	V<std::vector<int>, SIZE> sb;
	const V<std::vector<int>, SIZE> &sbc = sb;
	std::vector<std::vector<int>> vv;

	void test() {
		srand(0);
		TEST(sb.size() == 0);
		TEST(sb.free_size() == SIZE);
		TEST(sbc.begin() == sb.begin());
		TEST(sbc.end() == sb.end());
		TEST(sbc.free_size() == sb.free_size());
		TEST(sb.pop_back(temp) == false);

		fill_vv_random(vv);
		container_assign();
		container_assign_pop_front();
		test_3();
		test_destructor();
		test_vaious();
		for (size_t i = 0; i < 10; ++i) {
			test_move();
		}
		test_full();
	}

	static void fill_vv_random(std::vector<std::vector<int>> &vv,
				   unsigned at_least = 1) {
		for (size_t i = 0, max = std::min(at_least + (rand() % (SIZE - 1)), SIZE);
		     i < max; ++i) {
			std::vector<int> temp;
			for (size_t i = 0, max = 1 + rand() % SIZE; i < max;
			     ++i) {
				temp.push_back(rand() % 100);
			}
			vv.push_back(temp);
		}
	}

	void container_assign() {
		for (size_t i = 0; i < vv.size(); ++i) {
			TEST(sb.push_back(vv[i]) == true);
			TEST(sb.back() == vv[i], "i=%d sb.size()=%d", (int)i,
			     (int)sb.size());
			TEST(sb.size() == i + 1);
			TEST(sb.free_size() == SIZE - i - 1);
		}
		for (size_t i = vv.size(); i; --i) {
			TEST(sb.pop_back(temp) == true);
			TEST(temp == vv.at(i - 1));
			TEST(sb.size() == i - 1, "sb.size()=%d i=%d",
			     (int)sb.size(), (int)i);
			TEST(sb.free_size() == SIZE - i + 1);
		}
	}

	void container_assign_pop_front() {
		for (size_t i = 0; i < vv.size(); ++i) {
			TEST(sb.push_back(vv.at(i)) == true);
			TEST(sb.back() == vv.at(i));
			TEST(sb.size() == i + 1);
			TEST(sb.free_size() == SIZE - i - 1);
		}
		for (size_t i = vv.size(); i; --i) {
			TEST(sb.pop_front(temp) == true);
			TEST(temp == vv.at(vv.size() - i));
			TEST(sb.size() == i - 1, "%d %d", (int)sb.size(),
			     (int)i);
			TEST(sb.free_size() == SIZE - i + 1);
		}
	}

	void test_3() {
		for (size_t i = 0; i < vv.size(); ++i) {
			TEST(sb.push_back(vv[i]) == true);
			TEST(sb.size() == i + 1);
			TEST(sb.free_size() == SIZE - i - 1);
		}

		unsigned idx = 0;
		idx = 0;
		for (auto &i : sb) {
			TEST(vv[idx] == i);
			idx++;
		}
		idx = 0;
		for (auto &i : sbc) {
			TEST(vv[idx] == i);
			idx++;
		}
		for (size_t i = 0; i < sb.size(); ++i) {
			TEST(vv[i] == sb[i]);
			TEST(vv[i] == sbc[i]);
			TEST(vv[i] == sb.at(i));
			TEST(vv[i] == sbc.at(i));
		}
#ifdef __cpp_exceptions
		TEST([&]() -> bool {
			try {
				sb.at(1000);
			} catch (const std::out_of_range &e) {
				return true;
			}
			return false;
		}());
#endif
		sb.clear();
	}

	static void test_destructor() {
		V<std::vector<int>, SIZE> sb;
		std::vector<std::vector<int>> vvec;
		fill_vv_random(vvec, 3);
		TEST(sb.push_back(vvec.at(0)) == true);
		TEST(sb.push_back(vvec.at(1)) == true);
		TEST(sb.push_back(vvec.at(2)) == true);
		std::vector<int> temp;
		TEST(sb.pop_back(temp) == true);
	}

	static void test_vaious() {
		V<int, SIZE> rb;
		for (unsigned i = 0; i < 200; ++i) {
			TEST(rb.size() == 0, "%d %d", (int)rb.size(),
			     (int)SIZE);
			TEST(rb.free_size() == SIZE, "%d %d",
			     (int)rb.free_size(), (int)SIZE);

			unsigned count = 0;
			std::vector<int> vals;
			for (; rand() % SIZE != 0; count++) {
				const int val = rand() % 100;
				if (!rb.push_back(val))
					break;
				vals.push_back(val);
			}
			TEST(rb.size() == count, "%d %d", count,
			     (int)rb.size());
			TEST(rb.free_size() == SIZE - count);

			for (unsigned i = 0; i < count; ++i) {
				int val;
				TEST(rb.pop_front(val) == true, "%d %d %u",
				     (int)rb.size(), (int)rb.free_size(),
				     count);
				TEST(vals[i] == val);
			}
			TEST(rb.size() == 0);
			TEST(rb.free_size() == SIZE);
		}
	};

	static void test_move() {
		std::vector<std::vector<int>> vvec;
		V<std::vector<int>, SIZE> trb, trb2;
		fill_vv_random(vvec, 3);

		TEST(trb.push_back(std::vector<int>{1, 2}));
		trb.pop_back();
		TEST(trb.push_back(std::vector<int>{1, 2}));
		TEST(trb.push_back(std::vector<int>{1, 2}));
		trb.pop_back();
		trb.pop_back();
		TEST(trb.size() == 0);

		for (auto &i : vvec) {
			TEST(trb.push_back(i) == true);
		}
		TEST(trb.size() == vvec.size(), "%d %d", (int)trb.size(),
		     (int)vvec.size());
		trb2 = trb;
		TEST(trb2.size() == vvec.size(), "%d %d", (int)trb2.size(),
		     (int)vvec.size());
		for (size_t i = 0; i < vvec.size(); ++i) {
			TEST(trb[i] == vvec[i]);
			TEST(trb2[i] == vvec[i]);
		}
		trb.clear();
		TEST(trb.size() == 0);
		trb = std::move(trb2);
		TEST(trb2.size() == 0);
		TEST(trb.size() == vvec.size());
		for (size_t i = 0; i < vvec.size(); ++i) {
			TEST(trb[i] == vvec[i]);
		}
	}

	static void test_full() {
		std::vector<std::vector<int>> vvec;
		V<std::vector<int>, SIZE> trb;
		fill_vv_random(vvec, SIZE);
		TEST(vvec.size() == SIZE);

		TEST(trb.push_back(std::vector<int>{1, 2}));
		trb.pop_back();
		TEST(trb.push_back(std::vector<int>{1, 2}));
		TEST(trb.push_back(std::vector<int>{1, 2}));
		trb.pop_back();
		trb.pop_back();

		for (size_t i = 0; i < 3; ++i) {
			TEST(trb.size() == 0);
			TEST(trb.full() == false);
			TEST(trb.empty() == true);
			for (auto&& j : vvec) {
				TEST(trb.push_back(j) == true);
				TEST(trb.empty() == false);
			}
			TEST(trb.full() == true);
			TEST(trb.size() == SIZE);
			for (auto&& j : vvec) {
				(void)j;
				trb.pop_back();
				TEST(trb.full() == false);
			}
		}
	}
};

template <template <class, std::size_t> class V> void test_container() {
	tester<V, 20>().test();
	tester<V, 10>().test();
}

} // namespace kc

using namespace kc;
