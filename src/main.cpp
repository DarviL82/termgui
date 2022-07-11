#include <string>
#include <iostream>

#include "html_parser.hpp"


int main() {
	HtmlParser parser;
	parser.parse("<html><body><h1>Hello World</h1><img src=\"ball\"/></body></html>");


	auto test = parser.get_node_by_tag("body").children();

	for (auto x : test) {
		std::cout << x.tag_name() << " ";
	}

	// first tree init
//	myhtml_tree_t* tree = myhtml_tree_create();
//	myhtml_tree_init(tree, myhtml);
//
//	// parse html
//	myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));
//
//	// print result
//	// or see serialization function with callback: myhtml_serialization_tree_callback
//	mycore_string_raw_t str = {0};
//	myhtml_serialization_tree_buffer(myhtml_tree_get_document(tree), &str);
//	printf("%s\n", str.data);
//
//	// release resources
//	mycore_string_raw_destroy(&str, false);
//	myhtml_tree_destroy(tree);
//	myhtml_destroy(myhtml);
//
//	return 0;
}