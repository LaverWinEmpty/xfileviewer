#include "input.hpp"

const float InputUtil::DRAG_MOVE_SPEED = 0.005;
const float InputUtil::WHEEL_SIZE_UNIT = 0.1;

D3DXVECTOR2 InputUtil::angleLast = { 0, 0 };
D3DXVECTOR2 InputUtil::angleCurr = { 0, 0 };
D3DXVECTOR2 InputUtil::posLast   = { 0, 0 };
D3DXVECTOR2 InputUtil::posCurr   = { 0, 0 };
float       InputUtil::scale = 1;

POINT InputUtil::mouseLast;
bool  InputUtil::isLBtnClick = false;
bool  InputUtil::isRBtnClick = false;
bool  InputUtil::isWheelUp   = false;
bool  InputUtil::isWheelDown = false;

void InputUtil::SetWheelUp()
{
    isWheelUp   = true;
    isWheelDown = false;
}

void InputUtil::SetWheelDown()
{
    isWheelUp   = false;
    isWheelDown = true;
}

void InputUtil::DragLBtn()
{
    if(isLBtnClick == true) {
        return;
    }

    if(GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        if(isRBtnClick == false) {
            isRBtnClick = true;
            GetCursorPos(&mouseLast);
        }

        if(isRBtnClick) {
            POINT mouseCurr;
            GetCursorPos(&mouseCurr);
            angleCurr.x = angleLast.x + static_cast<float>(mouseLast.x - mouseCurr.x);
            angleCurr.y = angleLast.y + static_cast<float>(mouseLast.y - mouseCurr.y);
        }
    }

    else {
        isRBtnClick = false;
        GetCursorPos(&mouseLast);
        angleLast = angleCurr;
    }
}

void InputUtil::DragRBtn()
{
    if(isRBtnClick == true) {
        return;
    }

    if(GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        if(isLBtnClick == false) {
            isLBtnClick = true;
            GetCursorPos(&mouseLast);
        }

        if(isLBtnClick) {
            POINT mouseCurr;
            GetCursorPos(&mouseCurr);
            posCurr.x = (posLast.x + static_cast<float>(mouseCurr.x - mouseLast.x) * DRAG_MOVE_SPEED);
            posCurr.y = (posLast.y + static_cast<float>(mouseLast.y - mouseCurr.y) * DRAG_MOVE_SPEED);
        }
    }

    else {
        isLBtnClick = false;
        GetCursorPos(&mouseLast);
        posLast = posCurr;
    }
}

void InputUtil::MouseWheel()
{
    if(isWheelUp == true) {
        isWheelUp  = false;
        scale += WHEEL_SIZE_UNIT;
    }

    // 휠 다운
    if(isWheelDown == true) {
        isWheelDown  = false;
        if (scale > 0) {
            scale -= WHEEL_SIZE_UNIT;
        }
    }
}

void InputUtil::InitState()
{
    angleCurr = { 0, 0 };
    angleLast = { 0, 0 };
    posCurr   = { 0, 0 };
    posLast   = { 0, 0 };
    scale     = 1;
}

void InputUtil::Apply(D3DXMATRIXA16* world, D3DXMATRIXA16* mat)
{
    D3DXVECTOR2 vec = D3DXToRadian(angleCurr);

    D3DXMatrixRotationY(world, vec.x);
    D3DXMatrixRotationX(mat, vec.y);
    *world *= *mat;

    D3DXMatrixTranslation(mat, posCurr.x, posCurr.y, 0);
    *world *= *mat;

    D3DXMatrixScaling(mat, scale, scale, scale);
    *world *= *mat;
}