#include <stddef.h>
#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src) {
    if (dst == 0 || dst_size == 0) {
        return;
    }
    if (src == 0) {
        dst[0] = '\0';
        return;
    }
    (void)snprintf(dst, dst_size, "%s", src);
}

int yai_string_contains(const char *haystack, const char *needle) {
    if (haystack == 0 || needle == 0) {
        return 0;
    }
    return strstr(haystack, needle) != 0;
}
