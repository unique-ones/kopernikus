/* date = March 24th 2023 4:15 pm */

#ifndef TYPES_H
#define TYPES_H

#include <glad/glad.h>
#include <stddef.h>
#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u32 b32;

typedef float f32;
typedef double f64;

#define true 1
#define false 0

#define STACK_ARRAY_SIZE(stack_array) (sizeof stack_array / sizeof stack_array[0])

#define ASSERT(x, ...)               \
    if (!(x)) {                      \
        fprintf(stderr, __VA_ARGS__);\
        abort();                     \
    }


#endif //TYPES_H
