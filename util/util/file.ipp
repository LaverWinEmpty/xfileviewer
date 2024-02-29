#include "file.hpp"

bool FileUtil::isFileOpen = false;

Model* FileUtil::model = nullptr;

bool FileUtil::IsFileOpen()
{
    return isFileOpen;
}

void FileUtil::SetFileOpen()
{
    isFileOpen = true;
}

void FileUtil::SetFileClose()
{
    isFileOpen = false;
}

bool FileUtil::LoadModel(const LPDIRECT3DDEVICE9& device)
{
    Model* newModel = new Model;

    tstring xFileAddr = FileOpnGUI(EXTENSION_X_FILE, sizeof(EXTENSION_X_FILE) / sizeof(COMDLG_FILTERSPEC));
    if (xFileAddr.compare(TEXT("")) == 0) {
        return false;
    }

    LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;
    isFileOpen                   = false;

    if(FAILED(D3DXLoadMeshFromX(xFileAddr.c_str(),
                                D3DXMESH_SYSTEMMEM,
                                device,
                                NULL,
                                &pD3DXMtrlBuffer,
                                NULL,
                                &newModel->dwNumMaterials,
                                &newModel->mesh))) {
        MessageBox(nullptr, TEXT("File Error"), TEXT("FAIL"), MB_OK);
        SAFE_RELEASE(pD3DXMtrlBuffer);
        SAFE_DELETE(newModel);
        return true;
    }

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    newModel->materials            = new D3DMATERIAL9[newModel->dwNumMaterials];
    if(newModel->materials == NULL) {
        MessageBox(nullptr, TEXT("Materials Error"), TEXT("FAIL"), MB_OK);
        SAFE_RELEASE(pD3DXMtrlBuffer);
        SAFE_DELETE(newModel);
        return true;
    }

    newModel->textures = new LPDIRECT3DTEXTURE9[newModel->dwNumMaterials]{ NULL };
    if(newModel->mesh == NULL) {
        MessageBox(nullptr, TEXT("Mesh Error"), TEXT("FAIL"), MB_OK);
        SAFE_RELEASE(pD3DXMtrlBuffer);
        SAFE_DELETE(newModel);
        return true;
    }

    for(DWORD i = 0; i < newModel->dwNumMaterials; i++) {
        newModel->materials[i]         = d3dxMaterials[i].MatD3D;
        newModel->materials[i].Ambient = newModel->materials[i].Diffuse;
        newModel->textures[i];

        if(d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0) {
            tstring textureAddr;
            try {
                textureAddr = GetPairedTextureDirectory(xFileAddr, TEXT("dds"));
            }
            catch(const std::runtime_error& e) {
                MessageBox(nullptr, TEXT("Texture Loading Failed"), TEXT("The File Name Is Too Long."), MB_OK);
            }

            // select texture file: if a texture file with the same name cannot be found at the same directory.
            // ends upon cancellation
            if(FAILED(D3DXCreateTextureFromFile(device, textureAddr.c_str(), &newModel->textures[i]))) {
                textureAddr =
                    FileOpnGUI(FileUtil::EXTENSION_TEXTURE_FILE,
                               sizeof(FileUtil::EXTENSION_TEXTURE_FILE) / sizeof(COMDLG_FILTERSPEC)); // select

                // cancel when windows closes
                if(textureAddr.compare(TEXT("")) == 0) {
                    SAFE_RELEASE(pD3DXMtrlBuffer);
                    SAFE_DELETE(newModel);
                    return false;
                }

                else if(FAILED(D3DXCreateTextureFromFile(device, textureAddr.c_str(), &newModel->textures[i]))) {
                    MessageBox(nullptr, TEXT("Model Loading Cancel"), TEXT("FAIL"), MB_OK);
                    SAFE_RELEASE(pD3DXMtrlBuffer);
                    SAFE_DELETE(newModel);
                    return true;
                }
            }
        }
    }

    delete model;
    model = newModel;


    SAFE_RELEASE(pD3DXMtrlBuffer);
    return true;
}

tstring FileUtil::FileOpnGUI(const COMDLG_FILTERSPEC* extensionList, size_t listSize)
{
    IFileOpenDialog* pFileOpen   = nullptr;
    IShellItem*      pItem       = nullptr;
    PWSTR            pszFilePath = nullptr;
    tstring          address     = TEXT("");

    if(FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
        MessageBox(nullptr, TEXT("CoInitializeEx()"), TEXT("FAIL"), MB_OK);
        return address;
    }

    else if(FAILED(CoCreateInstance(CLSID_FileOpenDialog,
                                    NULL,
                                    CLSCTX_ALL,
                                    IID_IFileOpenDialog,
                                    reinterpret_cast<void**>(&pFileOpen)))) {
        MessageBox(nullptr, TEXT("CoCreateInstance()"), TEXT("FAIL"), MB_OK);
    }

    else if(FAILED(pFileOpen->SetFileTypes(listSize, extensionList))) {
        MessageBox(nullptr, TEXT("SetFileTypes()"), TEXT("FAIL"), MB_OK);
    }

    else if(FAILED(pFileOpen->Show(NULL))) {
        return address;
    }

    else if(FAILED(pFileOpen->GetResult(&pItem))) {
        MessageBox(nullptr, TEXT("GetResult()"), TEXT("FAIL"), MB_OK);
    }

    else if(FAILED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
        MessageBox(nullptr, TEXT("GetDisplayName()"), TEXT("FAIL"), MB_OK);
    }

    // succeed
    else {
        address = pszFilePath;
        CoTaskMemFree(pszFilePath);
    }

    SAFE_RELEASE(pItem);
    SAFE_RELEASE(pFileOpen);
    CoUninitialize();

    return address;
}

tstring FileUtil::GetPairedTextureDirectory(IN const tstring& xFileAddr, IN const tstring& textureExtension)
{
    tstring result = xFileAddr; // copy

    size_t extensionSize  = textureExtension.size();
    size_t textureAddrLen = xFileAddr.size() + extensionSize - 1;

    // check
    if(textureAddrLen > MAX_PATH) {
        throw std::runtime_error("The File Name Is Too Long.");
    }
    result.resize(textureAddrLen);

    // copy
    for(int i = 0; i < extensionSize; ++i) {
        result[textureAddrLen - extensionSize + i] = textureExtension[i];
    }

    return result;
}

void FileUtil::Release()
{
    SAFE_DELETE(model);
}

const Model* const FileUtil::GetModelRef()
{
    return model;
}