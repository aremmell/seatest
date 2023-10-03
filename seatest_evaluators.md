| Evaluator                 | Expression                                                                          |
|--------------------------:|:------------------------------------------------------------------------------------|
| ST_EXPECT                 | any truthy expression; if false, sets a warning on the active test (test pass)      |
| ST_REQUIRE                | any truthy expression; if false, adds an error to the active test (test fail)       |
| ST_EQUAL                  | lhs == rhs                                                                          |
| ST_NOT_EQUAL              | lhs != rhs                                                                          |
| ST_LESS_THAN              | lhs < rhs                                                                           |
| ST_LESS_THAN_OR_EQUAL     | lhs <= rhs                                                                          |
| ST_GREATER_THAN           | lhs > rhs                                                                           |
| ST_GREATER_THAN_OR_EQUAL  | lhs >= rhs                                                                          |
| ST_NULL                   | ptr == NULL                                                                         |
| ST_NOT_NULL               | ptr != NULL                                                                         |
| ----- bitwise -----       |                                                                                     |
| ST_BITS_HIGH              | (bitmask & bits) == bits                                                            |
| ST_BITS_LOW               | (bitmask & bits) == 0,                                                              |
| ST_BITWISE_EQUAL          | memcmp(&lhs, &rhs, size) == 0                                                       |
| ST_BITWISE_NOT_EQUAL      | memcmp(&lhs, &rhs, size) != 0                                                       |
| ST_BITWISE_ZEROED         | all bytes == 0                                                                      |
| ST_BITWISE_NOT_ZEROED     | any byte != 0                                                                       |
| ----- string -----        |                                                                                     |
| ST_STR_EQUAL              | strcmp == 0                                                                         |
| ST_STR_NOT_EQUAL          | strcmp != 0                                                                         |
| ST_STR_EQUAL_I            | strcasecmp/StrCmpI == 0                                                             |
| ST_STR_NOT_EQUAL_I        | strcasecmp/StrCmpI != 0                                                             |
| ST_STR_CONTAINS           | strstr/StrStr != NULL                                                               |
| ST_STR_NOT_CONTAINS       | strstr/StrStr == NULL                                                               |
| ST_STR_CONTAINS_I         | strcasestr/StrStrI != NULL                                                          |
| ST_STR_NOT_CONTAINS_I     | strcasestr/StrStrI == NULL                                                          |
| ST_STR_BEGINSWITH         | strncmp/StrCmpN(needle_len) == 0                                                    |
| ST_STR_NOT_BEGINSWITH     | strncmp/StrCmpN(needle_len) != 0                                                    |
| ST_STR_BEGINSWITH_I       | strncasecmp/StrCmpNI(needle_len) == 0                                               |
| ST_STR_NOT_BEGINSWITH_I   | strncasecmp/StrCmpNI(needle_len) != 0                                               |
| ST_STR_ENDSWITH           | strncmp/StrCmpN(haystack + (haystack_len - needle_len), needle_len) == 0            |
| ST_STR_NOT_ENDSWITH       | strncmp/StrCmpN(haystack + (haystack_len - needle_len), needle_len) != 0            |
| ST_STR_ENDSWITH_I         | strncasecmp/StrCmpNI(haystack + (haystack_len - needle_len), needle_len) == 0       |
| ST_STR_NOT_ENDSWITH_I     | strncasecmp/StrCmpNI(haystack + (haystack_len - needle_len), needle_len) != 0       |
| ST_STR_ALPHA              | foreach(str) => isalpha                                                             |
| ST_STR_NUMERIC            | foreach(str) => isdigit                                                             |
| ST_STR_ALPHANUMERIC       | foreach(str) => isalnum                                                             |
| ----- numeric -----       |                                                                                     |
| ST_NUM_POSITIVE           | num > 0                                                                             |
| ST_NUM_NEGATIVE           | num < 0                                                                             |
| ST_NUM_EVEN               | num % 2 == 0                                                                        |
| ST_NUM_ODD                | num % 2 != 0                                                                        |
| ST_NUM_MULTIPLE_OF        | num % exp == 0                                                                      |
| ST_NUM_NOT_MULTIPLE_OF    | num % exp != 0                                                                      |
| ----- array -----         |                                                                                     |
| ST_ARRAY_EQUAL            | sizeof(arr1[0]) == sizeof(arr2[0]) && count1 == count2 && all elems ==              |
| ST_ARRAY_NOT_EQUAL        | sizeof(arr1[0]) != sizeof(arr2[0]) || count1 != count2 || any elem !=               |
| ST_ARRAY_CONTAINS         | sizeof(arr[0]) == sizeof(val) && any elem == val                                    |
| ST_ARRAY_NOT_CONTAINS     | sizeof(arr[0]) != sizeof(val) && all elems != val                                   |
| ST_ARRAY_UNIQUE           | foreach(arr) => (n) foreach(arr) => (j) arr[n] != arr[j]

