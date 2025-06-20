#include "../include/peforge/common.hpp"
#include "../include/peforge/pe_headers.hpp"

namespace peforge {
    bool va_to_rva(IN const BYTE* pe_buffer, IN DWORD pe_vsize, IN const BYTE* va, OUT DWORD &rva) {
        if (!pe_buffer || !va || !is_ptr_valid(pe_buffer, pe_vsize, va, 1)) return false;

        rva = static_cast<DWORD>(va - pe_buffer);
        return true;
    }

    bool offset_to_rva(IN const BYTE* pe_buffer, IN DWORD pe_size, IN DWORD offset, OUT DWORD &rva)
    {
        size_t num_sections = peforge::get_sections_count(pe_buffer);
        for (size_t i = 0; i < num_sections; ++i) {
            IMAGE_SECTION_HEADER* section = peforge::get_section_header(pe_buffer, i);
            if (!section) continue;
            DWORD ptr_raw = section->PointerToRawData;
            DWORD size_raw = section->SizeOfRawData;
            DWORD va = section->VirtualAddress;

            if (offset >= ptr_raw && offset < ptr_raw + size_raw) {
                DWORD delta = offset - ptr_raw;
                rva = va + delta;
                return true;
            }
        }
        // Not found
        return false;
    }

    bool rva_to_offset(IN const BYTE* pe_buffer, IN DWORD pe_vsize, IN DWORD rva, OUT DWORD &offset)
    {
        for (WORD i = 0; i < peforge::get_sections_count(pe_buffer); ++i) {
            IMAGE_SECTION_HEADER* section = peforge::get_section_header(pe_buffer, i);
            if (!section) continue;
            DWORD sec_rva = section->VirtualAddress;
            DWORD sec_vsize = section->Misc.VirtualSize;
            DWORD sec_raw_ptr = section->PointerToRawData;
            DWORD sec_raw_size = section->SizeOfRawData;

            // Check if the RVA is within this section
            if (rva >= sec_rva && rva < sec_rva + sec_vsize) {
                DWORD delta = rva - sec_rva;
                // Make sure we don't go past the file's data
                if (delta >= sec_raw_size) return false;
                offset = sec_raw_ptr + delta;
                return true;
            }
        }

        // If not found in any section, check if it's in headers
        if (peforge::is_64bit(pe_buffer)) {
            if (rva < reinterpret_cast<const IMAGE_OPTIONAL_HEADER64*>(peforge::get_optional_header(pe_buffer))->SizeOfHeaders) {
                offset = rva;
                return true;
            }
        } else {
            if (rva < reinterpret_cast<const IMAGE_OPTIONAL_HEADER32*>(peforge::get_optional_header(pe_buffer))->SizeOfHeaders) {
                offset = rva;
                return true;
            }
        }

        return false; // RVA does not map to any section or header
    }

    bool is_ptr_valid(IN const BYTE* buffer, IN DWORD buffer_size, IN const BYTE* ptr, IN DWORD ptr_size) {
        if (!buffer || !ptr) return false;

        // Check if the ptr is within the buffer
        if (ptr >= buffer && ptr + ptr_size <= buffer + buffer_size) {
            return true;
        }

        return false;
    }
}