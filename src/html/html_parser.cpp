#include "html_parser.hpp"

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_collection_t* collection, myhtml_node* node,
	const std::string& key, const std::string& value, bool case_sensitive
) {

	myhtml_collection_t* nodes = myhtml_get_nodes_by_attribute_value(
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

	if (nodes == nullptr || nodes->length == 0) {
		throw HtmlNoElementsFoundException();
	}

	return nodes;
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
	auto new_collection = myhtml_get_nodes_by_name(tree, collection, tagname.c_str(), tagname.length(), nullptr);

	if (new_collection == nullptr || new_collection->length == 0) {
		throw HtmlNoElementsFoundException();
	}

	return new_collection;
}

myhtml_collection_t* my_get_nodes_by_tagname(myhtml_tree_t* tree, const std::string& tagname) {
	return my_get_nodes_by_tagname(tree, nullptr, tagname);
}




// *********************************************************************************************************************
// Exceptions
// *********************************************************************************************************************

const char* HtmlNoChildrenException::what() const noexcept {
	return "HtmlNode has no children";
}

void HtmlNoChildrenException::throw_if_no_children(const HtmlNode& node) {
	if (!node.has_children()) throw HtmlNoChildrenException();
}


const char* HtmlNoSiblingException::what() const noexcept {
	return is_left ? "HtmlNode has no left sibling" : "HtmlNode has no right sibling";
}


const char* HtmlNoElementsFoundException::what() const noexcept {
	return "No elements found";
}




// *********************************************************************************************************************
// HtmlParser
// *********************************************************************************************************************

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

HtmlNode HtmlParser::parse(const std::string& _html) const {
	myhtml_parse(html_tree, MyENCODING_UTF_8, _html.c_str(), _html.length());
	return first_node();
}

HtmlNode HtmlParser::parse_file(const std::string& filename) const {
	std::ifstream in(filename);
	std::stringstream str;
	str << in.rdbuf();
	return parse(str.str());
}

HtmlNode HtmlParser::first_node() const {
	return {myhtml_node_first(html_tree), this};
}




// *********************************************************************************************************************
// HtmlNode
// *********************************************************************************************************************

HtmlNode::~HtmlNode() {
	myhtml_node_free(node);
}

HtmlNode HtmlNode::next() const {
	myhtml_node* sibling = myhtml_node_next(node);

	return from((sibling != nullptr) ? sibling : throw HtmlNoSiblingException(false));
}

HtmlNode HtmlNode::prev() const {
	myhtml_node* sibling = myhtml_node_prev(node);

	return from((sibling != nullptr) ? sibling : throw HtmlNoSiblingException(true));
}

HtmlNode HtmlNode::parent() const {
	return from(myhtml_node_parent(node));
}

HtmlNode HtmlNode::first_child() const {
	HtmlNoChildrenException::throw_if_no_children(*this);

	return from(myhtml_node_child(node));
}

HtmlNode HtmlNode::last_child() const {
	HtmlNoChildrenException::throw_if_no_children(*this);

	return from(myhtml_node_last_child(node));
}

std::string HtmlNode::tag_name() const {
	return myhtml_tag_name_by_id(parser->html_tree, myhtml_node_tag_id(node), nullptr);
}

HtmlNode HtmlNode::from(myhtml_node* n) const {
	return {n, parser};
}

std::string HtmlNode::text() const {
	return myhtml_node_text(node, nullptr);
}

bool HtmlNode::has_closing_tag() const {
	return !myhtml_node_is_close_self(node);
}

std::string HtmlNode::text_content() const {
	return first_child().text();
}

std::vector<HtmlNode> HtmlNode::children() const {
	HtmlNoChildrenException::throw_if_no_children(*this);

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

std::vector<HtmlNode> HtmlNode::get_nodes_by_tagname(const std::string& tag) const {
	return parser->myhtml_collection_to_htmlnode_vec(
		my_get_nodes_by_tagname(myhtml_node_tree(node), tag)
	);
}

std::vector<HtmlNode> HtmlNode::get_nodes_by_classname(const std::string& classname) const {
	return parser->myhtml_collection_to_htmlnode_vec(
		my_get_nodes_by_attribute(myhtml_node_tree(node), "class", classname)
	);
}

std::vector<HtmlNode> HtmlNode::get_nodes_by_id(const std::string& id) const {
	return parser->myhtml_collection_to_htmlnode_vec(
		my_get_nodes_by_attribute(myhtml_node_tree(node), "id", id)
	);
}

HtmlNode HtmlNode::get_node_by_tagname(const std::string& tag) const {
	return get_nodes_by_tagname(tag)[0];
}

HtmlNode HtmlNode::get_node_by_classname(const std::string& classname) const {
	return get_nodes_by_classname(classname)[0];
}

HtmlNode HtmlNode::get_node_by_id(const std::string& id) const {
	return get_nodes_by_id(id)[0];
}

bool HtmlNode::has_children() const {
	return myhtml_node_child(node) != nullptr;
}

