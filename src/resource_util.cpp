#include "../include/peforge/resource_util.hpp"

namespace peforge {
    BYTE* get_resource(IN int res_id, IN LPCWSTR res_type, IN HMODULE hInstance, OUT DWORD* size) {
        if (!res_id || !res_type || !hInstance || !size) return nullptr;

        // Get the resource from the PE
        HMODULE hModule = hInstance;
        HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(res_id), res_type);
        if (!hResource) return nullptr;

        // Load the resource
        HGLOBAL hGlobal = LoadResource(hModule, hResource);
        if (!hGlobal) return nullptr;

        // Get the size of the resource
        *size = SizeofResource(hModule, hResource);

        // Get the resource data
        BYTE* pRes = (BYTE*)LockResource(hGlobal);
        if (!pRes) return nullptr;

        return pRes;
    }
}