#include "html_parser.hpp"

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_collection_t* collection, myhtml_node* node,
	const std::string& key, const std::string& value, bool case_sensitive
) {
	return myhtml_get_nodes_by_attribute_value(
		tree,
		collection,
		node,
		!case_sensitive,
		key.c_str(),
		key.length(),
		value.c_str(),
		value.length(),
		nullptr
	);
}

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_node* scope,
	const std::string& key, const std::string& value, bool case_sensitive
) {
	return my_get_nodes_by_attribute(tree, nullptr, scope, key, value, case_sensitive);
}

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, const std::string& key, const std::string& value, bool case_sensitive
) {
	return my_get_nodes_by_attribute(tree, nullptr, nullptr, key, value, case_sensitive);
}

myhtml_collection_t* my_get_nodes_by_tagname(
	myhtml_tree_t* tree, myhtml_collection_t* collection, const std::string& tagname
) {
	return myhtml_get_nodes_by_name(tree, collection, tagname.c_str(), tagname.length(), nullptr);
}

myhtml_collection_t* my_get_nodes_by_tagname(myhtml_tree_t* tree, const std::string& tagname) {
	return my_get_nodes_by_tagname(tree, nullptr, tagname);
}



std::vector<HtmlNode> HtmlParser::myhtml_collection_to_htmlnode_vec(myhtml_collection_t* collection) const {
	std::vector<HtmlNode> newNodes;

	for (size_t i = 0; i < collection->length; i++)
		newNodes.emplace_back(collection->list[i], this);

	return newNodes;
}


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

HtmlNode HtmlParser::first_node() const {
	return {myhtml_node_first(html_tree), this};
}

std::vector<HtmlNode> HtmlParser::get_nodes_by_tag(const std::string& tag) const {
	return myhtml_collection_to_htmlnode_vec(my_get_nodes_by_tagname(html_tree, tag));
}

HtmlNode HtmlParser::get_node_by_tag(const std::string& tag) const {
	return get_nodes_by_tag(tag)[0];
}

std::vector<HtmlNode>
HtmlParser::get_nodes_by_classname(const std::string& classname, bool case_sensitive) const {
	return get_nodes_by_attribute("class", classname, case_sensitive);
}

HtmlNode HtmlParser::get_node_by_classname(const std::string& selector, bool case_sensitive) const {
	return get_nodes_by_classname(selector, case_sensitive)[0];
}

std::vector<HtmlNode>
HtmlParser::get_nodes_by_attribute(const std::string& key, const std::string& value, bool case_sensitive) const {
	return myhtml_collection_to_htmlnode_vec(my_get_nodes_by_attribute(html_tree, key, value, case_sensitive));
}

std::vector<HtmlNode>
HtmlParser::get_nodes_by_attribute(
		const HtmlNode& scope, const std::string& key, const std::string& value, bool case_sensitive
) const {
	return myhtml_collection_to_htmlnode_vec(
		my_get_nodes_by_attribute(html_tree, scope.node, key, value, case_sensitive)
	);
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
	std::vector<HtmlNode> newNodes;

	for (
		myhtml_node* sibling = myhtml_node_child(node);
		sibling;
		sibling = myhtml_node_next(sibling)
	) {
		newNodes.push_back(from(sibling));
	}

	return newNodes;
}