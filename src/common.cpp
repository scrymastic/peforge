#include "../include/peforge/common.hpp"

namespace peforge {
    bool va_to_rva(IN ULONGLONG img_base, IN DWORD img_size, IN ULONGLONG va, OUT DWORD &rva) {
        if (!img_base || !va) return false;

        // If va is within the image, it is a VA
        if (va >= img_base && va < img_base + img_size) {
            rva = static_cast<DWORD>(va - img_base);
            return true;
        }

        // If va is less than the image base, it is an RVA
        if (va < img_base) {
            rva = static_cast<DWORD>(va);
            return true;
        }

        // Otherwise, it is not a valid address
        return false;
    }

    bool is_field_within_buffer(IN const BYTE* buffer, IN DWORD buffer_size, IN const BYTE* field, IN DWORD field_size) {
        if (!buffer || !field) return false;

        // Check if the field is within the buffer
        if (field >= buffer && field + field_size <= buffer + buffer_size) {
            return true;
        }

        return false;
    }
}