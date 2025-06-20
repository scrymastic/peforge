#include "../include/peforge/pe_headers.hpp"

namespace peforge {

    IMAGE_DOS_HEADER *get_dos_header(IN const BYTE* pe_buffer) {
        if (!pe_buffer) return nullptr;

        const IMAGE_DOS_HEADER* dos_hdr = reinterpret_cast<const IMAGE_DOS_HEADER*>(pe_buffer);
        if (dos_hdr->e_magic != IMAGE_DOS_SIGNATURE) return nullptr; // 'MZ'

        return const_cast<IMAGE_DOS_HEADER*>(dos_hdr);
    }

    BYTE *get_nt_headers(IN const BYTE* pe_buffer) {
        if (!pe_buffer) return nullptr;

        const IMAGE_DOS_HEADER* dos_hdr = get_dos_header(pe_buffer);
        if (!dos_hdr) return nullptr;

        const IMAGE_NT_HEADERS* nt_hdrs = reinterpret_cast<const IMAGE_NT_HEADERS*>(pe_buffer + dos_hdr->e_lfanew);
        if (!nt_hdrs) return nullptr;

        if (nt_hdrs->Signature != IMAGE_NT_SIGNATURE) return nullptr; // 'PE\0\0'

        return const_cast<BYTE*>(pe_buffer + dos_hdr->e_lfanew);
    }

    IMAGE_FILE_HEADER *get_file_header(IN const BYTE* pe_buffer) {
        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return nullptr;

        return const_cast<IMAGE_FILE_HEADER*>(reinterpret_cast<const IMAGE_FILE_HEADER*>(nt_hdrs + sizeof(DWORD)));
    }

    BYTE *get_optional_header(IN const BYTE* pe_buffer) {
        IMAGE_FILE_HEADER *file_hdr = get_file_header(pe_buffer);
        if (!file_hdr) return nullptr;

        return const_cast<BYTE*>(reinterpret_cast<const BYTE*>(file_hdr + sizeof(IMAGE_FILE_HEADER)));
    }

    IMAGE_SECTION_HEADER *get_section_header(IN const BYTE* pe_buffer, IN DWORD section_idx) {
        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return nullptr;

        DWORD optional_header_size = is_64bit(pe_buffer) ? sizeof(IMAGE_OPTIONAL_HEADER64) : sizeof(IMAGE_OPTIONAL_HEADER32);

        DWORD section_offset = sizeof(DWORD) + // NT signature
                              sizeof(IMAGE_FILE_HEADER) + // File header
                              optional_header_size + // Optional header (size varies)
                              (section_idx * sizeof(IMAGE_SECTION_HEADER)); // Section headers

        return const_cast<IMAGE_SECTION_HEADER*>(reinterpret_cast<const IMAGE_SECTION_HEADER*>(nt_hdrs + section_offset));
    }

    bool is_64bit(IN const BYTE* pe_buffer) {
        const IMAGE_NT_HEADERS* nt_hdrs = reinterpret_cast<const IMAGE_NT_HEADERS*>(get_nt_headers(pe_buffer));
        if (!nt_hdrs) return false;

        WORD magic = nt_hdrs->OptionalHeader.Magic;
        return (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC); // 0x20B
    }

    size_t get_sections_count(IN const BYTE* pe_buffer) {
        const IMAGE_NT_HEADERS* nt_hdrs = reinterpret_cast<const IMAGE_NT_HEADERS*>(get_nt_headers(pe_buffer));
        if (!nt_hdrs) return 0;

        return nt_hdrs->FileHeader.NumberOfSections;
    }

    DWORD get_image_size(IN const BYTE* pe_buffer) {
        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return 0;

        if (is_64bit(pe_buffer)) {
            const IMAGE_NT_HEADERS64* nt_hdrs64 = reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt_hdrs);
            return nt_hdrs64->OptionalHeader.SizeOfImage;
        } else {
            const IMAGE_NT_HEADERS32* nt_hdrs32 = reinterpret_cast<const IMAGE_NT_HEADERS32*>(nt_hdrs);
            return nt_hdrs32->OptionalHeader.SizeOfImage;
        }
    }

    IMAGE_DATA_DIRECTORY *get_directory_entry(IN const BYTE* pe_buffer, IN DWORD dir_id) {
        if (dir_id >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES) return nullptr;

        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return nullptr;

        if (is_64bit(pe_buffer)) {
            const IMAGE_NT_HEADERS64* nt_hdrs64 = reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt_hdrs);
            return const_cast<IMAGE_DATA_DIRECTORY*>(&nt_hdrs64->OptionalHeader.DataDirectory[dir_id]);
        } else {
            const IMAGE_NT_HEADERS32* nt_hdrs32 = reinterpret_cast<const IMAGE_NT_HEADERS32*>(nt_hdrs);
            return const_cast<IMAGE_DATA_DIRECTORY*>(&nt_hdrs32->OptionalHeader.DataDirectory[dir_id]);
        }
    }

    DWORD get_entry_point(IN const BYTE* pe_buffer) {
        BYTE *nt_hdrs = get_nt_headers(pe_buffer);
        if (!nt_hdrs) return 0;

        if (is_64bit(pe_buffer)) {
            const IMAGE_NT_HEADERS64* nt_hdrs64 = reinterpret_cast<const IMAGE_NT_HEADERS64*>(nt_hdrs);
            return nt_hdrs64->OptionalHeader.AddressOfEntryPoint;
        } else {
            const IMAGE_NT_HEADERS32* nt_hdrs32 = reinterpret_cast<const IMAGE_NT_HEADERS32*>(nt_hdrs);
            return nt_hdrs32->OptionalHeader.AddressOfEntryPoint;
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