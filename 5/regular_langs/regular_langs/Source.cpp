#include <iostream>
#include <fstream>
#include <optional>
#include <unordered_set>

#include "Language.h"
#include "Tests.h"

using namespace std;

//создаёт svg по языку
void create_svg(const string& name, const Language& l);
//создаёт выходной md файл
void make_md(const string& name, bool diff, bool intersection);
//ввод языка из файла
Language input_lang(const string& filename);

int main(int argc, char* argv[] ) {
	setlocale(LC_ALL, "Rus");

	if (argc < 4) {
		cout << "недостаточно аргументов" << endl;
		return 0;
	}

	//обработка фалагов
	auto f_diff = false;
	auto f_ins = false;

	{
		auto flags = unordered_set<string_view>();
		if (argc > 4) {
			flags.insert(argv[4]);
		}
		if (argc > 5) {
			flags.insert(argv[5]);
		}
		f_diff = flags.find("-d") != end(flags);
		f_ins = flags.find("-i") != end(flags);
	}


	//тестирование только на отладке
	#ifdef _DEBUG
		run_tests();
	#endif // !1

	try {
		auto l1 = input_lang(argv[1]);
		auto l2 = input_lang(argv[2]);


		auto u = make_union(l1, l2);
		auto diff = make_difference(l1, l2);
		auto intersection = make_intersection_from_union(l1, l2, u);

		create_svg("1", l1);
		create_svg("2", l2);

		create_svg("union", u);

		if(f_diff)
			create_svg("difference", diff);
		if (f_ins)
			create_svg("intersection", intersection);

		make_md(argv[3], f_diff, f_ins);
	}
	catch (exception& e) {
		cout << "error" << '\n';
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}



void create_svg(const string& name, const Language& l) {
	{
		auto os = ofstream("output_data/" + name + ".gv");
		os << l;
	}
	system(("dot\\dot.exe -Tsvg output_data/" + name + ".gv -o output_data/" + name + ".svg").c_str());
}

void make_md(const string& name, bool diff, bool intersection) {
	auto os = ofstream(name);

	os << "# L 1" << '\n';
	os << "![L 1](./output_data/1.svg) " << '\n';

	os << "# L 2" << '\n';
	os << "![L 2](./output_data/2.svg) " << '\n';

	os << "# Union" << '\n';
	os << "![union](./output_data/union.svg) " << '\n';

	if (diff) {
		os << "# Difference" << '\n';
		os << "![difference](./output_data/difference.svg) " << '\n';
	}
	if (intersection) {
		os << "# Intersection" << '\n';
		os << "![intersection](./output_data/intersection.svg) " << '\n';
	}
}

Language input_lang(const string& filename)
{
	auto result = Language();
	{
		auto is = ifstream(filename);
		if (!is)
			throw invalid_argument("файл не найден");
		is >> result;
	}
	return result;
}
