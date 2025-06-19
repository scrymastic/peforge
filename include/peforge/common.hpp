#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Convert a virtual address to a relative virtual address.
    * @param img_base: The base address of the image.
    * @param img_size: The size of the image.
    * @param va: The virtual address to convert.
    * @param rva: The relative virtual address.
    * @return: True if the conversion was successful, false otherwise.
    */
    bool va_to_rva(IN ULONGLONG img_base, IN DWORD img_size, IN ULONGLONG va, OUT DWORD &rva);
    
    /**
    * Check if the given field is within the given buffer.
    * @param buffer: The buffer to check.
    * @param buffer_size: The size of the buffer.
    * @param field: The field to check.
    * @param field_size: The size of the field.
    * @return: True if the field is within the buffer, false otherwise.
    */
    bool is_field_within_buffer(IN const BYTE* buffer, IN DWORD buffer_size, IN const BYTE* field, IN DWORD field_size);

}