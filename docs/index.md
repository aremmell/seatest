# Documentation

## Evaluators

seatest evaluators make writing tests less painful. They provide a straightforward yet verbose way to express what the results of your tests should be&mdash;in a granular fashion. Each evaluator ultimately boils down to a truthy statement about the expression and/or data passed into it. At each step of your test, its state can be positively confirmed to either be correct or incorrect.

If an evaluator represents a false expression, an error message is emitted which includes the evaluator's name, the line of code at which the expression resides, and the expression that evaluated to false.

Upon the failure of any evaulator (*with the exception of ST_EXPECT*), the test is marked as failed.

### Special

|                    Evaluator | Expression                   |
| :--------------------------- | :--------------------------- |
| ST_EXPECT                    | Any truthy expression. Unlike all other evaluators, marks the test with a warning instead of an error if the expression passed to it evaluates to false |

### General Purpose

|                    Evaluator | Expression                   |
| :--------------------------- | :--------------------------- |
|                      ST_TRUE | `any truthy expression`      |
|                     ST_FALSE | `!(any truthy expression)`   |
|                     ST_EQUAL | `lhs == rhs`                 |
|                 ST_NOT_EQUAL | `lhs != rhs`                 |
|                 ST_LESS_THAN | `lhs < rhs`                  |
|        ST_LESS_THAN_OR_EQUAL | `lhs <= rhs`                 |
|              ST_GREATER_THAN | `lhs > rhs`                  |
|     ST_GREATER_THAN_OR_EQUAL | `lhs >= rhs`                 |
|                      ST_NULL | `ptr == NULL`                |
|                  ST_NOT_NULL | `ptr != NULL`                |

### Bitwise

|                    Evaluator | Expression                                                   |
| :--------------------------- | :----------------------------------------------------------- |
|                 ST_BITS_HIGH | `(bitmask & bits) == bits`                                   |
|                  ST_BITS_LOW | `(bitmask & bits) == 0`                                      |
|             ST_BITWISE_EQUAL | `memcmp(&lhs, &rhs, size) == 0`                              |
|         ST_BITWISE_NOT_EQUAL | `memcmp(&lhs, &rhs, size) != 0`                              |
|            ST_BITWISE_ZEROED | `all bytes == 0`                                             |
|        ST_BITWISE_NOT_ZEROED | `any byte != 0`                                              |

### String

| Evaluator                    | Expression                                                                       |
| :--------------------------- | :------------------------------------------------------------------------------- |
| ST_STR_EMPTY                 | `str == NULL \|\| *(str) == '\0'`                                                |
| ST_STR_NOT_EMPTY             | `str != NULL && *str != '\0'`                                                    |
| ST_STR_EQUAL                 | `strcmp(str1, str2) == 0`                                                        |
| ST_STR_NOT_EQUAL             | `strcmp(str1, str2) != 0`                                                        |
| ST_STR_EQUAL_I               | `strcasecmp/StrCmpI(str1, str2) == 0`                                            |
| ST_STR_NOT_EQUAL_I           | `strcasecmp/StrCmpI(str1, str2) != 0`                                            |
| ST_STR_CONTAINS              | `strstr/StrStr(needle, haystack) != NULL`                                        |
| ST_STR_NOT_CONTAINS          | `strstr/StrStr(needle, haystack) == NULL`                                        |
| ST_STR_CONTAINS_I            | `strcasestr/StrStrI(needle, haystack) != NULL`                                   |
| ST_STR_NOT_CONTAINS_I        | `strcasestr/StrStrI(needle, haystack) == NULL`                                   |
| ST_STR_BEGINSWITH            | `strncmp/StrCmpN(needle, haystack, needle_len) == 0`                             |
| ST_STR_NOT_BEGINSWITH        | `strncmp/StrCmpN(needle, haystack, needle_len) != 0`                             |
| ST_STR_BEGINSWITH_I          | `strncasecmp/StrCmpNI(needle, haystack, needle_len) == 0`                        |
| ST_STR_NOT_BEGINSWITH_I      | `strncasecmp/StrCmpNI(needle, haystack, needle_len) != 0`                        |
| ST_STR_BEGINSWITH_WSPACE     | `isspace(str[0])`                                                                |
| ST_STR_NOT_BEGINSWITH_WSPACE | `!isspace(str[0])`                                                               |
| ST_STR_ENDSWITH              | `strncmp/StrCmpN(haystack + (haystack_len - needle_len), needle_len) == 0`       |
| ST_STR_NOT_ENDSWITH          | `strncmp/StrCmpN(haystack + (haystack_len - needle_len), needle_len) != 0`       |
| ST_STR_ENDSWITH_I            | `strncasecmp/StrCmpNI(haystack + (haystack_len - needle_len), needle_len) == 0`  |
| ST_STR_NOT_ENDSWITH_I        | `strncasecmp/StrCmpNI(haystack + (haystack_len - needle_len), needle_len) != 0`  |
| ST_STR_ENDSWITH_WSPACE       | `isspace(p[strlen(p) - 1])`                                                      |
| ST_STR_NOT_ENDSWITH_WSPACE   | `!isspace(p[strlen(p) - 1])`                                                     |
| ST_STR_ALPHA                 | `foreach(str) => isalpha`                                                        |
| ST_STR_NUMERIC               | `foreach(str) => isdigit`                                                        |
| ST_STR_ALPHANUMERIC          | `foreach(str) => isalnum`                                                        |

### Numeric

| Evaluator              | Expression                  |
| :--------------------- | :-------------------------- |
| ST_NUM_POSITIVE        | `num > 0`                   |
| ST_NUM_NEGATIVE        | `num < 0`                   |
| ST_NUM_EVEN            | `num % 2 == 0`              |
| ST_NUM_ODD             | `num % 2 != 0`              |
| ST_NUM_MULTIPLE_OF     | `num % exp == 0`            |
| ST_NUM_NOT_MULTIPLE_OF | `num % exp != 0`            |
| ST_NUM_IN_RANGE        | `num >= low && num <= high` |
| ST_NUM_NOT_IN_RANGE    | `num < low \|\| num > high` |

### Array

|                    Evaluator | Expression                                                               |
| :--------------------------- | :----------------------------------------------------------------------- |
|               ST_ARRAY_EQUAL | `sizeof(arr1[0]) == sizeof(arr2[0]) && count1 == count2 && all elems ==` |
|           ST_ARRAY_NOT_EQUAL | `sizeof(arr1[0]) != sizeof(arr2[0])`                                     |
|            ST_ARRAY_CONTAINS | `sizeof(arr[0]) == sizeof(val) && any elem == val`                       |
|        ST_ARRAY_NOT_CONTAINS | `sizeof(arr[0]) != sizeof(val) && all elems != val`                      |
|              ST_ARRAY_UNIQUE | `foreach(arr) => (n) foreach(arr) => (j) arr[n] != arr[j]`               |

## Proposed To-Be-Implemented (someday) Evaluators

### Floating Point

| Evaluator | Expression |
| --- | --- |
| ST\_FLOAT\_EQUAL | `fabs(a - b) <= epsilon` |
| ST\_FLOAT\_NOT\_EQUAL | `fabs(a - b) > epsilon` |

### General Purpose (additions)

| Evaluator | Expression |
| --- | --- |
| ST\_NUM\_ZERO | `num == 0` |
| ST\_NUM\_NOT\_ZERO | `num != 0` |
| ST\_WITHIN | `abs(val - target) <= tolerance` |
| ST\_POWER\_OF\_TWO | `(num != 0) && ((num & (num - 1)) == 0)` |
| ST\_SIZEOF | `sizeof(type_or_expr) == expected_size` |
| ST\_ALIGNOF | `_Alignof(type) == expected_alignment` |
| ST\_ERRNO | `errno == expected_errno` |

### String (additions)

| Evaluator | Expression |
| --- | --- |
| ST\_STR\_LENGTH | `strlen(str) == expected_len` |
| ST\_STR\_FOREACH | `foreach(str) => predicate_fn(ch) == true` |

### Numeric (additions)

| Evaluator | Expression |
| --- | --- |
| ST\_NUM\_POWER\_OF\_TWO | `(num != 0) && ((num & (num - 1)) == 0)` |

### Array (additions)

| Evaluator | Expression |
| --- | --- |
| ST\_ARRAY\_LENGTH | `sizeof(arr) / sizeof(arr[0]) == expected_count` |
| ST\_ARRAY\_SORTED | `foreach(arr) => arr[n] <= arr[n+1]` |
| ST\_ARRAY\_SORTED\_DESC | `foreach(arr) => arr[n] >= arr[n+1]` |
| ST\_ARRAY\_ALL | `foreach(arr) => predicate_fn(arr[n]) == true` |
| ST\_ARRAY\_ANY | `foreach(arr) => predicate_fn(arr[n]) == true (at least one)` |

## Helpers

| Helper                  | Description                                                                         |
|:------------------------|:------------------------------------------------------------------------------------|
| ST_DEBUG                | If `ST_DEBUG_MESSAGES` is defined, emits a diagnotic message in gray                |
| ST_MESSAGE              | Emits an informative message                                                        |
| ST_SUCCESS              | Emits a message in green                                                            |
| ST_WARNING              | Emits a message in orange                                                           |
| ST_ERROR                | Emits a message in red                                                              |
| ST_TEST_IS_FAILED       | Returns true if any errors have occurred during the execution of the test           |
| ST_TEST_IS_PASSING      | Returns true if no errors have occurred during the excecution of the test           |
| ST_TEST_LAST_EVAL_FALSE | Returns true if the very last evaluator to execute resulted in a false expression    |
| ST_TEST_ERROR_COUNT     | Returns the number of errors that have occurred during the excecution of the test   |
| ST_TEST_WARNING_COUNT   | Returns the number of warnings that have occurred during the excecution of the test |
| ST_TEST_EXIT_IF_FAILED  | Exits immediately from the test if any errors have occurred                         |
| ST_OS_ERROR_MSG         | Emits a formatted error message describing an OS/libc error that occcurred          |

## Preprocessor macros

| Macro                       | Description                                                                           |
|:----------------------------|:--------------------------------------------------------------------------------------|
| ST_SIMULATE_FS_ERROR        | Simulates a failure to determine available disk space                                 |
| ST_SIMULATE_FS_INSUFFICIENT | Simulates a low disk space condition (*mutually exclusive with ST_SIMULATE_FS_ERROR*) |
| ST_SIMULATE_INET_ERROR      | Simulates a failure to detect an Internet connection                                  |
| ST_DEBUG_MESSAGES           | Enables the diagnostic output to the terminal                                         |
