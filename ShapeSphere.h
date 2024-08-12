//
// Created by me on 8/12/24.
//

#ifndef SHAPESPHERE_H
#define SHAPESPHERE_H

#include "Shape.h"

class ShapeSphere : public Shape {
public:
    ShapeSphere(float radius) : m_Radius (radius) {};
    ShapeType GetType() const override { return SHAPE_SPHERE; };

    float m_Radius;
};



#endif //SHAPESPHERE_H
