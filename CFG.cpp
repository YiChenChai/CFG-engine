#include "CFG.h"

int Terminal::termcount = 0;
int Node::nodecnt = 0;
std::vector<Terminal* > Grammar::terms;
bool Terminal::matches(Grammar* parent, char c) {
	if (type == StrLit) {
		if (name[0] == c) return true;
		else return false;
	}
	else if (type == NonTerm) {
		std::vector<std::vector<int> >& prods = parent->getProd();
		int sz = prods.size();
		bool res = false;
		for (int i = 0;i < sz; i++) {
			if (prods[i][0] == termid) {
				if (Grammar::terms[prods[i][1]]->matches(parent, c)) {
					res = true;
					break;
				}
			}
		}
		return res;
	}
	else if (type == Empty) {
		return false;
	}
	// return false;
}
char* Terminal::getName() {
	return name;
}
void Grammar::addTerm(TermType t, const char* name) {
	Terminal* entry = new Terminal(t, name);
	terms.push_back(entry);
}
Terminal* Grammar::find(const char* name) {
	for (int i = 0; i < Terminal::termcount; i++) {
		if (strcmp(terms[i]->name, name) == 0) return terms[i];
	}
	return NULL;
}
void Grammar::addProd(const char* nonterm, const char** tlist) {
	std::vector<int> prod;
	prod.push_back(find(nonterm)->termid);
	int i = 0;
	for (; tlist[i] != NULL; i++) {
		prod.push_back(find(tlist[i])->termid);
	}
	if (i == 0) prod.push_back(find("")->termid);
	productions.push_back(prod);
	prodcount++;
}

void Grammar::nextTerm() {
	lookahead = getchar();
}
void Grammar::match(Node* n, Terminal* t, bool emp) {
	if (t->matches(this, lookahead)) {
		if (t->type==StrLit) {
			n->addChild(new Node(Grammar::terms[t->termid]));
			nextTerm();
		}
		else if (t->type == NonTerm) {
			depth++;
			n->addChild(input(t->termid));
			depth--;
		}
	}
	else if (emp) {
		Node* parent = new Node(Grammar::terms[t->termid]);
		parent->addChild(new Node(find("")));
		n->addChild(parent);
	}
	else throw Exception();
}
Node* Grammar::input(const char* nt) {
	return input(find(nt)->termid);
}
Node* Grammar::input(int ntid) {
	int prodcand = -1;
	bool allowemp = false;
	Node *n = new Node(Grammar::terms[ntid]);
	for (int i = 0; i < prodcount; i++) {
		int ntidx = productions[i][0];
		if (terms[ntidx]->termid == ntid) {
			int idx = productions[i][1];
			Terminal* target = terms[idx];
			if (target->matches(this, lookahead)) {
				prodcand = i;
			}
			else if (target->type == Empty){
				allowemp = true;
			}
		}
	}
	if (prodcand == -1) {
		if (!allowemp) throw Exception();
		else {
			return new Node(find(""));
		}
	}
	else execProd(n, prodcand, allowemp);
	if (depth == 0 && lookahead != '\n') {
		throw Exception();
	}
	return n;
}
void Grammar::execProd(Node* n, int idx, bool emp) {
	for (int i = 1; i < productions[idx].size(); i++) {
		match(n, terms[productions[idx][i]], emp);
	}
}

std::vector<std::vector<int> >& Grammar::getProd() {
	return productions;
}

void Node::addChild(Node* n) {
	children.push_back(n);
}

std::vector<Node* >& Node::getChildren() {
	return children;
}
Terminal* Node::getType() {
	return ntype;
}
std::string* Node::strRep() {
	if (ntype->type == StrLit) {
		std::string* s = new std::string(ntype->getName());
		*s = "\'" + *s + "\'";
		return s;
	}
	else if (ntype->type == NonTerm) {
		return new std::string(ntype->getName());
	}
	else {
		return new std::string("ϵ");
	}
}
int Node::getId() {
	return nodeid;
}
void NodeGraph::traverse(Node* n) {
	int nid = n->getId();
	labels[nid] = n->strRep();
	typeinfo[nid] = n->getType();
	std::vector<Node* >& nchildren = n->getChildren();
	for (int i = 0; i < nchildren.size(); i++) {
		adjlist[nid].push_back(nchildren[i]->getId());
		traverse(nchildren[i]);
	}
}

std::string NodeGraph::renderText() {
	int nodesz = labels.size();
	std::ostringstream o;
	o << "graph G {" << std::endl;
	for (int i = 0; i < nodesz; i++) {
		switch(typeinfo[i]->type) {
			case StrLit:
			case Empty:
				o << "\t" << i << "[label=\"" << *labels[i] << "\", shape=none];" << std::endl;
				break;
			case NonTerm:
				o << "\t" << i << "[label=<<I>" << *labels[i] << "</I>>, shape=none];" << std::endl;
				break;
		}
		int sz = adjlist[i].size();
		for (int j = 0; j < sz; j++) {
			o << "\t" << i << "--" << adjlist[i][j] << ";" << std::endl;
		}
	}
	o << "}" << std::endl;
	return o.str();
}