#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Get the NT headers from the PE buffer.
    * @param pe_buffer: The PE buffer.
    * @return: The NT headers.
    */
    BYTE *get_nt_headers(IN const BYTE* pe_buffer);

    /**
    * Get the file header from the PE buffer.
    * @param pe_buffer: The PE buffer.
    * @return: The file header.
    */
    BYTE *get_file_header(IN const BYTE* pe_buffer);

    /**
    * Check if the PE is 64-bit.
    * @param pe_buffer: The PE buffer.
    * @return: True if the PE is 64-bit, false otherwise.
    */
    bool is_64bit(IN const BYTE* pe_buffer);

    /**
    * Get the number of sections in the PE.
    * @param pe_buffer: The PE buffer.
    * @return: The number of sections.
    */
    size_t get_sections_count(IN const BYTE* pe_buffer);

    /**
    * Get the image size from the PE.
    * @param pe_buffer: The PE buffer.
    * @return: The image size.
    */
    DWORD get_image_size(IN const BYTE* pe_buffer);

    /**
    * Get the directory of a given type from the PE.
    * @param pe_buffer: The PE buffer.
    * @param dir_id: The type of directory to get.
    * @return: The directory.
    */
    BYTE* get_directory_of_type(IN const BYTE* pe_buffer, IN DWORD dir_id);

    /**
    * Update the entry point of the PE.
    * @param pe_buffer: The PE buffer.
    * @param new_ep: The new entry point.
    * @return: True if the update was successful, false otherwise.
    */
    bool update_entry_point(IN OUT BYTE *pe_buffer, IN DWORD new_ep);
}