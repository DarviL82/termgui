#pragma once

#include <string>
#include "myhtml/api.h"
#include <vector>

typedef myhtml_tree_node_t myhtml_node;

class HtmlParser;
class HtmlNode;


myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_collection_t* collection, myhtml_node* node,
	const std::string& key, const std::string& value, bool case_sensitive
);

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, myhtml_node* node,
	const std::string& key, const std::string& value, bool case_sensitive
);

myhtml_collection_t* my_get_nodes_by_attribute(
	myhtml_tree_t* tree, const std::string& key, const std::string& value, bool case_sensitive
);

myhtml_collection_t* my_get_nodes_by_tagname(
	myhtml_tree_t* tree, myhtml_collection_t* collection, const std::string& tagname
);

myhtml_collection_t* my_get_nodes_by_tagname(myhtml_tree_t* tree, const std::string& tagname);



class HtmlNode {
	const HtmlParser* parser;
	myhtml_node* node;

	friend HtmlParser;

public:
	HtmlNode(myhtml_node* n, const HtmlParser* p) :
		node{n}, parser{p} { }

	~HtmlNode();

	// node navigation
	HtmlNode next();
	HtmlNode prev();
	HtmlNode parent();
	HtmlNode first_child();
	HtmlNode last_child();
	std::vector<HtmlNode> children();


	// node params
	bool has_closing_tag();
	std::string tag_name();
	std::string text_content();

protected:
	std::string text();

private:
	HtmlNode from(myhtml_node* n) const;
};


/**
 * Class for interacting with the underlying myhtml lib a bit easier.
 */
class HtmlParser {
	myhtml_t* html = myhtml_create();
public:
	myhtml_tree_t* html_tree = myhtml_tree_create();

	HtmlParser();

	~HtmlParser();

	void parse(const std::string& _html) const;
	[[nodiscard]] HtmlNode first_node() const;
	[[nodiscard]] HtmlNode get_node_by_tag(const std::string& tag) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_tag(const std::string& tag) const;
	[[nodiscard]] HtmlNode get_node_by_classname(const std::string& selector, bool case_sensitive = true) const;
	[[nodiscard]] std::vector<HtmlNode>
	get_nodes_by_classname(const std::string& selector, bool case_sensitive = true) const;

private:
	std::vector<HtmlNode> myhtml_collection_to_htmlnode_vec(myhtml_collection_t* collection) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_attribute(
		const std::string& key, const std::string& value, bool case_sensitive = true
	) const;
	[[nodiscard]] std::vector<HtmlNode> get_nodes_by_attribute(
		const HtmlNode& scope, const std::string& key, const std::string& value, bool case_sensitive = true
	) const;
};