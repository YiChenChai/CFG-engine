#include "CFG.h"
#include <iostream>
int main() {
	Grammar::addTerm(NonTerm, "S");
	Grammar::addTerm(NonTerm, "R");
	Grammar::addTerm(StrLit, "(");
	Grammar::addTerm(StrLit, ")");
	Grammar::addTerm(Empty, "");
	Grammar g;
	const char* opts[] = {"R", NULL}; const char* opts2[] = {"(", "R", ")", "R", NULL}; const char* opts3[] = {NULL};
	g.addProd("S", opts);
	g.addProd("R", opts2);
	g.addProd("R", opts3);
	g.nextTerm();
	Node* n = g.input("S");
	NodeGraph ng;
	ng.traverse(n);
	std::cout << ng.renderText();
}
