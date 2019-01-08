#include "CFG.h"
#include <iostream>
int main() {
	// 
	Grammar::addTerm(NonTerm, "S");
	Grammar::addTerm(NonTerm, "R");
	Grammar::addTerm(StrLit, "0");
	Grammar::addTerm(StrLit, "1");
	Grammar g;
	const char* opts[] = {"0", "R", NULL}; const char* opts2[] = {"S", "1", NULL}; const char* opts3[] = {"1", NULL};
	g.addProd("S", opts);
	g.addProd("R", opts2);
	g.addProd("R", opts3);
	g.nextTerm();
	Node* n = g.input("S");
	NodeGraph ng;
	ng.traverse(n);
	std::cout << ng.renderText();
}
