#include "tree.hpp"
#include <string>
#include <iostream>

void printT(std::ostream& out, const std::string& prefix, Tree* node, bool isLeft);

void Tree::printTree(std::ostream& out){
	printT(out, "", this, false);
}

/*
 * Part of the logic of printT is from the following link :
 * Nikki Firpo
 * https://copyprogramming.com/howto/print-binary-tree-in-a-pretty-way-using-c
 */


void printT(std::ostream& out, const std::string& prefix, Tree* node, bool isLeft)
{
    if( node != nullptr )
    {
        out << prefix;
        out << (isLeft ? "├──" : "└──" );
        // print the value of the node
		node->printNode(out); out << std::endl;
		std::vector<Tree*> children = node->getChildren();
		for(size_t i = 0; i < children.size(); i++){
			auto child = children[i];
			if(i == children.size() - 1){
				printT(out, prefix + (isLeft ? "│   " : "    "), child, false);	
			}else{
				printT(out, prefix + (isLeft ? "│   " : "    "), child, true);		
			}
		}
    }
}

