#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Get the resource from the PE.
    * @param hInstance: The instance handle.
    * @param res_id: The resource ID.
    * @param res_type: The resource type.
    * @param size: The size of the resource.
    * @return: The resource.
    */
    BYTE* get_resource(IN HMODULE hInstance, IN int res_id, IN LPSTR res_type, OUT DWORD* size);
}