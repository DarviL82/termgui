#include <string>
#include <iostream>

#include "html/html_parser.hpp"


int main() {
	HtmlParser parser;
	parser.parse("<h1>test1</h1><h1 class='poggers'>test2</h1>");
	auto node = parser.get_node_by_classname("poggers");

	std::cout << node.text_content() << std::endl;
}