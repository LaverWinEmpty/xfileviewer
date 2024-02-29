#ifndef LWE__X_FILE_LOADER_COMMON_HPP__
#define LWE__X_FILE_LOADER_COMMON_HPP__

#include "../../d3d/d3d/inc/d3d9.h"
#include "../../d3d/d3d/inc/d3dx9math.h"
#include "macros.hpp"

struct Model
{
    ~Model()
    {
        SAFE_DELETES(materials);
        if(mesh) {
            for (DWORD i = 0; i < dwNumMaterials; ++i) {
                SAFE_RELEASE(textures[i]); 
            }
            SAFE_DELETES(textures);
        }
        SAFE_RELEASE(mesh);
    }

    LPD3DXMESH          mesh           = nullptr;
    D3DMATERIAL9*       materials      = nullptr;
    LPDIRECT3DTEXTURE9* textures       = nullptr;
    DWORD               dwNumMaterials = 0;
};

#endif