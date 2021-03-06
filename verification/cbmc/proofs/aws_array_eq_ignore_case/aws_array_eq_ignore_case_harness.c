/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/byte_buf.h>
#include <proof_helpers/make_common_data_structures.h>

void aws_array_eq_ignore_case_harness() {
    /* assumptions */
    size_t lhs_len;
    __CPROVER_assume(lhs_len <= UINT32_MAX);
    void *lhs = malloc(lhs_len);

    void *rhs;
    size_t rhs_len;
    if (nondet_bool()) { /* rhs could be equal to lhs */
        rhs_len = lhs_len;
        rhs = lhs;
    } else {
        __CPROVER_assume(rhs_len <= UINT32_MAX);
        rhs = malloc(rhs_len);
    }

    /* save current state of the parameters */
    struct store_byte_from_buffer old_byte_from_lhs;
    save_byte_from_array((uint8_t *)lhs, lhs_len, &old_byte_from_lhs);
    struct store_byte_from_buffer old_byte_from_rhs;
    save_byte_from_array((uint8_t *)rhs, rhs_len, &old_byte_from_rhs);

    /* pre-conditions */
    __CPROVER_assume((lhs_len == 0) || AWS_MEM_IS_READABLE(lhs, lhs_len));
    __CPROVER_assume((rhs_len == 0) || AWS_MEM_IS_READABLE(rhs, rhs_len));

    /* operation under verification */
    if (aws_array_eq_ignore_case(lhs, lhs_len, rhs, rhs_len)) {
        assert(lhs_len == rhs_len);
    }

    /* asserts both parameters remain unchanged */
    if (lhs_len > 0) {
        assert_byte_from_buffer_matches((uint8_t *)lhs, &old_byte_from_lhs);
    }
    if (rhs_len > 0) {
        assert_byte_from_buffer_matches((uint8_t *)rhs, &old_byte_from_rhs);
    }
}
