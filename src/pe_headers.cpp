#include "../include/peforge/pe_headers.hpp"

namespace peforge {

    BYTE *get_nt_headers(IN const BYTE* pe_buffer) {
        if (pe_buffer == nullptr) return nullptr;

        const IMAGE_DOS_HEADER* dos_hdr = reinterpret_cast<const IMAGE_DOS_HEADER*>(pe_buffer);
        if (dos_hdr->e_magic != IMAGE_DOS_SIGNATURE) return nullptr; // 'MZ'

        const IMAGE_NT_HEADERS* nt_hdrs = reinterpret_cast<const IMAGE_NT_HEADERS*>(pe_buffer + dos_hdr->e_lfanew);
        if (nt_hdrs->Signature != IMAGE_NT_SIGNATURE) return nullptr; // 'PE\0\0'

        return const_cast<BYTE*>(pe_buffer + dos_hdr->e_lfanew);
    }

    BYTE *get_file_header(IN const BYTE* pe_buffer) {
        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (nt_hdrs == nullptr) return nullptr;

        return const_cast<BYTE*>(nt_hdrs + sizeof(DWORD));
    }


    bool is_64bit(IN const BYTE* pe_buffer) {
        if (pe_buffer == nullptr) return false;

        const IMAGE_NT_HEADERS* nt_hdrs = reinterpret_cast<const IMAGE_NT_HEADERS*>(get_nt_headers(pe_buffer));
        if (nt_hdrs->Signature != IMAGE_NT_SIGNATURE) return false; // 'PE\0\0'

        WORD magic = nt_hdrs->OptionalHeader.Magic;
        return (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC); // 0x20B
    }

    size_t get_sections_count(IN const BYTE* pe_buffer) {
        if (pe_buffer == nullptr) return 0;

        const IMAGE_NT_HEADERS* nt_hdrs = reinterpret_cast<const IMAGE_NT_HEADERS*>(get_nt_headers(pe_buffer));

        return nt_hdrs->FileHeader.NumberOfSections;
    }

    DWORD get_image_size(IN const BYTE* pe_buffer) {
        BYTE* nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return 0;

        if (is_64bit(pe_buffer)) {
            const IMAGE_NT_HEADERS64* nt_hdrs64 = reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt_hdrs);
            return nt_hdrs64->OptionalHeader.SizeOfImage;
        } else {
            const IMAGE_NT_HEADERS32* nt_hdrs32 = reinterpret_cast<const IMAGE_NT_HEADERS32*>(nt_hdrs);
            return nt_hdrs32->OptionalHeader.SizeOfImage;
        }
    }

    BYTE* get_directory_of_type(IN const BYTE* pe_buffer, IN DWORD dir_id) {
        if (dir_id >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES) return nullptr;

        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return nullptr;

        if (is_64bit(pe_buffer)) {
            const IMAGE_NT_HEADERS64* nt_hdrs64 = reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt_hdrs);
            return const_cast<BYTE*>(pe_buffer + nt_hdrs64->OptionalHeader.DataDirectory[dir_id].VirtualAddress);
        } else {
            const IMAGE_NT_HEADERS32* nt_hdrs32 = reinterpret_cast<const IMAGE_NT_HEADERS32*>(nt_hdrs);
            return const_cast<BYTE*>(pe_buffer + nt_hdrs32->OptionalHeader.DataDirectory[dir_id].VirtualAddress);
        }
    }

    bool update_entry_point(IN OUT BYTE *pe_buffer, IN DWORD new_ep) {
        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (nt_hdrs == nullptr) return false;

        if (is_64bit(pe_buffer)) {
            IMAGE_NT_HEADERS64* nt_hdrs64 = reinterpret_cast<IMAGE_NT_HEADERS64*>(nt_hdrs);
            nt_hdrs64->OptionalHeader.AddressOfEntryPoint = new_ep;
        } else {
            IMAGE_NT_HEADERS32* nt_hdrs32 = reinterpret_cast<IMAGE_NT_HEADERS32*>(nt_hdrs);
            nt_hdrs32->OptionalHeader.AddressOfEntryPoint = new_ep;
        }

        return true;
    }
}