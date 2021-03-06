/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/array_list.h>
#include <proof_helpers/make_common_data_structures.h>

/**
 * Runtime: 3m 42s
 */
void aws_array_list_set_at_harness() {
    /* data structure */
    struct aws_array_list list;
    __CPROVER_assume(list.current_size < UINT32_MAX);
    
    /* assumptions */
    __CPROVER_assume(aws_array_list_is_bounded(&list, MAX_INITIAL_ITEM_ALLOCATION, MAX_ITEM_SIZE));
    list.data = malloc(list.current_size);
    list.alloc = nondet_bool() ? NULL : aws_default_allocator();
    __CPROVER_assume(aws_array_list_is_valid(&list));
    __CPROVER_assume(list.data != NULL);
    size_t malloc_size;
    void *val = malloc(list.item_size);
    size_t index;
    __CPROVER_assume(index <= MAX_INITIAL_ITEM_ALLOCATION);

    /* save current state of the data structure */
    struct aws_array_list old = list;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array((uint8_t *)list.data, list.current_size, &old_byte);

    /* assume preconditions */
    __CPROVER_assume(aws_array_list_is_valid(&list));
    __CPROVER_assume(val && AWS_MEM_IS_READABLE(val, list.item_size));

    /* perform operation under verification and assertions */
    if (!aws_array_list_set_at(&list, val, index)) {
        if (index > old.length)
            assert(list.length == index + 1);
    } else {
        /* In the case aws_array_list_set_at is not successful, the list must not change */
        assert_array_list_equivalence(&list, &old, &old_byte);
    }
    bool flag = aws_array_list_is_valid(&list);
    assert(flag);
}
