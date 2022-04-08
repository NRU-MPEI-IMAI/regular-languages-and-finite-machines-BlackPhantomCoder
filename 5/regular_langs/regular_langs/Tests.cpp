#include "Tests.h"


#include "test_runner.h"
#include "profile.h"
#include "Language.h"

using namespace std;

//тестирование произведения
void test_union() {
	{
		auto l1 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"1|2|2\n"
				"2|1|1\n"
				"\n"
				"1\n"
				"\n"
				"1\n"
			);
			in >> l1;
		}

		auto l2 = Language();
		{
			auto in = istringstream(
				"b\n"
				"1|1\n"
				"\n"
				"1\n"
				"\n"
				"1\n"
			);
			in >> l2;
		}
		auto u = make_union(l1, l2);
		ASSERT_EQUAL(u.get_delta().size(), 2);
		ASSERT_EQUAL(u.get_delta().at("<1,1>").at('b'), "<2,1>");
		ASSERT_EQUAL(u.get_delta().at("<2,1>").at('b'), "<1,1>");

		ASSERT_EQUAL(u.get_S(), "<1,1>");
		ASSERT_EQUAL(u.get_T().size(), 1);
		ASSERT_EQUAL(u.get_T().find("<1,1>") != end(u.get_T()), true);
	}

	{
		auto l1 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"s1|s2|s2\n"
				"s2|s1|s1\n"
				"\n"
				"s1\n"
				"\n"
				"s1\n"
			);
			in >> l1;
		}

		auto l2 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"s1|s2|s2\n"
				"s2|s1|s1\n"
				"\n"
				"s1\n"
				"\n"
				"s1\n"
			);
			in >> l2;
		}
		auto u = make_union(l1, l2);
		ASSERT_EQUAL(u.get_delta().size(), 4);
		ASSERT_EQUAL(u.get_delta().at("<s1,s1>").at('a'), "<s2,s2>");
		ASSERT_EQUAL(u.get_delta().at("<s1,s2>").at('a'), "<s2,s1>");
		ASSERT_EQUAL(u.get_delta().at("<s2,s1>").at('a'), "<s1,s2>");
		ASSERT_EQUAL(u.get_delta().at("<s2,s2>").at('a'), "<s1,s1>");

		ASSERT_EQUAL(u.get_delta().at("<s1,s1>").at('b'), "<s2,s2>");
		ASSERT_EQUAL(u.get_delta().at("<s1,s2>").at('b'), "<s2,s1>");
		ASSERT_EQUAL(u.get_delta().at("<s2,s1>").at('b'), "<s1,s2>");
		ASSERT_EQUAL(u.get_delta().at("<s2,s2>").at('b'), "<s1,s1>");

		ASSERT_EQUAL(u.get_S(), "<s1,s1>");
		ASSERT_EQUAL(u.get_T().size(), 1);
		ASSERT_EQUAL(u.get_T().find("<s1,s1>") != end(u.get_T()), true);
	}
}

//тестирование разности
void test_difference() {
	{
		auto l1 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"1|2|2\n"
				"2|1|1\n"
				"\n"
				"1\n"
				"\n"
				"1\n"
			);
			in >> l1;
		}

		auto l2 = Language();
		{
			auto in = istringstream(
				"b\n"
				"1|1\n"
				"\n"
				"1\n"
				"\n"
				"1\n"
			);
			in >> l2;
		}
		auto diff = make_difference(l1, l2);
		ASSERT_EQUAL(diff.get_delta().size(), 2);
		ASSERT_EQUAL(diff.get_delta().at("<1,1>").at('b'), "<2,1>");
		ASSERT_EQUAL(diff.get_delta().at("<2,1>").at('b'), "<1,1>");

		ASSERT_EQUAL(diff.get_S(), "<1,1>");
		ASSERT_EQUAL(diff.get_T().size(), 0);
	}

	{
		auto l1 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"s1|s2|s2\n"
				"s2|s1|s1\n"
				"\n"
				"s1\n"
				"\n"
				"s1\n"
			);
			in >> l1;
		}

		auto l2 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"s1|s2|s2\n"
				"s2|s1|s1\n"
				"\n"
				"s1\n"
				"\n"
				"s1\n"
			);
			in >> l2;
		}
		auto diff = make_difference(l1, l2);
		ASSERT_EQUAL(diff.get_delta().size(), 4);
		ASSERT_EQUAL(diff.get_delta().at("<s1,s1>").at('a'), "<s2,s2>");
		ASSERT_EQUAL(diff.get_delta().at("<s1,s2>").at('a'), "<s2,s1>");
		ASSERT_EQUAL(diff.get_delta().at("<s2,s1>").at('a'), "<s1,s2>");
		ASSERT_EQUAL(diff.get_delta().at("<s2,s2>").at('a'), "<s1,s1>");

		ASSERT_EQUAL(diff.get_delta().at("<s1,s1>").at('b'), "<s2,s2>");
		ASSERT_EQUAL(diff.get_delta().at("<s1,s2>").at('b'), "<s2,s1>");
		ASSERT_EQUAL(diff.get_delta().at("<s2,s1>").at('b'), "<s1,s2>");
		ASSERT_EQUAL(diff.get_delta().at("<s2,s2>").at('b'), "<s1,s1>");

		ASSERT_EQUAL(diff.get_S(), "<s1,s1>");
		ASSERT_EQUAL(diff.get_T().size(), 1);
		ASSERT_EQUAL(diff.get_T().find("<s1,s2>") != end(diff.get_T()), true);
	}
}

//тестирование объединения
void test_intersection() {
	{
		auto l1 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"1|2|2\n"
				"2|1|1\n"
				"\n"
				"1\n"
				"\n"
				"1\n"
			);
			in >> l1;
		}

		auto l2 = Language();
		{
			auto in = istringstream(
				"b\n"
				"1|1\n"
				"\n"
				"1\n"
				"\n"
				"1\n"
			);
			in >> l2;
		}
		auto u = make_union(l1, l2);
		auto intersection = make_intersection_from_union(l1, l2, u);
		ASSERT_EQUAL(intersection.get_delta().size(), 2);
		ASSERT_EQUAL(intersection.get_delta().at("<1,1>").at('b'), "<2,1>");
		ASSERT_EQUAL(intersection.get_delta().at("<2,1>").at('b'), "<1,1>");

		ASSERT_EQUAL(intersection.get_S(), "<1,1>");


		ASSERT_EQUAL(intersection.get_T().size(), 2);
		ASSERT_EQUAL(intersection.get_T().find("<1,1>") != end(intersection.get_T()), true);
		ASSERT_EQUAL(intersection.get_T().find("<2,1>") != end(intersection.get_T()), true);
	}

	{
		auto l1 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"s1|s2|s2\n"
				"s2|s1|s1\n"
				"\n"
				"s1\n"
				"\n"
				"s1\n"
			);
			in >> l1;
		}

		auto l2 = Language();
		{
			auto in = istringstream(
				"ab\n"
				"s1|s2|s2\n"
				"s2|s1|s1\n"
				"\n"
				"s1\n"
				"\n"
				"s1\n"
			);
			in >> l2;
		}
		auto u = make_union(l1, l2);
		auto intersection = make_intersection_from_union(l1, l2, u);
		ASSERT_EQUAL(intersection.get_delta().size(), 4);
		ASSERT_EQUAL(intersection.get_delta().at("<s1,s1>").at('a'), "<s2,s2>");
		ASSERT_EQUAL(intersection.get_delta().at("<s1,s2>").at('a'), "<s2,s1>");
		ASSERT_EQUAL(intersection.get_delta().at("<s2,s1>").at('a'), "<s1,s2>");
		ASSERT_EQUAL(intersection.get_delta().at("<s2,s2>").at('a'), "<s1,s1>");

		ASSERT_EQUAL(intersection.get_delta().at("<s1,s1>").at('b'), "<s2,s2>");
		ASSERT_EQUAL(intersection.get_delta().at("<s1,s2>").at('b'), "<s2,s1>");
		ASSERT_EQUAL(intersection.get_delta().at("<s2,s1>").at('b'), "<s1,s2>");
		ASSERT_EQUAL(intersection.get_delta().at("<s2,s2>").at('b'), "<s1,s1>");

		ASSERT_EQUAL(intersection.get_T().size(), 3);
		ASSERT_EQUAL(intersection.get_T().find("<s1,s1>") != end(intersection.get_T()), true);
		ASSERT_EQUAL(intersection.get_T().find("<s1,s2>") != end(intersection.get_T()), true);
		ASSERT_EQUAL(intersection.get_T().find("<s2,s1>") != end(intersection.get_T()), true);
	}
}

void run_tests()
{
	TestRunner runner;
	RUN_TEST(runner, test_union);
	RUN_TEST(runner, test_difference);
	RUN_TEST(runner, test_intersection);
}
