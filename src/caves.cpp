#include "../include/peforge/caves.hpp"
#include "../include/peforge/pe_headers.hpp"

namespace peforge {
    BYTE* get_minimum_cave(IN const BYTE* pe_buffer, IN DWORD required_size, OUT DWORD* cave_size) {
        *cave_size = 0;
        IMAGE_FILE_HEADER *file_header = get_file_header(pe_buffer);
        if (file_header == nullptr) return nullptr;

        // Section headers start after NT headers
        size_t opt_header_size = is_64bit(pe_buffer) ? sizeof(IMAGE_OPTIONAL_HEADER64) : sizeof(IMAGE_OPTIONAL_HEADER32);
        const IMAGE_SECTION_HEADER* sections = reinterpret_cast<const IMAGE_SECTION_HEADER*>(
            reinterpret_cast<const BYTE*>(file_header) + sizeof(IMAGE_FILE_HEADER) + opt_header_size);

        DWORD min_cave_size = ULONG_MAX;
        BYTE* min_cave_ptr = nullptr;

        for (WORD i = 0; i < get_sections_count(pe_buffer); ++i) {
            const IMAGE_SECTION_HEADER& sh = sections[i];

            // Only scan sections that are big enough and actually present in file
            if (sh.SizeOfRawData == 0 || sh.PointerToRawData == 0) continue;
            const BYTE* sect = pe_buffer + sh.PointerToRawData;
            DWORD sect_size = sh.SizeOfRawData;

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
                    }
                } else {
                    cur_idx++;
                }
            }
        }

        if (min_cave_ptr && min_cave_size != ULONG_MAX) {
            *cave_size = min_cave_size;
            return min_cave_ptr;
        }

        return nullptr;
    }

}