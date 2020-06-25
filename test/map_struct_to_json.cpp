#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "struct_mapping/struct_mapping.h"

namespace {

struct Simple {
	bool a;
	int b;
	double c;
	std::string d;
	bool e;
	int f;
	double g;
	std::string h;
};

TEST(struct_mapping_mapper_map_struct_to_json, test_simple) {
	Simple source;

	struct_mapping::reg(&Simple::a, "a");
	struct_mapping::reg(&Simple::b, "b");
	struct_mapping::reg(&Simple::c, "c");
	struct_mapping::reg(&Simple::d, "d");
	struct_mapping::reg(&Simple::e, "e");
	struct_mapping::reg(&Simple::f, "f");
	struct_mapping::reg(&Simple::g, "g");
	struct_mapping::reg(&Simple::h, "h");

	source.a = false;
	source.b = 1975;
	source.c = 14.768;
	source.d = std::string("Apollo");
	source.e = true;
	source.f = 1975;
	source.g = 6.790;
	source.h = std::string("Soyuz");

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json);

	std::string expected_json(R"json({"a":false,"b":1975,"c":14.768,"d":"Apollo","e":true,"f":1975,"g":6.79,"h":"Soyuz"})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

struct Struct_A {
	int value;
};

struct Struct {
	Struct_A a;
	Struct_A b;
};

TEST(struct_mapping_mapper_map_struct_to_json, test_struct) {
	Struct source;
	
	struct_mapping::reg(&Struct_A::value, "value");
	struct_mapping::reg(&Struct::a, "a");
	struct_mapping::reg(&Struct::b, "b");
	
	source.a.value = 42;
	source.b.value = 2020;

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json);

	std::string expected_json(R"json({"a":{"value":42},"b":{"value":2020}})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

TEST(struct_mapping_mapper_map_struct_to_json_pretty, test_struct) {
	Struct source;

	source.a.value = 42;
	source.b.value = 2020;

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json, "++");

	std::string expected_json(
R"json({
++"a": {
++++"value": 42
++},
++"b": {
++++"value": 2020
++}
})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

struct Array {
	std::vector<int> a;
};

TEST(struct_mapping_mapper_map_struct_to_json, test_array) {
	Array source;

	struct_mapping::reg(&Array::a, "a");

	source.a.push_back(1);
	source.a.push_back(3);
	source.a.push_back(5);

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json);

	std::string expected_json(R"json({"a":[1,3,5]})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

TEST(struct_mapping_mapper_map_struct_to_json_pretty, test_array) {
	Array source;

	source.a.push_back(1);
	source.a.push_back(3);
	source.a.push_back(5);

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json, "++");

	std::string expected_json(
R"json({
++"a": [
++++1,
++++3,
++++5
++]
})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

struct ComplexAuthor {
	std::string name;
	int birth_year;
};

struct ComplexBook {
	double price;
	bool free;
	ComplexAuthor author;
	std::vector<std::string> chapters;
};

TEST(struct_mapping_mapper_map_struct_to_json, test_complex) {
	ComplexBook source;

	struct_mapping::reg(&ComplexAuthor::name, "name");
	struct_mapping::reg(&ComplexAuthor::birth_year, "birth_year");
	struct_mapping::reg(&ComplexBook::price, "price");
	struct_mapping::reg(&ComplexBook::free, "free");
	struct_mapping::reg(&ComplexBook::author, "author");
	struct_mapping::reg(&ComplexBook::chapters, "chapters");

	source.price = 12.35;
	source.free = false;
	source.author.name = "Gogol";
	source.author.birth_year = 1809;
	source.chapters.push_back(std::string("B 1999"));
	source.chapters.push_back(std::string("M 1998"));
	source.chapters.push_back(std::string("E 1997"));

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json);

	std::string expected_json(R"json({"price":12.35,"free":false,"author":{"name":"Gogol","birth_year":1809},"chapters":["B 1999","M 1998","E 1997"]})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

TEST(struct_mapping_mapper_map_struct_to_json_pretty, test_complex) {
	ComplexBook source;

	source.price = 12.35;
	source.free = false;
	source.author.name = "Gogol";
	source.author.birth_year = 1809;
	source.chapters.push_back(std::string("B 1999"));
	source.chapters.push_back(std::string("M 1998"));
	source.chapters.push_back(std::string("E 1997"));

	std::ostringstream result_json;
	struct_mapping::map_struct_to_json(source, result_json, "++");

	std::string expected_json(
R"json({
++"price": 12.35,
++"free": false,
++"author": {
++++"name": "Gogol",
++++"birth_year": 1809
++},
++"chapters": [
++++"B 1999",
++++"M 1998",
++++"E 1997"
++]
})json");

	ASSERT_EQ(result_json.str(), expected_json);
}

}