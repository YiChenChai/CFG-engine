#include "CFG.h"
#include <iostream>
int main() {
	Grammar::addTerm(NonTerm, "S");
	Grammar::addTerm(StrLit, "-");
	Grammar::addTerm(StrLit, "+");
	Grammar::addTerm(StrLit, "a");
	Grammar g;
	const char* opts[] = {"-", "S", "S", NULL}; const char* opts2[] = {"+", "S", "S", NULL}; const char* opts3[] = {"a", NULL};
	g.addProd("S", opts);
	g.addProd("S", opts2);
	g.addProd("S", opts3);
	g.nextTerm();
	Node* n = g.input("S");
	NodeGraph ng;
	ng.traverse(n);
	std::cout << ng.renderText();
}