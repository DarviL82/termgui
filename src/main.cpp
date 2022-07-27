#include <string>
#include <iostream>

#include "html/html_parser.hpp"
//


int main() {
	HtmlParser parser;
	auto tree = parser.parse_file("test.html");

	HtmlNode nodes = tree.get_node_by_tagname("panel");

	for (auto& node : nodes.children()) {
		std::cout << node.tag_name() << std::endl;
	}
}