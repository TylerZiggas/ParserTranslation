#include "tree.h"

void traversePreorder(NodeT* node, int depth, string indent, bool last) {
	if (node == NULL) {
		return;
	}

	string indentSymbol = "";
	if (depth > 0) {
		indentSymbol = (last) ? " |_ " : "|- ";
	} else {
		indentSymbol = "   ";
	}
	cout << indent << indentSymbol << node->label << " ";

	for (unsigned int i = 0; i < node->tokens.size(); i++) {
		cout << node->tokens[i].tokenInstance;

		if (((i + 1) != node->tokens.size()) && (node->tokens[i].tokenID != opTk)) {
			cout << ",";
		}
		cout << " ";
	}
	cout << endl;

	indent += (last) ? "   " : "|  ";

	vector<NodeT*> nodes;
	if (node->nodeOne != NULL) {
		nodes.push_back(node->nodeOne);
	}
	if (node->nodeTwo != NULL) {
		nodes.push_back(node->nodeTwo);
	}
	if (node->nodeThree != NULL) {
		nodes.push_back(node->nodeThree);
	}
	if (node->nodeFour != NULL) {
		nodes.push_back(node->nodeFour);
	}
	if (node->nodeFive != NULL) {
		nodes.push_back(node->nodeFive);
	}

	for (unsigned int i = 0; i < nodes.size(); i++) {
		traversePreorder(nodes[i], depth + 1, indent, i == nodes.size() - 1);
	}
}
