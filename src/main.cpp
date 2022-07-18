#include <string>
#include <iostream>

#include "html/html_parser.hpp"



int main() {
	HtmlParser parser;
	parser.parse("<h1>test1</h1><h1 class='poggers'><span>hello!</span></h1>");
	auto node = parser.get_node_by_classname("poggers");

	std::cout << node.get_nodes_by_tagname("span")[0].text_content() << std::endl;


}