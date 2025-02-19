#ifndef SORT_AUX_H
# define SORT_AUX_H

bool alphanumeric_compare(void *v1, int low, int high);
bool timestamp_compare(void *v1, int low, int high);
bool reverse_timestamp_compare(void *v1, int low, int high);
bool reverse_alphanumeric_compare(void *v1, int low, int high);

#endif //SORT_AUX