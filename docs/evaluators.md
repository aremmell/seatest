# Evaluators

seatest evaluators make writing tests less painful. They provide a straightforward yet comprehensive way to express what the results of your tests should be, in a granular fashion.

Each evaluator ultimately boils down to a truthy statement about the expression and data passed into it. At each step of your test, its state can be positively confirmed to either be correct or incorrect.

Any evaluator that results in a false expression will cause the active test to fail (*but not stop&mdash;it is on my TODO list to make that behavior configurable*). Finally, [additional helpers](./helpers.md) are provided to check the state of your test at any time, and optionally exit immediately. 

## General Purpose

|                    Evaluator | Expression                                                   |
| :--------------------------- | :----------------------------------------------------------- |
|        ST_EXPECT             | any truthy expression; if false, a warning is set for the active test (*no failure*) |
|          ST_REQUIRE          | any truthy expression; if false, an error is set for the active test (*failure*) |
|           ST_EQUAL           | lhs == rhs                                                   |
|                 ST_NOT_EQUAL | lhs != rhs                                                   |
|                 ST_LESS_THAN | lhs < rhs                                                    |
|        ST_LESS_THAN_OR_EQUAL | lhs <= rhs                                                   |
|              ST_GREATER_THAN | lhs > rhs                                                    |
|     ST_GREATER_THAN_OR_EQUAL | lhs >= rhs                                                   |
|                      ST_NULL | ptr == NULL                                                  |
|                  ST_NOT_NULL | ptr != NULL                                                  |

## Bitwise

|                    Evaluator | Expression                                                   |
| :--------------------------- | :----------------------------------------------------------- |
|                 ST_BITS_HIGH | (bitmask & bits) == bits                                     |
|                  ST_BITS_LOW | (bitmask & bits) == 0,                                       |
|             ST_BITWISE_EQUAL | memcmp(&lhs, &rhs, size) == 0                                |
|         ST_BITWISE_NOT_EQUAL | memcmp(&lhs, &rhs, size) != 0                                |
|            ST_BITWISE_ZEROED | all bytes == 0                                               |
|        ST_BITWISE_NOT_ZEROED | any byte != 0                                                |

## String

|                    Evaluator | Expression                                                   |
| :--------------------------- | :----------------------------------------------------------- |
|                 ST_STR_EQUAL | strcmp == 0                                                  |
|             ST_STR_NOT_EQUAL | strcmp != 0                                                  |
|               ST_STR_EQUAL_I | strcasecmp/StrCmpI == 0                                      |
|           ST_STR_NOT_EQUAL_I | strcasecmp/StrCmpI != 0                                      |
|              ST_STR_CONTAINS | strstr/StrStr != NULL                                        |
|          ST_STR_NOT_CONTAINS | strstr/StrStr == NULL                                        |
|            ST_STR_CONTAINS_I | strcasestr/StrStrI != NULL                                   |
|        ST_STR_NOT_CONTAINS_I | strcasestr/StrStrI == NULL                                   |
|            ST_STR_BEGINSWITH | strncmp/StrCmpN(needle_len) == 0                             |
|        ST_STR_NOT_BEGINSWITH | strncmp/StrCmpN(needle_len) != 0                             |
|          ST_STR_BEGINSWITH_I | strncasecmp/StrCmpNI(needle_len) == 0                        |
|      ST_STR_NOT_BEGINSWITH_I | strncasecmp/StrCmpNI(needle_len) != 0                        |
|     ST_STR_BEGINSWITH_WSPACE | isspace(str[0])                                              |
| ST_STR_NOT_BEGINSWITH_WSPACE | !isspace(str[0])                                             |
|              ST_STR_ENDSWITH | strncmp/StrCmpN(haystack + (haystack_len - needle_len), needle_len) == 0 |
|          ST_STR_NOT_ENDSWITH | strncmp/StrCmpN(haystack + (haystack_len - needle_len), needle_len) != 0 |
|            ST_STR_ENDSWITH_I | strncasecmp/StrCmpNI(haystack + (haystack_len - needle_len), needle_len) == 0 |
|        ST_STR_NOT_ENDSWITH_I | strncasecmp/StrCmpNI(haystack + (haystack_len - needle_len), needle_len) != 0 |
|       ST_STR_ENDSWITH_WSPACE | isspace(p[strlen(p) - 1])                                   |
|   ST_STR_NOT_ENDSWITH_WSPACE | !isspace(p[strlen(p) - 1])                                   |
|                 ST_STR_ALPHA | foreach(str) => isalpha                                      |
|               ST_STR_NUMERIC | foreach(str) => isdigit                                      |
|          ST_STR_ALPHANUMERIC | foreach(str) => isalnum                                      |

## Numeric

|                    Evaluator | Expression                                                   |
| :--------------------------- | :----------------------------------------------------------- |
|              ST_NUM_POSITIVE | num > 0                                                      |
|              ST_NUM_NEGATIVE | num < 0                                                      |
|                  ST_NUM_EVEN | num % 2 == 0                                                 |
|                   ST_NUM_ODD | num % 2 != 0                                                 |
|           ST_NUM_MULTIPLE_OF | num % exp == 0                                               |
|       ST_NUM_NOT_MULTIPLE_OF | num % exp != 0                                               |

## Array

|                    Evaluator | Expression                                                   |
| :--------------------------- | :----------------------------------------------------------- |
|               ST_ARRAY_EQUAL | sizeof(arr1[0]) == sizeof(arr2[0]) && count1 == count2 && all elems == |
|           ST_ARRAY_NOT_EQUAL | sizeof(arr1[0]) != sizeof(arr2[0])                           |
|            ST_ARRAY_CONTAINS | sizeof(arr[0]) == sizeof(val) && any elem == val             |
|        ST_ARRAY_NOT_CONTAINS | sizeof(arr[0]) != sizeof(val) && all elems != val            |
|              ST_ARRAY_UNIQUE | foreach(arr) => (n) foreach(arr) => (j) arr[n] != arr[j]     |

