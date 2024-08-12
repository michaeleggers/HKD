//
// Created by me on 8/12/24.
//

#ifndef SHAPE_H
#define SHAPE_H



class Shape {
public:
    enum ShapeType {
        SHAPE_SPHERE
    };

    virtual ShapeType GetType() const = 0;
};





#endif //SHAPE_H
