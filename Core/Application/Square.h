#pragma once
#include "Mesh.h"

class Square : public Mesh
{
public:
    void OnInit() override;
    void OnUpdate(TimeStep ts) override;
};