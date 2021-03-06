/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/array_list.h>
#include <proof_helpers/make_common_data_structures.h>

/**
 * Runtime: 5s
 */
void aws_array_list_clear_harness() {
    /* data structure */
    struct aws_array_list list;
    __CPROVER_assume(list.current_size < UINT32_MAX);
    
    /* assumptions */
    __CPROVER_assume(aws_array_list_is_bounded(&list, MAX_INITIAL_ITEM_ALLOCATION, MAX_ITEM_SIZE));
    // ensure_array_list_has_allocated_data_member(&list);
    list.data = malloc(list.current_size);
    list.alloc = nondet_bool() ? NULL : aws_default_allocator();
    __CPROVER_assume(aws_array_list_is_valid(&list));

    /* save current state of the data structure */
    struct aws_array_list old = list;

    /* perform operation under verification */
    aws_array_list_clear(&list);

    /* assertions */
    bool flag = aws_array_list_is_valid(&list);
    assert(flag);
    assert(list.length == 0);
    assert(list.alloc == old.alloc);
    assert(list.current_size == old.current_size);
    assert(list.item_size == old.item_size);
    assert(list.data == old.data);
}
