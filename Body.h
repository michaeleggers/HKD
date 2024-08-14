//
// Created by me on 8/12/24.
//

#ifndef BODY_H
#define BODY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Shape.h"

class Body {
public:
    glm::vec3 GetCenterOfMassWorldSpace() const;
    glm::vec3 GetCenterOfMassModelSpace() const;

    glm::vec3 WorldSpaceToBodySpace(glm::vec3& pt);
    glm::vec3 BodySpaceToWorldSpace(glm::vec3& pt);

    glm::vec3   m_Position;
    glm::quat   m_Orientation;
    glm::vec3   m_LinearVelocity;
    Shape*      m_Shape;
};



#endif //BODY_H
