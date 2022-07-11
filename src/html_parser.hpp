//
// Created by darvil on 7/9/22.
//

#pragma once

#include <string>
#include "myhtml/api.h"
#include <vector>


class HtmlParser;


class HtmlNode {
	const HtmlParser* parser;
	myhtml_tree_node_t* node;

public:
	HtmlNode(myhtml_tree_node_t* n, const HtmlParser* p) :
			node{n}, parser{p} { }

	~HtmlNode();

	// node navigation
	HtmlNode next();
	HtmlNode prev();
	HtmlNode parent();
	HtmlNode child();
	HtmlNode last_child();

	// node params
	std::string text();
	bool has_closing_tag();
	std::string tag_name();
	std::string get_text_content();

private:
	HtmlNode from(myhtml_tree_node_t* n) const;
};


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