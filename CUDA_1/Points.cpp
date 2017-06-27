//
//  Points.cpp
//  V1
//
//  Created by Vitaliy Vrublevskiy on 2/24/17.
//  Copyright © 2017 Vitaliy Vrublevskiy. All rights reserved.
//

#include "Points.hpp"

Points::Points(int number, OpenGLHelper * _openGLHelper) {
    numberOfPoints = number > 0
        ? number
        : DEFAULT_POINTS_NUMBER;
    
    openGLHelper = _openGLHelper;
    
    pointsPositions = std::vector<glm::vec2>(numberOfPoints);
    pointsMoves = std::vector<glm::vec2>(numberOfPoints);
    pointsColors = std::vector<glm::vec3>(numberOfPoints);
    
    initPoints();
}

void Points::initPoints() {
    
    float speed, a, w, h, dx, dy, x, y;
    for (int i = 0; i < numberOfPoints; i++ )
    {
        pointsPositions[i].x     = random.getRandomInRange(-0.95, 0.95);
        pointsPositions[i].y     = random.getRandomInRange(-0.95, 0.95);

		float M_PI = 3.1415926;
        speed = random.getRandomInRange(0.002, 0.005);
        a     = random.getRandomInRange(0, M_PI * 2);
        
        dx    = ( speed * cos(a) );
        dy    = ( speed * sin(a) );
        
		pointsPositions[i].x = random.getRandomPosition();
		pointsPositions[i].y = random.getRandomPosition();

        pointsMoves[i].x = dx;
        pointsMoves[i].y = dy;

        pointsColors[i].x = random.getRandom();
        pointsColors[i].y = random.getRandom();
        pointsColors[i].z = random.getRandom();
    }
}

int Points::getNumberOfPoints() {
    return numberOfPoints;
}

void Points::applyMove(bool useMouse) {
    float px, py, dx, dy;
    
    glm::vec3 mouse = openGLHelper->getMousePosition();
    
    float mx = mouse.x;
    float my = mouse.y;
    
    for (int i = 0; i < numberOfPoints && false; i++ )
    {
        px = pointsPositions[i].x;
        py = pointsPositions[i].y;
        dx = pointsMoves[i].x;
        dy = pointsMoves[i].y;
        
        if (useMouse) {
            
            float dxm = mx-px;
            float dym = my-py;
            
            float d_sq = (dxm*dxm)+(dym*dym);
            
            float damp = -0.0005 /(d_sq);
            
            px += dxm * damp ;
            py += dym * damp ;

        }
        
        px += dx;
        py += dy;
        
        if( px < lowerBoundArea || px > upperBoundArea ||
           py < lowerBoundArea || py > upperBoundArea )
        {
            px = 0 + dx * 50;
            py = 0 + dy * 50;
        }
        
        pointsPositions[i].x = px;
        pointsPositions[i].y = py;
    }
}

std::vector<glm::vec2> Points::getPointsPositions() {
    return pointsPositions;
}

std::vector<glm::vec2> Points::getPointsMoves() {
    return pointsMoves;
}

std::vector<glm::vec3> Points::getPointsColors() {
    return pointsColors;
}
