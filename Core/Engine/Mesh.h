#pragma once
#include "Application.h"
#include "ObjectLoader.h"
#include "TinyWrapper.h"

class Mesh :
    public Application
{
public:
    Mesh() {}
    Mesh(std::string file_path) :
        m_OBJFilePath{ file_path }
    {

    }

    void OnInit() override;
    void OnUpdate(float ts) override;

    virtual void MoveBackward() override;
    virtual void MoveForward() override;
    virtual void MoveLeft() override;
    virtual void MoveRight() override;
    virtual void MoveUp() override;
    virtual void MoveDown() override;

    static ObjectLoader ob;
    //static TinyWrapper TWr;
    static Mesh Batch(std::vector<Mesh>& meshes);

private:
    std::string m_OBJFilePath;
};

