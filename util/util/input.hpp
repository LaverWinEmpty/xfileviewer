#ifndef LWE__CTRL_CONST_HPP__
#define LWE__CTRL_CONST_HPP__

#include "../../d3d/d3d/inc/d3dx9math.h"
#include "../../common/common/common.hpp"

class InputUtil
{
    InputUtil() = delete;

public:
    static void SetWheelUp();
    static void SetWheelDown();

public:
    static void DragLBtn();
    static void DragRBtn();
    static void MouseWheel();

public:
    static void InitState();

public:
    static void Apply(OUT D3DXMATRIXA16* world, OUT D3DXMATRIXA16*);

private:
    /**
     * @brief model scale
     */
    static float scale;

    /**
     * @brief rotated angle
     */
    static D3DXVECTOR2 angleLast;

    /**
     * @brief rotate angle
     */
    static D3DXVECTOR2 angleCurr;

    /**
     * @brief moved postion
     */
    static D3DXVECTOR2 posLast;

    /**
     * @brief move position
     */
    static D3DXVECTOR2 posCurr;

public:
    /**
     * @brief mouse move speed
     */
    static const float DRAG_MOVE_SPEED;

    /**
     * @brief mouse wheel scale unit
     */
    static const float WHEEL_SIZE_UNIT;

    /**
     * @brief mouse last position
     */
    static POINT mouseLast;

    /**
     * @brief mouse click check
     */
    static bool isLBtnClick;

    /**
     * @brief mouse click check
     */
    static bool isRBtnClick;

    /**
     * @brief whell check
     */
    static bool isWheelUp;

    /**
     * @brief wheel check
     */
    static bool isWheelDown;
};

#include "input.ipp"
#endif