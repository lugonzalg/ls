#include "system_file.h"
#include "libft.h"

bool alphanumeric_compare(void *v1, int low, int high) {

    t_system_file   **sysfile;
    int             diff;
    int             len1;
    int             len2;
    int             max_len;

    sysfile = v1;
    len1 = ft_strlen(sysfile[low]->name);
    len2 = ft_strlen(sysfile[high]->name);
    max_len = len1;
    if (max_len < len2)
        max_len = len2;

    diff = ft_strncmp(sysfile[low]->name, sysfile[high]->name, max_len);

    if (diff <= 0)
        return true;
    return false;

}

bool timestamp_compare(void *v1, int low, int high) {
    t_system_file **sysfile = v1;

    if (sysfile[low]->time_ref < sysfile[high]->time_ref)
        return false;
    
    if (sysfile[low]->time_ref > sysfile[high]->time_ref)
        return true;

    return alphanumeric_compare(v1, low, high);
}

bool reverse_timestamp_compare(void *v1, int low, int high) {
    return !timestamp_compare(v1, low, high);
}

bool reverse_alphanumeric_compare(void *v1, int low, int high) {
    return !alphanumeric_compare(v1, low, high);
}
