#ifndef SKYBOX_H
#define SKYBOX_H

#include "helper/trianglemesh.h"

class SkyBox : public TriangleMesh
{
public:
    SkyBox(float size = 50.0f);
};


#endif // SKYBOX_H
