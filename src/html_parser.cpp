#include <stdexcept>
#include "html_parser.hpp"

HtmlParser::HtmlParser() {
	myhtml_init(html, MyHTML_OPTIONS_DEFAULT, 1, 0);
	myhtml_tree_init(html_tree, html);
}

HtmlParser::~HtmlParser() {
	myhtml_tree_destroy(html_tree);
	myhtml_destroy(html);
}

void HtmlParser::parse(const std::string& _html) const {
	myhtml_parse(html_tree, MyENCODING_UTF_8, _html.c_str(), _html.length());
}

HtmlNode HtmlParser::first_node() {
	return {myhtml_node_first(html_tree), this};
}

std::vector<HtmlNode> HtmlParser::get_nodes_by_tag(const std::string& tag) {
	myhtml_collection_t* collection = myhtml_get_nodes_by_name(
		html_tree,
		nullptr,
		tag.c_str(),
		tag.length(),
		nullptr
	);

	auto newNodes = std::vector<HtmlNode>();

	for (size_t i = 0; i < collection->length; i++)
		newNodes.emplace_back(collection->list[i], this);

	return newNodes;
}

HtmlNode HtmlParser::get_node_by_tag(const std::string& tag) {
	return get_nodes_by_tag(tag)[0];
}

HtmlNode::~HtmlNode() {
	myhtml_node_free(node);
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

HtmlNode HtmlNode::first_child() {
	return from(myhtml_node_child(node));
}

HtmlNode HtmlNode::last_child() {
	return from(myhtml_node_last_child(node));
}

std::string HtmlNode::tag_name() {
	return myhtml_tag_name_by_id(parser->html_tree, myhtml_node_tag_id(node), nullptr);
}

HtmlNode HtmlNode::from(myhtml_node* n) const {
	return {n, parser};
}

std::string HtmlNode::text() {
	return myhtml_node_text(node, nullptr);
}

bool HtmlNode::has_closing_tag() {
	return !myhtml_node_is_close_self(node);
}

std::string HtmlNode::text_content() {
	return first_child().text();
}

std::vector<HtmlNode> HtmlNode::children() {
	auto newNodes = std::vector<HtmlNode>();

	for (
		myhtml_node* sibling = myhtml_node_child(node);
		sibling;
		sibling = myhtml_node_next(sibling)
	) {
		newNodes.push_back(from(sibling));
	}

	return newNodes;
}

