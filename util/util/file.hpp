#ifndef LWE__FILE_HPP__
#define LWE__FILE_HPP__

#include "stdexcept"
#include "string"
#include "shobjidl.h"
#include "../../common/common/common.hpp"
#include "../../common/common/macros.hpp"

using tstring = std::basic_string<TCHAR>;

class FileUtil
{
public:
    static bool IsFileOpen();
    static void SetFileOpen();
    static void SetFileClose();

public:
    static bool    LoadModel(IN const LPDIRECT3DDEVICE9&);
    static tstring FileOpnGUI(IN const COMDLG_FILTERSPEC*, IN size_t);
    static tstring GetPairedTextureDirectory(IN const tstring& xFileAddress, IN const tstring& textureFileAddress);
    static void    Release();

public:
    static const Model* const GetModelRef();

private:
    /**
     * @brief loading model
     */
    static Model* model;

private:
    /**
     * @brief check file open
     */
    static bool isFileOpen;

public:
    static constexpr COMDLG_FILTERSPEC EXTENSION_X_FILE[] = {
        {L"Model files", L"*.x"}
    };

    static constexpr COMDLG_FILTERSPEC EXTENSION_TEXTURE_FILE[] = {
        {L"Texture files", L"*.dds"},
        {    L"Bmp files", L"*.bmp"},
        {       L"others",     L"*"}
    };
};

#include "file.ipp"
#endif