//
//  RandomClass.hpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 2/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#ifndef RandomClass_hpp
#define RandomClass_hpp

#include <stdio.h>

class Random{
public:
    Random();
    //return value from 0 to 1
    float getRandom();
    //return value from -1 to +1
    float getRandomPosition();
    //return value from -0.002 to +0.002
    float getRandomMove();
    //return value from min to max
    float getRandomInRange(float min, float max);
};

#endif /* RandomClass_hpp */
