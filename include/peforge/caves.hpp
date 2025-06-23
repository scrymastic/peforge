#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Get the smallest cave in the PE that can fit the given size.
    * @param pe_rbuffer: The buffer of the raw PE file.
    * @param required_size: The required size of the cave.
    * @param cave_size: The size of the cave.
    * @return: The minimum cave.
    */
    BYTE* get_minimum_cave(IN const BYTE* pe_rbuffer, IN DWORD required_size, OUT DWORD* cave_size);
}