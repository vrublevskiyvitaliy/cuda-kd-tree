//
//  KDTreeNode.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef KDTreeNode_hpp
#define KDTreeNode_hpp

#include <stdio.h>

#include <cmath>
#include <glm/glm.hpp>


struct KDTreeNode {
    
    unsigned int  BIT_LEAF = 0x80;
    unsigned int  BIT_DIM  = 0x80;
    unsigned int  BIT_SIGN_X  = 0x80;
    unsigned int  BIT_SIGN_Y  = 0x80;
    unsigned int  BIT_NUMBER_BEGIN  = 0x7F;
    unsigned int  BIT_NUMBER_END  = 0x7F80;
    unsigned int  BIT_NUMBER_CONVERT  = 0x7F;
    
    unsigned int MAX_NUMBER = 16384;
    
    glm::vec2 p;
    int dim;
    int index;
    bool isLeaf;
    bool isEmpty;
    float r,g,b,a;
    
    KDTreeNode();
    void setRGBA();
    KDTreeNode getNodeByRGBA(float r, float g, float b, float a);
    bool compare(KDTreeNode node);

};
#endif /* KDTreeNode_hpp */
