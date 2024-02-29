#ifndef LWE__CORE_HPP__
#define LWE__CORE_HPP__

#include "../../util/util/dx.hpp"
#include "../../util/util/file.hpp"
#include "../../util/util/input.hpp"

class Core
{
public:
    Core() = delete;

private:
    static void Input(IN const float);
    static void Update(IN const float);
    static bool Display(IN const float);
    static void Render();
    static void Cleanup();

public:
    static int Run();
};

#include "core.ipp"
#endif