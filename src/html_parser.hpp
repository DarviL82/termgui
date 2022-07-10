//
// Created by darvil on 7/9/22.
//

#pragma once

#include <string>
#include "myhtml/api.h"


class HtmlParser;


class HtmlNode {
	const HtmlParser& parser;
	myhtml_tree_node_t* node;

public:
	HtmlNode(myhtml_tree_node_t* n, const HtmlParser& p):
		node{n}, parser{p}
	{}

	HtmlNode next();
	HtmlNode prev();
	HtmlNode parent();
	HtmlNode child();
	HtmlNode last_child();

	std::string tag_name();

private:
	HtmlNode from(myhtml_tree_node_t* n) const;
};



class HtmlParser {
	myhtml_t* html = myhtml_create();
public:
	myhtml_tree_t* html_tree = myhtml_tree_create();

	HtmlParser();

	~HtmlParser();

	[[nodiscard]] HtmlNode parse(const std::string& _html) const;
};


