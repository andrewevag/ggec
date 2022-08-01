#ifndef __TREE_HPP__
#define __TREE_HPP__
#include <vector>
#include <iostream>

class Tree{
public:
	void printTree(std::ostream& out);
	virtual std::vector<Tree*> getChildren() = 0;
	/**
	 * @brief need to be overriden , print nodes' name on out without endl
	 * 
	 * @param out the stream to print to
	 */
	virtual void printNode(std::ostream& out) = 0;

};





#endif/*__TREE_HPP__ */
