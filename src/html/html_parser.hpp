#pragma once

#include <string>
#include "myhtml/api.h"
#include <vector>

typedef myhtml_tree_node_t myhtml_node;

class HtmlParser;
class HtmlNode;


myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_collection_t* collection, myhtml_node* node,
	const std::string& key, const std::string& value, bool case_sensitive = false
);

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_node* scope,
	const std::string& key, const std::string& value, bool case_sensitive = false
);

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, const std::string& key, const std::string& value, bool case_sensitive = false
);

myhtml_collection_t* my_get_nodes_by_tagname(
	myhtml_tree_t* tree, myhtml_collection_t* collection, const std::string& tagname
);

myhtml_collection_t* my_get_nodes_by_tagname(myhtml_tree_t* tree, const std::string& tagname);


class HtmlNoChildrenException : public std::exception {
public:
	[[nodiscard]] const char* what() const noexcept override;
	static void throw_if_no_children(const HtmlNode& node);
};

class HtmlNoSiblingException : public std::exception {
	bool is_left;
public:
	explicit HtmlNoSiblingException(bool is_left): is_left(is_left) {}

	[[nodiscard]] const char* what() const noexcept override;
	static void throw_if_no_sibling(const HtmlNode& node, bool left);
};


class HtmlNode {
	const HtmlParser* parser;
	myhtml_node* node;

	friend HtmlParser;

public:
	HtmlNode(myhtml_node* n, const HtmlParser* p) :
		node{n}, parser{p}
	{ }

	~HtmlNode();

	// node navigation
	[[nodiscard]] HtmlNode next() const;
	[[nodiscard]] HtmlNode prev() const;
	[[nodiscard]] HtmlNode parent() const;
	[[nodiscard]] HtmlNode first_child() const;
	[[nodiscard]] HtmlNode last_child() const;
	[[nodiscard]] std::vector<HtmlNode> children() const;
	[[nodiscard]] bool has_children() const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_tagname(const std::string& tag) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_classname(const std::string& classname) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_id(const std::string& id) const;
	[[nodiscard]] HtmlNode get_node_by_tagname(const std::string& tag) const;
	[[nodiscard]] HtmlNode get_node_by_classname(const std::string& classname) const;
	[[nodiscard]] HtmlNode get_node_by_id(const std::string& id) const;

	// node params
	[[nodiscard]] bool has_closing_tag() const;
	[[nodiscard]] std::string tag_name() const;
	[[nodiscard]] std::string text_content() const;

protected:
	[[nodiscard]] std::string text() const;

private:
	HtmlNode from(myhtml_node* n) const;
};


/**
 * Class for interacting with the underlying myhtml lib a bit easier.
 */
class HtmlParser {
	myhtml_t* html = myhtml_create();

	friend HtmlNode;
public:
	myhtml_tree_t* html_tree = myhtml_tree_create();

	HtmlParser();

	~HtmlParser();

	[[nodiscard]] HtmlNode parse(const std::string& _html) const;
	[[nodiscard]] HtmlNode first_node() const;

private:
	std::vector<HtmlNode> myhtml_collection_to_htmlnode_vec(myhtml_collection_t* collection) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_attribute(
		const std::string& key, const std::string& value, bool case_sensitive = false
	) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_attribute(
		const HtmlNode& scope, const std::string& key, const std::string& value, bool case_sensitive = false
	) const;
};