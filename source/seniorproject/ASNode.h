#ifndef ASNODE_H
#define ASNODE_H

#include <cstdlib>

class ASNode
{
    public:
        ASNode(ASNode* inParent = NULL);
        ~ASNode();

    private:
        ASNode* mParent;
};

#endif
