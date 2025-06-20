#pragma once

#include <windows.h>

namespace peforge {
    /**
    * Get the DOS header from the PE buffer.
    * @param pe_buffer: The PE buffer.
    * @return: The DOS header.
    */
    IMAGE_DOS_HEADER *get_dos_header(IN const BYTE* pe_buffer);

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
    IMAGE_FILE_HEADER *get_file_header(IN const BYTE* pe_buffer);

    /**
    * Get the optional header from the PE buffer.
    * @param pe_buffer: The PE buffer.
    * @return: The optional header.
    */
    BYTE *get_optional_header(IN const BYTE* pe_buffer);

    /**
    * Get the section header from the PE buffer.
    * @param pe_buffer: The PE buffer.
    * @param section_idx: The index of the section.
    * @param is_raw: If true, use raw offset, otherwise use virtual address.
    * @return: The section header.
    */
    IMAGE_SECTION_HEADER *get_section_header(IN const BYTE* pe_buffer, IN DWORD section_idx);

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
    * Get the directory entry of a given type from the PE.
    * @param pe_buffer: The PE buffer.
    * @param dir_id: The type of directory to get.
    * @return: The directory entry.
    */
    IMAGE_DATA_DIRECTORY *get_directory_entry(IN const BYTE* pe_buffer, IN DWORD dir_id);

    /**
     * Get the entry point of the PE.
     * @param pe_buffer: The PE buffer.
     * @return: The entry point.
     */
    DWORD get_entry_point(IN const BYTE* pe_buffer);

    /**
    * Update the entry point of the PE.
    * @param pe_buffer: The PE buffer.
    * @param new_ep: The new entry point.
    * @return: True if the update was successful, false otherwise.
    */
    bool update_entry_point(IN OUT BYTE *pe_buffer, IN DWORD new_ep);
}