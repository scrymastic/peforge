#include "../include/peforge/caves.hpp"
#include "../include/peforge/pe_headers.hpp"

namespace peforge {
    BYTE* get_minimum_cave(IN const BYTE* pe_buffer, IN DWORD required_size, OUT DWORD* cave_size, IN DWORD characteristics) {
        *cave_size = 0;
        DWORD min_cave_size = ULONG_MAX;
        BYTE* min_cave_ptr = nullptr;
        DWORD min_cave_section_idx = 0;

        for (WORD i = 0; i < get_sections_count(pe_buffer); ++i) {
            IMAGE_SECTION_HEADER* section = get_section_header(pe_buffer, i);
            if (!section) continue;

            // Only scan sections that are big enough and actually present in file
            if (section->SizeOfRawData == 0 || section->PointerToRawData == 0) continue;
            
            // Check if section has the required characteristics or more
            if (characteristics != 0 && (section->Characteristics & characteristics) != characteristics) continue;
            const BYTE* sect = pe_buffer + section->PointerToRawData;
            DWORD sect_size = section->SizeOfRawData;

            DWORD cur_idx = 0;
            while (cur_idx < sect_size) {
                // Start cave scan for both 0x00 and 0xCC
                if (sect[cur_idx] == 0x00 || sect[cur_idx] == 0xCC) {
                    BYTE cave_byte = sect[cur_idx];
                    DWORD start = cur_idx;
                    while (cur_idx < sect_size && sect[cur_idx] == cave_byte) cur_idx++;
                    DWORD len = cur_idx - start;
                    if (len >= required_size && len < min_cave_size) {
                        min_cave_size = len;
                        min_cave_ptr = (BYTE*)(sect + start);
                        min_cave_section_idx = i;
                    }
                } else cur_idx++;
            }
        }

        if (min_cave_ptr && min_cave_size != ULONG_MAX) {
            *cave_size = min_cave_size;
            
            // Check if we need to update the VirtualSize of the section
            IMAGE_SECTION_HEADER* section = get_section_header(pe_buffer, min_cave_section_idx);
            if (!section) return nullptr;
            DWORD cave_offset = static_cast<DWORD>(min_cave_ptr - (pe_buffer + section->PointerToRawData));
            DWORD cave_end_offset = cave_offset + required_size;
            
            // If the cave extends beyond the current VirtualSize, update it
            if (cave_end_offset > section->Misc.VirtualSize) {
                // Get section alignment from optional header
                DWORD section_alignment = 0;
                if (is_64bit(pe_buffer)) {
                    section_alignment = reinterpret_cast<const IMAGE_NT_HEADERS64*>(pe_buffer)->OptionalHeader.SectionAlignment;
                } else {
                    section_alignment = reinterpret_cast<const IMAGE_NT_HEADERS32*>(pe_buffer)->OptionalHeader.SectionAlignment;
                }
                
                DWORD new_size = (cave_end_offset + section_alignment - 1) & ~(section_alignment - 1);
                section->Misc.VirtualSize = new_size;
            }
            
            return min_cave_ptr;
        }

        return nullptr;
    }

}