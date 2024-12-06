// RUN: %clang_analyze_cc1 -verify -analyzer-output=text %s \
// RUN:   -analyzer-checker=amadeus.NullablePointers

#include "nullable_pointers/gsl/pointers.hpp"
using gsl::not_null;

void any_call(int *&p);

// RULE 8.4.6
// a while statement `while (c) { s }` where `c` depends on `v == nullptr`
//

//
// RULE 8.4.6.1
// when `c` is `true` when `v != nullptr` a null-safety proof for `v` is established in `s`
//

int test_safe_deref_lvalue_proof_via_while_not_null_condition_param(int *p) {
  while (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_misproof_via_while_not_null_condition_unmodified_param(int *p) { // expected-note{{declared as nullable pointer here}}
  while (p) { // expected-note{{Assuming pointer value is null}}
              // expected-note@-1{{Loop condition is false. Execution continues on line}}
  }
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

//
// RULE 8.4.6.2
// when `c` is `false` when `v == nullptr` and for all branches in `s` terminating with `break` a null-safety proof for `v` is established, then a null-safety proof for `v` is established immediately after the `while`
//

int test_safe_deref_lvalue_proof_via_while_null_condition_modified_param(int *p) {
  while (!p) { ++p; }
  return *p;
}

int test_unsafe_deref_lvalue_proof_via_while_null_condition_unmodified_param(int *p) { // expected-note{{declared as nullable pointer here}}
  while (!p) { } // expected-note{{Assuming 'p' is null}}
                 // expected-note@-1{{Loop condition is true.  Entering loop body}}
                 // expected-note@-2{{Loop condition is false. Execution continues on line}}
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

int test_safe_deref_lvalue_proof_via_while_null_condition_modified_and_nested_if_break_with_safe_param(int *p, int coin) {
  while (!p) {
    if (coin) {
      p = &coin;
      break;
    }
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_while_null_condition_and_nested_if_else_break_param(int *p, int coin, not_null<int*> q) {
  while (!p) {
    if (coin) {
      p = &coin;
      break;
    }
    else {
      p = q;
      break;
    }
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_while_null_condition_and_nested_if_param(int *p, int coin) {
  while (!p) {
    if (coin)
      p = &coin;
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_while_null_condition_and_nested_if_param(int *p, bool coin, int* q) {
  while (!p) {
    if (coin)
      p = q;
  }
  return *p;
}


int test_unsafe_deref_lvalue_misproof_via_while_body_break_null_condition(int *p, int* q) { // expected-note{{declared as nullable pointer here}}
  while (!p) { // expected-note{{Assuming 'p' is null}}
               // expected-note@-1{{Loop condition is true.  Entering loop body}}
    p = q; // expected-note{{Value assigned to 'p'}}
    break; // expected-note{{Execution continues on line}}
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_while_null_condition_and_nested_if_break_param(int *p, int coin, int* q) { // expected-note{{declared as nullable pointer here}}
  while (!p) { // expected-note{{Assuming 'p' is null}}
               // expected-note@-1{{Loop condition is true.  Entering loop body}}
    if (coin) { // expected-note{{Assuming 'coin' is not equal to 0}}
                // expected-note@-1{{Taking true branch}}
      p = q; // expected-note{{Value assigned to 'p'}}
      break; // expected-note{{Execution continues on line}}
    }
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_while_null_condition_and_nested_if_else_break_param(int *p, int coin, int* q) { // expected-note{{declared as nullable pointer here}}
  while (!p) { // expected-note{{Assuming 'p' is null}}
               // expected-note@-1{{Loop condition is true.  Entering loop body}}
    if (coin) { // expected-note{{Assuming 'coin' is 0}}
                // expected-note@-1{{Taking false branch}}
      p = &coin;
      break;
    }
    else {
      p = q; // expected-note{{Value assigned to 'p'}}
      break; // expected-note{{Execution continues on line}}
    }
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_while_null_condition_param(int *p) { // expected-note{{declared as nullable pointer here}}
  while (!p) { // expected-note{{Assuming 'p' is null}}
               // expected-note@-1{{Loop condition is true.  Entering loop body}}
    return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{nullptr dereference of a nullable pointer}}
  }
  return 0;
}

int test_unsafe_deref_lvalue_misproof_via_while_null_condition_break_param(int *p) { // expected-note{{declared as nullable pointer here}}
  while (!p) { // expected-note{{Assuming 'p' is null}}
               // expected-note@-1{{Loop condition is true.  Entering loop body}}
    break; // expected-note{{Execution continues on line}}
  }
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

//
// RULE 8.4.7
// a do-while statement `do { s } while(c)` when `c` is `false` when `v == nullptr` and for all branches in `s` terminating with `break` a null safety proof for `v` is established, then a null-safety proof for `v` is established immediately after the do-while.
//

int test_safe_deref_lvalue_proof_via_do_while_null_condition_param(int *p) {
  do {

  } while (!p);

  return *p;
}

int test_unsafe_deref_lvalue_misproof_via_do_while_null_condition_break_param(int *p) { // expected-note{{declared as nullable pointer here}}
  do {
    break; // expected-note{{Execution continues on line}}
  } while (!p);

  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_safe_deref_lvalue_proof_via_do_while_null_condition_break_param(int *p, not_null<int*> q) {
  do {
    p = q;
    break;
  } while (!p);

  return *p;
}

int test_safe_deref_lvalue_proof_via_do_while_null_condition_if_break_param(int *p, int *q) {
  do {
    if (q) {
      p = q;
      break;
    }
  } while (!p);

  return *p;
}

int test_safe_deref_lvalue_proof_via_do_while_null_condition_if_else_break_param(int *p, int coin, int *q) {
  do {
    if (q) {
      p = q;
      break;
    } else {
      p = &coin;
      break;
    }
  } while (!p);

  return *p;
}

int test_unsafe_deref_lvalue_misproof_via_do_while_null_condition_break_param(int *p, int *q) { // expected-note{{declared as nullable pointer here}}
  do {
    p = q; // expected-note{{Value assigned to 'p'}}
    break; // expected-note{{Execution continues on line}}
  } while (!p);

  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_do_while_null_condition_if_break_param(int *p, int coin, int *q) { // expected-note{{declared as nullable pointer here}}
  do {
    if (coin) { // expected-note{{Assuming 'coin' is not equal to 0}}
                // expected-note@-1{{Taking true branch}}
      p = q; // expected-note{{Value assigned to 'p'}}
      break; // expected-note{{Execution continues on line}}
    }
  } while (!p);

  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_do_while_null_condition_if_else_break_param(int *p, int coin, int *q) { // expected-note{{declared as nullable pointer here}}
  do {
    if (coin) { // expected-note{{Assuming 'coin' is 0}}
                // expected-note@-1{{Taking false branch}}
      p = &coin;
      break;
    } else {
      p = q; // expected-note{{Value assigned to 'p'}}
      break; // expected-note{{Execution continues on line}}
    }
  } while (!p);

  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

//
// RULE 8.4.8
// a for statement `for (a; c; b) s` where `c` depends on `v == nullptr`
//

//
// RULE 8.4.8.1
// when `c` is `true` when `v != nullptr` a null-safety proof for `v` is established in the statement sequence `s;b`
//

int test_safe_deref_loop_body_lvalue_proof_via_for_not_null_condition_param(int *p) {
  for (; p; ) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_loop_body_lvalue_for_null_condition_param(int *p) { // expected-note{{declared as nullable pointer here}}
  for (; !p; ) { // expected-note{{Assuming 'p' is null}}
                 // expected-note@-1{{Loop condition is true.  Entering loop body}}
    return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{nullptr dereference of a nullable pointer}}
  }
  return 0;
}

int test_unsafe_deref_lvalue_misproof_via_for_not_null_condition_break_param(int *p) { // expected-note{{declared as nullable pointer here}}
  for (; p; ) { // expected-note{{Assuming pointer value is null}}
                // expected-note@-1{{Loop condition is false. Execution continues on line}}
    break;
  }
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_for_not_null_condition_param(int *p) { // expected-note{{declared as nullable pointer here}}
  for (; p; ) { // expected-note{{Assuming pointer value is null}}
                // expected-note@-1{{Loop condition is false. Execution continues on line}}
  }
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

int test_safe_deref_iter_expr_lvalue_proof_via_for_not_null_condition_param(int *p) {
  for (; p; (*p)++) {
    return *p;
  }
  return 0;
}

int test_safe_deref_iter_expr_with_mutation_lvalue_proof_via_for_not_null_condition_param(int *p) {
  for (; p; (*p)++, p++) {

  }
  return 0;
}

//
// RULE 8.4.8.2
// when `c` is `false` when `v == nullptr` and for all branches in `s` terminating with `break` a null-safety proof for `v` is established, then a null-safety proof for `v` is established immediately after the `for`
//

int test_safe_deref_lvalue_proof_via_for_null_condition_param(int *p) {
  for (; !p; ) { }
  return *p;
}

int test_unsafe_deref_lvalue_misproof_via_for_null_condition_break_param(int *p) { // expected-note{{declared as nullable pointer here}}
  for (; !p; ) { // expected-note{{Assuming 'p' is null}}
                 // expected-note@-1{{Loop condition is true.  Entering loop body}}
    break; // expected-note{{Execution continues on line}}
  }
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

int test_safe_deref_lvalue_proof_via_for_null_condition_break_param(int *p, not_null<int*> q) {
  for (; !p; ) {
    p = q;
    break;
  }
  return *p;
}

int test_unsafe_deref_lvalue_misproof_via_for_null_condition_assign_break_param(int *p, int* q) { // expected-note{{declared as nullable pointer here}}
  for (; !p; ) { // expected-note{{Assuming 'p' is null}}
                 // expected-note@-1{{Loop condition is true.  Entering loop body}}
    p = q; // expected-note{{Value assigned to 'p'}}
    break; // expected-note{{Execution continues on line}}
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_safe_deref_lvalue_proof_via_for_null_condition_assign_param(int *p, int *q) {
  for (; !p; ) {
    p = q;
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_for_null_condition_if_assign_param(int *p, int *q) {
  for (; !p; ) {
    if (q) {
      p = q;
    }
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_for_null_condition_if_assign_break_param(int *p, int *q) {
  for (; !p; ) {
    if (q) {
      p = q;
      break;
    }
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_for_null_condition_else_assign_break_param(int *p, int coin, int *q) {
  for (; !p; ) {
    if (q) {
      p = q;
    } else {
      p = &coin;
      break;
    }
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_for_null_condition_else_nonptr_break_param(int *p, int coin, int *q) {
  for (; !p; ) {
    if (coin) {
      p = q;
    } else {
      p = &coin;
      break;
    }
  }
  return *p;
}

int test_safe_deref_lvalue_proof_via_for_null_condition_else_assign_break_param(int *p, int coin, not_null<int*> q) {
  for (; !p; ) {
    if (q) {
      p = q;
      break;
    } else {
      p = &coin;
      break;
    }
  }
  return *p;
}

int test_unsafe_deref_lvalue_misproof_via_for_null_condition_if_assign_break_param(int *p, int coin, int* q) { // expected-note{{declared as nullable pointer here}}
  for (; !p; ) { // expected-note{{Assuming 'p' is null}}
                 // expected-note@-1{{Loop condition is true.  Entering loop body}}
    if (coin) { // expected-note{{Assuming 'coin' is not equal to 0}}
                // expected-note@-1{{Taking true branch}}
      p = q; // expected-note{{Value assigned to 'p'}}
      break; // expected-note{{Execution continues on line}}
    }
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_misproof_via_for_null_condition_else_assign_break_param(int *p, int coin, int* q) { // expected-note{{declared as nullable pointer here}}
  for (; !p; ) { // expected-note{{Assuming 'p' is null}}
                 // expected-note@-1{{Loop condition is true.  Entering loop body}}
    if (coin) { // expected-note{{Assuming 'coin' is 0}}
                // expected-note@-1{{Taking false branch}}
      p = &coin;
      break;
    } else {
      p = q; // expected-note{{Value assigned to 'p'}}
      break; // expected-note{{Execution continues on line}}
    }
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

//
// RULE 13
// Given a nullable pointer `v` and a loop of either:
//

// RULE 13.a.1
// `while(c) s`
//  All loop-modified null-safe variant nullable pointers have any null-safety proofs that hold prior to the loop-statement-sequence invalidated at the start of the loop-statement-sequence.
//

void test_safe_modified_invariant_prior_proof_lvalue_param(int *p) {
  if (p) {
    while (*p) {
      ++p;
    }
  }
}

void test_unsafe_modified_variant_via_unmodified_without_prior_proof_lvalue_param(int *p, int *q) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    while (*p) { // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                 // expected-note@-1{{dereference of a nullable pointer}}
                 // expected-note@-2{{nullable-safety proof established from this point}}
      p = q;
    }
  }
}

void test_unsafe_modified_variant_via_call_lvalue_param(int *p, int *q) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    while (*p) { // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                 // expected-note@-1{{dereference of a nullable pointer}}
                 // expected-note@-2{{nullable-safety proof established from this point}}
      any_call(p);
    }
  }
}

void test_unsafe_modified_variant_via_modified_invariant_without_prior_proof_lvalue_param(int *p, int *q) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    while (*p) { // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                 // expected-note@-1{{dereference of a nullable pointer}}
                 // expected-note@-2{{nullable-safety proof established from this point}}
                 // expected-note@-3{{Loop condition is true.  Entering loop body}}
      ++q; // expected-note{{Value assigned to 'q'}}
      p = q; // expected-note{{The value of 'q' is assigned to 'p'}}
    }
  }
}

void test_safe_modified_invariant_via_unmodified_depedency_with_prior_proof_lvalue_param(int *p, int *q) {
  if (!p) return;
  if (!q) return;

  while (*p) {
    p = q + 1;
  }
}

void test_safe_modified_invariant_via_mutual_depedency_with_prior_proof_lvalue_param(int *p, int *q) {
  if (!p) return;
  if (!q) return;

  while (*p) {
    q = p + 1;
    p = q + 1;
  }
}

void test_safe_condition_invalidated_and_proven_when_modified_invariant_without_prior_proof_lvalue_param(int *p) {
  while (p && *p) {
    ++p;
  }
}

void test_safe_condition_modified_to_null_safe_is_invariant_lvalue_param(int *p) {
  int x = 0;

  if (p) {
    while (*p) {
      p = &x;
    }
  }
}

void test_safe_condition_modified_as_non_nullable_lvalue_param(int *p) {
  int x = 0;
  if (!p) return;

  not_null<int *> q = p;

  while (*q) {
    q = &x;
  }
}

// RULE 13.a.2
// `while(c) s`
// If at the end of any branch of the loop-statement-sequence a nullable pointer is not null-safe then it is not null-safe after the loop.
//

int test_unsafe_after_invariant_safe_in_loop_but_unsafe_out_lvalue_param(int *p) { // expected-note{{declared as nullable pointer here}}
  while (p && *p > 0) { // expected-note{{Value assigned to 'p'}}
                        // expected-note@-1{{Assuming 'p' is null}}
                        // expected-note@-2{{Left side of '&&' is false}}
                        // expected-note@-3{{Loop condition is false. Execution continues on line}}
    ++p;
  }
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_after_invariant_but_unsafe_lvalue_param(int *p, int *q) { // expected-note{{declared as nullable pointer here}}
  if (!p) return 0; // expected-note{{Assuming 'p' is non-null}}
                    // expected-note@-1{{Taking false branch}}

  while (*p) { // expected-note{{Loop condition is false. Execution continues on line}}
    ++p;
    ++q;
  }
  return *q; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

// XXRULE 13.a.2
// `while(c) s`
// Null-safety proofs established in `s` do not survive from one iteration to the next.
//

int test_unsafe_no_cross_iteration_proof_simple_within_max_block_count_lvalue_param(int *p, not_null<int*> q) { // expected-note{{declared as nullable pointer here}}
  int i = 1;
  // int *r = &i;
  while (i) { // expected-note{{Loop condition is true.  Entering loop body}}
    if (i == 2) return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                           // expected-note@-1{{Assuming 'i' is equal to 2}}
                           // expected-note@-2{{Taking true branch}}
                           // expected-note@-3{{dereference of a nullable pointer}}
    p = q;
    ++i;
    // ++*r;
  }
  return 0;
}

int test_unsafe_no_cross_iteration_proof_simple_beyond_max_block_count_lvalue_param(int *p, not_null<int*> q) { // expected-note{{declared as nullable pointer here}}
  int i = 1;
  // int *r = &i;
  while (i) { // expected-note{{Loop condition is true.  Entering loop body}}
    if (i == 5) return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                           // expected-note@-1{{Assuming 'i' is equal to 5}}
                           // expected-note@-2{{Taking true branch}}
                           // expected-note@-3{{dereference of a nullable pointer}}
    p = q;
    ++i;
    // ++*r;
  }
  return 0;
}

// b. `do s while (c)`
// c. `for (a; c; b) s`
//
