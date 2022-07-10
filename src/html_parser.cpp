#include "html_parser.hpp"

HtmlParser::HtmlParser() {
	myhtml_init(html, MyHTML_OPTIONS_DEFAULT, 1, 0);
	myhtml_tree_init(html_tree, html);
}

HtmlParser::~HtmlParser() {
	myhtml_tree_destroy(html_tree);
	myhtml_destroy(html);
}

HtmlNode HtmlParser::parse(const std::string &_html) const {
	myhtml_parse(html_tree, MyENCODING_UTF_8, _html.c_str(), _html.length());
	return {myhtml_node_first(html_tree), *this};
}


HtmlNode HtmlNode::next() {
	return from(myhtml_node_next(node));
}

HtmlNode HtmlNode::prev() {
	return from(myhtml_node_prev(node));
}

HtmlNode HtmlNode::parent() {
	return from(myhtml_node_parent(node));
}

HtmlNode HtmlNode::child() {
	return from(myhtml_node_child(node));
}

HtmlNode HtmlNode::last_child() {
	return from(myhtml_node_last_child(node));
}

std::string HtmlNode::tag_name() {
	return myhtml_tag_name_by_id(parser.html_tree, myhtml_node_tag_id(node), nullptr);
}

HtmlNode HtmlNode::from(myhtml_tree_node_t *n) const {
	return {n, parser};
}
