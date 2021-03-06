/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/byte_buf.h>
#include <proof_helpers/make_common_data_structures.h>

void aws_byte_buf_init_copy_harness() {
    /* data structure */
    struct aws_byte_buf *dest;

    /* parameters */
    struct aws_allocator *allocator;
    struct aws_byte_buf src;

    /* assumptions */
    __CPROVER_assume(aws_byte_buf_is_bounded(&src, UINT32_MAX));
    src.allocator = (nondet_bool()) ? NULL : aws_default_allocator();
    src.buffer = malloc(sizeof(*(src.buffer)) * src.capacity);
    __CPROVER_assume(aws_byte_buf_is_valid(&src));
    ASSUME_VALID_MEMORY(dest);
    ASSUME_DEFAULT_ALLOCATOR(allocator);

    /* save current state of the data structure */
    struct aws_byte_buf old = src;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array(src.buffer, src.len, &old_byte);

    size_t old_byte_ind = old_byte.index;
    uint8_t old_byte_val = old_byte.byte;

    /* operation under verification */
    if (!aws_byte_buf_init_copy(dest, allocator, &src)) {
        /* assertions */
        uint8_t *dest_buf = dest->buffer;
        bool flag = aws_byte_buf_is_valid(dest);
        assert(flag);
        flag = aws_byte_buf_has_allocator(dest);
        assert(flag);
        assert(dest->len == src.len);
        assert(dest->capacity == src.capacity);
        assert_bytes_match(dest->buffer, src.buffer, dest->len);
        flag = aws_byte_buf_is_valid(&src);
        assert(flag);
        if (src.len > 0) {
            assert_byte_from_buffer_matches(src.buffer, &old_byte);
            assert_byte_from_buffer_matches(dest->buffer, &old_byte);
        }
    }
}
