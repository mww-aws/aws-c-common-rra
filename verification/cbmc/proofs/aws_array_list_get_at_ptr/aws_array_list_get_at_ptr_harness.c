/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/array_list.h>
#include <proof_helpers/make_common_data_structures.h>

/**
 * Runtime: 6s
 */
void aws_array_list_get_at_ptr_harness() {
    /* data structure */
    struct aws_array_list list;
    __CPROVER_assume(list.current_size < UINT32_MAX);

    /* assumptions */
    __CPROVER_assume(aws_array_list_is_bounded(&list, MAX_INITIAL_ITEM_ALLOCATION, MAX_ITEM_SIZE));
    list.data = malloc(list.current_size);
    list.alloc = nondet_bool() ? NULL : aws_default_allocator();
    ensure_array_list_has_allocated_data_member(&list);
    __CPROVER_assume(aws_array_list_is_valid(&list));
    void **val = malloc(sizeof(void *));
    size_t index;

    /* save current state of the data structure */
    struct aws_array_list old = list;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array((uint8_t *)list.data, list.current_size, &old_byte);

    /* assume preconditions */
    __CPROVER_assume(aws_array_list_is_valid(&list));
    __CPROVER_assume(val);

    /* perform operation under verification */
    if (!aws_array_list_get_at_ptr(&list, val, index)) {
        /* In the case aws_array_list_get_at is successful, we can ensure the list isn't empty
         * and index is within bounds.
         */
        assert(list.data);
        assert(list.length > index);
    }

    /* assertions */
    bool flag = aws_array_list_is_valid(&list);
    assert(flag);
    assert_array_list_equivalence(&list, &old, &old_byte);
}
