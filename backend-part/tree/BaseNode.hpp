#ifndef BASENODE_H
#define BASENODE_H

#include <vector>

class BaseNode {
protected:
	std::vector<BaseNode*> children;
public:
	BaseNode();
	void addChild(BaseNode* child);
	const std::vector<BaseNode*>& getChildren() const;
	
	virtual bool isLeaf() = 0;
	virtual std::vector<double> evaluate() = 0;
};

#endif