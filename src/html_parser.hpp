//
// Created by darvil on 7/9/22.
//

#pragma once

#include <string>
#include "myhtml/api.h"
#include <vector>

typedef myhtml_tree_node_t myhtml_node;



class HtmlParser;

class HtmlNode {
	const HtmlParser* parser;
	myhtml_node* node;

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
	HtmlNode first_node();
	std::vector<HtmlNode> get_nodes_by_tag(const std::string& tag);
	HtmlNode get_node_by_tag(const std::string& tag);
};