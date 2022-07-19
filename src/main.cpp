#include <string>
#include <iostream>

#include "html/html_parser.hpp"



int main() {
	HtmlParser parser;
	auto tree = parser.parse("<h1>test1</h1><h1 class='poggers'><span class='baller'><img class='shit'>hello!</span></h1>");
	HtmlNode nodes = tree.get_node_by_classname("da");

	std::cout << nodes.tag_name() << std::endl;


}