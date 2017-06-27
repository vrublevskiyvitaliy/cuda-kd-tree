//
//  KDTreeNode.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 3/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "KDTreeNode.hpp"

KDTreeNode::KDTreeNode() {
    dim = -1;
    isEmpty = true;
    isLeaf = false;
    index = -1;
    r = 0;g = 0;b = 0;a = 0;
}

void KDTreeNode::setRGBA() {
    if (isLeaf) {
        r += BIT_LEAF;
    }
    if (dim) {
        g += BIT_DIM;
    }
    if (p.x < 0) {
        b += BIT_SIGN_X;
    }
    if (p.y < 0) {
        a += BIT_SIGN_Y;
    }
    float inverse = 1./ MAX_NUMBER;
    long long x_number = (fabs(p.x) / inverse);
    long long y_number = (fabs(p.y) / inverse);
    
    long long r_n =(x_number & BIT_NUMBER_BEGIN );
    r += r_n;
    
    long long g_n =(x_number & BIT_NUMBER_END );
    g_n >>= 7;
    
    g += g_n;
    
    b += (y_number & BIT_NUMBER_BEGIN );
    
    a += (y_number & BIT_NUMBER_END) >> 7;
    
    r /= 255.; g /= 255.; b /= 255.; a /= 255.;
}

KDTreeNode KDTreeNode::getNodeByRGBA(float r, float g, float b, float a) {
    r *= 255; g *= 255; b *= 255; a *= 255;
    
    long long r_n = (int)r;
    long long g_n = (int)g;
    long long b_n = (int)b;
    long long a_n = (int)a;
    
    KDTreeNode node;

    node.isLeaf = r_n & BIT_LEAF;
    node.dim = (g_n & BIT_DIM) > 1 ? 1 : 0;
    
    int sign_x = b_n & BIT_SIGN_X ? -1 : 1 ;
    int sign_y = a_n & BIT_SIGN_Y ? -1 : 1 ;
    
    long long part_1 = r_n & BIT_NUMBER_CONVERT;
    long long part_2 = (g_n & BIT_NUMBER_CONVERT) << 7;
    
    float x = part_1 + part_2;

    
    part_1 = b_n & BIT_NUMBER_CONVERT;
    part_2 = (a_n & BIT_NUMBER_CONVERT) << 7;
    
    float y = part_1 + part_2;
    
    float inverse = 1./ MAX_NUMBER;
    
    x *= inverse;
    y *= inverse;
    
    node.p.x = sign_x * x;
    node.p.y = sign_y * y;
    
    return node;
}

bool KDTreeNode::compare(KDTreeNode node) {
    return node.dim == dim &&
    node.isLeaf == isLeaf &&
    fabs(node.p.x - p.x) < 0.0001 &&
    fabs(node.p.y - p.y) < 0.0001;
}
