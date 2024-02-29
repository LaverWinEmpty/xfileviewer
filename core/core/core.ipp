#include "core.hpp"

void Core::HideConsole()
{
    auto console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);
}

void Core::Input(const float deltaTime)
{
    InputUtil::MouseWheel();
    InputUtil::DragLBtn();
    InputUtil::DragRBtn();
}

void Core::Render()
{
    const Model* const model = FileUtil::GetModelRef();
    if(!model) {
        return;
    }

    const LPDIRECT3DDEVICE9& device = DirectXUtil::Device();

    for(DWORD i = 0; i < model->dwNumMaterials; i++) {
        device->SetMaterial(&model->materials[i]);
        device->SetTexture(0, model->textures[i]);
        model->mesh->DrawSubset(i);
    }
}

void Core::Update(const float deltaTime)
{
    if(FileUtil::IsFileOpen()) {
        if(false == FileUtil::LoadModel(DirectXUtil::Device())) {
            FileUtil::SetFileClose();
            return;
        }
        InputUtil::InitState();
    }

    else {
        D3DXMATRIXA16 world, mat;
        InputUtil::Apply(&world, &mat);
        DirectXUtil::Device()->SetTransform(D3DTS_WORLD, &world);
    }
}

bool Core::Display(const float deltaTime)
{
    const LPDIRECT3DDEVICE9& device = DirectXUtil::Device();
    if(!device) {
        return false;
    }

    Input(deltaTime);
    Update(deltaTime);

    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, -1, 1.0f, 0);
    if(FAILED(device->BeginScene())) return false;

    Render();

    device->EndScene();
    device->Present(nullptr, nullptr, nullptr, nullptr);

    return true;
}

void Core::Cleanup()
{
    FileUtil::Release();
    DirectXUtil::Release();
}

int Core::Run()
{
    if(DirectXUtil::InitD3D(D3DDEVTYPE_HAL, 1280, 720, true) == false) {
        return MessageBox(nullptr, TEXT("Call InitD3D Fail"), TEXT("Error"), MB_OK);
    }
    DirectXUtil::Setup();
    int res = DirectXUtil::EnterMessageLoop(Display);
    Cleanup();
    return res;
}