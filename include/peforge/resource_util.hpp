#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Get the resource from the PE.
    * @param pe_buffer: The PE buffer.
    * @param res_id: The resource ID.
    * @param res_type: The resource type.
    * @param hInstance: The instance handle.
    * @return: The resource.
    */
    BYTE* get_resource(IN int res_id, IN LPSTR res_type, IN HMODULE hInstance, OUT DWORD* size);
}