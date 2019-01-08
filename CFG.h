#include <vector>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdio>
#include <map>
#include <iostream>
#include <sstream>

enum TermType { NonTerm, StrLit, Empty };
class Grammar;
class Node;
class Terminal;
class Terminal {
private:
	static int termcount;
	char* name;
public:
	const TermType type;
	const int termid;
	Terminal(TermType t, const char* n) : type(t), termid(termcount++) {
		name = new char[strlen(n) + 1];
		strcpy(name, n);
	}
	bool matches(Grammar*, char);
	char* getName();
	friend class Grammar;
};
class Grammar {
private:
	static std::vector<Terminal* > terms;
	std::vector<std::vector<int> > productions;
	int prodcount;
	char lookahead;
	int depth;
public:
	Grammar() : prodcount(0), depth(0) {}
	Terminal* find(const char*);
	static void addTerm(TermType, const char*);
	void addProd(const char*, const char**);
	void nextTerm();
	void match(Node*, Terminal*, bool);
	Node* input(const char*);
	Node* input(int ntid);
	void execProd(Node*, int, bool);
	std::vector<std::vector<int> >& getProd();
	friend class Terminal;
};
class Node {
private:
	Terminal* ntype;
	std::vector<Node* > children;
	int nodeid;
	static int nodecnt;
public:
	void addChild(Node*);
	std::vector<Node* >& getChildren();
	Terminal* getType();
	int getId();
	std::string* strRep();
	Node(Terminal* t) : ntype(t), nodeid(nodecnt++) {}
};
class NodeGraph {
private:
	std::map<int, std::vector<int> > adjlist;
	std::map<int, std::string* > labels;
	std::map<int, Terminal* > typeinfo;
public:
	std::string renderText();
	void traverse(Node*);
};
class Exception {

};