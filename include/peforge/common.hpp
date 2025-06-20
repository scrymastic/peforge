#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Convert a virtual address to a relative virtual address.
    * @param pe_buffer: The base address of the image.
    * @param pe_vsize: The virtual size of the image.
    * @param va: The virtual address to convert.
    * @param rva: The relative virtual address.
    * @return: True if the conversion was successful, false otherwise.
    */
    bool va_to_rva(IN const BYTE* pe_buffer, IN DWORD pe_vsize, IN const BYTE* va, OUT DWORD &rva);

    /**
     * Convert offset to rva.
     * @param pe_buffer: The base address of the image.
     * @param pe_size: The size of the image.
     * @param offset: The offset to convert.
     * @param rva: The relative virtual address.
     * @return: True if the conversion was successful, false otherwise.
     */
    bool offset_to_rva(IN const BYTE* pe_buffer, IN DWORD pe_size, IN DWORD offset, OUT DWORD &rva);

    /**
     * Convert rva to offset.
     * @param pe_buffer: The base address of the image.
     * @param pe_vsize: The virtual size of the image.
     * @param rva: The relative virtual address to convert.
     * @param offset: The offset.
     * @return: True if the conversion was successful, false otherwise.
     */
    bool rva_to_offset(IN const BYTE* pe_buffer, IN DWORD pe_vsize, IN DWORD rva, OUT DWORD &offset);
    
    /**
    * Check if the given field is within the given buffer.
    * @param buffer: The buffer to check.
    * @param buffer_size: The size of the buffer.
    * @param ptr: The pointer to check.
    * @param ptr_size: The size of the pointer.
    * @return: True if the pointer is within the buffer, false otherwise.
    */
    bool is_ptr_valid(IN const BYTE* buffer, IN DWORD buffer_size, IN const BYTE* ptr, IN DWORD ptr_size);

}