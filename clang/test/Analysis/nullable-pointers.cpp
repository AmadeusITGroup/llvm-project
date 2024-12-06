// RUN: %clang_analyze_cc1 -verify -analyzer-output=text %s \
// RUN:   -analyzer-checker=amadeus.NullablePointers

#include "nullable_pointers/gsl/pointers.hpp"

using gsl::not_null;

//
// Helper declarations
//

struct NonScalar {
  int member_field;
  int member_func();
};

// FIXME consider all other variations
struct NullableScalar {
  NullableScalar() = default;
  NullableScalar(decltype(nullptr));
  explicit operator bool() const;
  friend bool operator==(NullableScalar L, NullableScalar R);
  friend bool operator!=(NullableScalar L, NullableScalar R);

  int operator*();
  int operator*() const;
  int operator*() volatile;
  int operator*() const volatile;

private:
  int v;
};

struct NullableNonScalar {
  NullableNonScalar() = default;
  NullableNonScalar(decltype(nullptr));
  explicit operator bool() const;
  friend bool operator==(NullableNonScalar L, NullableNonScalar R);
  friend bool operator!=(NullableNonScalar L, NullableNonScalar R);

  int member_field;
  void non_const_member_func();
  void const_member_func() const;

  NonScalar operator*();
  NonScalar operator*() const;
  NonScalar operator*() volatile;
  NonScalar operator*() const volatile;

  NonScalar *operator->();
  NonScalar *operator->() const;
  NonScalar *operator->() volatile;
  NonScalar *operator->() const volatile;

  NonScalar *operator->*(int NonScalar::* mp);
  NonScalar *operator->*(int NonScalar::* mp) const;
  NonScalar *operator->*(int NonScalar::* mp) volatile;
  NonScalar *operator->*(int NonScalar::* mp) const volatile;
private:
  NonScalar v;
};

void any_call();

struct Member;

int * get_scalar(); // expected-note{{declared returning nullable pointer here}}
int (*get_func())();
int (Member::*get_member_func())();
int Member::* get_member_pointer();
NonScalar* get_non_scalar();
NullableScalar get_nullable_scalar();
NullableNonScalar get_nullable_non_scalar();
not_null<int*> get_not_null_scalar();
not_null<int (*)()> get_not_null_func();
not_null<int (Member::*)()> get_not_null_member_func();
not_null<int Member::*> get_not_null_member();
not_null<NullableScalar> get_not_null_nullable_scalar();
not_null<NonScalar*> get_not_null_non_scalar();
not_null<NullableNonScalar> get_not_null_nullable_non_scalar();

int * global_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
                            // expected-note@-1{{declared as nullable pointer here}}
int *const global_scalar_const_lvalue = global_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
int *volatile global_scalar_volatile_lvalue; // expected-note{{declared as nullable pointer here}}
int *const volatile global_scalar_const_volatile_lvalue = global_scalar_volatile_lvalue; // expected-note{{declared as nullable pointer here}}
int *& global_scalar_lvalue_ref = global_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
int *const& global_scalar_const_lvalue_ref = global_scalar_const_lvalue; // expected-note{{declared as nullable pointer here}}
int *volatile& global_scalar_volatile_lvalue_ref = global_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
int *const volatile& global_scalar_const_volatile_lvalue_ref = global_scalar_volatile_lvalue_ref; // expected-note{{declared as nullable pointer here}}
int *&& global_scalar_rvalue_ref = get_scalar(); // expected-note{{declared as nullable pointer here}}
int *const&& global_scalar_const_rvalue_ref = get_scalar(); // expected-note{{declared as nullable pointer here}}
int *volatile&& global_scalar_volatile_rvalue_ref = get_scalar(); // expected-note{{declared as nullable pointer here}}
int *const volatile&& global_scalar_const_volatile_rvalue_ref = get_scalar(); // expected-note{{declared as nullable pointer here}}

int (* global_func_lvalue)(); // expected-note{{declared as nullable pointer here}}
int (*const global_func_const_lvalue)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*volatile global_func_volatile_lvalue)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*const volatile global_func_const_volatile_lvalue)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*& global_func_lvalue_ref)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*const& global_func_const_lvalue_ref)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*volatile& global_func_volatile_lvalue_ref)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*const volatile& global_func_const_volatile_lvalue_ref)() = global_func_lvalue; // expected-note{{declared as nullable pointer here}}
int (*&& global_func_rvalue_ref)() = get_func(); // expected-note{{declared as nullable pointer here}}
int (*const&& global_func_const_rvalue_ref)() = get_func(); // expected-note{{declared as nullable pointer here}}
int (*volatile&& global_func_volatile_rvalue_ref)() = get_func(); // expected-note{{declared as nullable pointer here}}
int (*const volatile&& global_func_const_volatile_rvalue_ref)() = get_func(); // expected-note{{declared as nullable pointer here}}

int (Member::* global_member_func_pointer_lvalue)(); // expected-note{{declared as nullable pointer here}}
int (Member::*const global_member_func_pointer_const_lvalue)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*volatile global_member_func_pointer_volatile_lvalue)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*const volatile global_member_func_pointer_const_volatile_lvalue)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*& global_member_func_pointer_lvalue_ref)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*const& global_member_func_pointer_const_lvalue_ref)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*volatile& global_member_func_pointer_volatile_lvalue_ref)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*const volatile& global_member_func_pointer_const_volatile_lvalue_ref)() = global_member_func_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int (Member::*&& global_member_func_pointer_rvalue_ref)() = get_member_func(); // expected-note{{declared as nullable pointer here}}
int (Member::*const&& global_member_func_pointer_const_rvalue_ref)() = get_member_func(); // expected-note{{declared as nullable pointer here}}
int (Member::*volatile&& global_member_func_pointer_volatile_rvalue_ref)() = get_member_func(); // expected-note{{declared as nullable pointer here}}
int (Member::*const volatile&& global_member_func_pointer_const_volatile_rvalue_ref)() = get_member_func(); // expected-note{{declared as nullable pointer here}}

int Member::*               global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*const          global_member_pointer_const_lvalue          = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*volatile       global_member_pointer_volatile_lvalue       = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*const volatile global_member_pointer_const_volatile_lvalue = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*&               global_member_pointer_lvalue_ref                = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*const&          global_member_pointer_const_lvalue_ref          = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*volatile&       global_member_pointer_volatile_lvalue_ref       = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*const volatile& global_member_pointer_const_volatile_lvalue_ref = global_member_pointer_lvalue; // expected-note{{declared as nullable pointer here}}
int Member::*&&               global_member_pointer_rvalue_ref                = get_member_pointer(); // expected-note{{declared as nullable pointer here}}
int Member::*const&&          global_member_pointer_const_rvalue_ref          = get_member_pointer(); // expected-note{{declared as nullable pointer here}}
int Member::*volatile&&       global_member_pointer_volatile_rvalue_ref       = get_member_pointer(); // expected-note{{declared as nullable pointer here}}
int Member::*const volatile&& global_member_pointer_const_volatile_rvalue_ref = get_member_pointer(); // expected-note{{declared as nullable pointer here}}

NonScalar *                global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar * const          global_non_scalar_const_lvalue = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar * volatile       global_non_scalar_volatile_lvalue = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar * const volatile global_non_scalar_const_volatile_lvalue = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar *&                global_non_scalar_lvalue_ref = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar * const&          global_non_scalar_const_lvalue_ref = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar * volatile&       global_non_scalar_volatile_lvalue_ref = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar * const volatile& global_non_scalar_const_volatile_lvalue_ref = global_non_scalar_lvalue; // expected-note{{declared as nullable pointer here}}
NonScalar *&&                global_non_scalar_rvalue_ref = get_non_scalar(); // expected-note{{declared as nullable pointer here}}
NonScalar * const&&          global_non_scalar_const_rvalue_ref = get_non_scalar(); // expected-note{{declared as nullable pointer here}}
NonScalar * volatile&&       global_non_scalar_volatile_rvalue_ref = get_non_scalar(); // expected-note{{declared as nullable pointer here}}
NonScalar * const volatile&& global_non_scalar_const_volatile_rvalue_ref = get_non_scalar(); // expected-note{{declared as nullable pointer here}}

NullableScalar                global_nullable_scalar_lvalue;
const NullableScalar          global_nullable_scalar_const_lvalue          = global_nullable_scalar_lvalue;
volatile NullableScalar       global_nullable_scalar_volatile_lvalue       = global_nullable_scalar_lvalue;
const volatile NullableScalar global_nullable_scalar_const_volatile_lvalue = global_nullable_scalar_lvalue;
NullableScalar                &global_nullable_scalar_lvalue_ref                = global_nullable_scalar_lvalue;
const NullableScalar          &global_nullable_scalar_const_lvalue_ref          = global_nullable_scalar_lvalue;
volatile NullableScalar       &global_nullable_scalar_volatile_lvalue_ref       = global_nullable_scalar_lvalue;
const volatile NullableScalar &global_nullable_scalar_const_volatile_lvalue_ref = global_nullable_scalar_lvalue;
NullableScalar                &&global_nullable_scalar_rvalue_ref                = get_nullable_scalar();
const NullableScalar          &&global_nullable_scalar_const_rvalue_ref          = get_nullable_scalar();
volatile NullableScalar       &&global_nullable_scalar_volatile_rvalue_ref       = get_nullable_scalar();
const volatile NullableScalar &&global_nullable_scalar_const_volatile_rvalue_ref = get_nullable_scalar();

NullableNonScalar                global_nullable_non_scalar_lvalue;
const NullableNonScalar          global_nullable_non_scalar_const_lvalue          = global_nullable_non_scalar_lvalue;
volatile NullableNonScalar       global_nullable_non_scalar_volatile_lvalue       = global_nullable_non_scalar_lvalue;
const volatile NullableNonScalar global_nullable_non_scalar_const_volatile_lvalue = global_nullable_non_scalar_lvalue;
NullableNonScalar                &global_nullable_non_scalar_lvalue_ref                = global_nullable_non_scalar_lvalue;
const NullableNonScalar          &global_nullable_non_scalar_const_lvalue_ref          = global_nullable_non_scalar_lvalue;
volatile NullableNonScalar       &global_nullable_non_scalar_volatile_lvalue_ref       = global_nullable_non_scalar_lvalue;
const volatile NullableNonScalar &global_nullable_non_scalar_const_volatile_lvalue_ref = global_nullable_non_scalar_lvalue;
NullableNonScalar                &&global_nullable_non_scalar_rvalue_ref                = get_nullable_non_scalar();
const NullableNonScalar          &&global_nullable_non_scalar_const_rvalue_ref          = get_nullable_non_scalar();
volatile NullableNonScalar       &&global_nullable_non_scalar_volatile_rvalue_ref       = get_nullable_non_scalar();
const volatile NullableNonScalar &&global_nullable_non_scalar_const_volatile_rvalue_ref = get_nullable_non_scalar();

not_null<int*> global_not_null_scalar_lvalue = get_not_null_scalar();
const not_null<int*> global_not_null_scalar_const_lvalue = global_not_null_scalar_lvalue;
not_null<int*> &global_not_null_scalar_lvalue_ref = global_not_null_scalar_lvalue;
const not_null<int*> &global_not_null_scalar_const_lvalue_ref = global_not_null_scalar_lvalue;
not_null<int*> &&global_not_null_scalar_rvalue_ref = get_not_null_scalar();
const not_null<int*> &&global_not_null_scalar_const_rvalue_ref = get_not_null_scalar();

not_null<int (*)()> global_not_null_func_lvalue = get_not_null_func();
const not_null<int (*)()> global_not_null_func_const_lvalue = global_not_null_func_lvalue;
not_null<int (*)()> &global_not_null_func_lvalue_ref = global_not_null_func_lvalue;
const not_null<int (*)()> &global_not_null_func_const_lvalue_ref = global_not_null_func_lvalue;
not_null<int (*)()> &&global_not_null_func_rvalue_ref = get_not_null_func();
const not_null<int (*)()> &&global_not_null_func_const_rvalue_ref = get_not_null_func();

not_null<int (Member::*)()> global_not_null_member_func_lvalue = get_not_null_member_func();
const not_null<int (Member::*)()> global_not_null_member_func_const_lvalue = global_not_null_member_func_lvalue;
not_null<int (Member::*)()> &global_not_null_member_func_lvalue_ref = global_not_null_member_func_lvalue;
const not_null<int (Member::*)()> &global_not_null_member_func_const_lvalue_ref = global_not_null_member_func_lvalue;
not_null<int (Member::*)()> &&global_not_null_member_func_rvalue_ref = get_not_null_member_func();
const not_null<int (Member::*)()> &&global_not_null_member_func_const_rvalue_ref = get_not_null_member_func();

not_null<int Member::*> global_not_null_member_pointer_lvalue = get_not_null_member();
const not_null<int Member::*> global_not_null_member_pointer_const_lvalue = global_not_null_member_pointer_lvalue;
not_null<int Member::*> &global_not_null_member_pointer_lvalue_ref = global_not_null_member_pointer_lvalue;
const not_null<int Member::*> &global_not_null_member_pointer_const_lvalue_ref = global_not_null_member_pointer_lvalue;
not_null<int Member::*> &&global_not_null_member_pointer_rvalue_ref = get_not_null_member();
const not_null<int Member::*> &&global_not_null_member_pointer_const_rvalue_ref = get_not_null_member();

not_null<NullableScalar> global_not_null_nullable_scalar_lvalue = get_not_null_nullable_scalar();
const not_null<NullableScalar> global_not_null_nullable_scalar_const_lvalue = global_not_null_nullable_scalar_lvalue;
not_null<NullableScalar> &global_not_null_nullable_scalar_lvalue_ref = global_not_null_nullable_scalar_lvalue;
const not_null<NullableScalar> &global_not_null_nullable_scalar_const_lvalue_ref = global_not_null_nullable_scalar_lvalue_ref;
not_null<NullableScalar> &&global_not_null_nullable_scalar_rvalue_ref = get_not_null_nullable_scalar();
const not_null<NullableScalar> &&global_not_null_nullable_scalar_const_rvalue_ref = get_not_null_nullable_scalar();

not_null<NonScalar*> global_not_null_non_scalar_lvalue = get_not_null_non_scalar();
const not_null<NonScalar*> global_not_null_non_scalar_const_lvalue = global_not_null_non_scalar_lvalue;
not_null<NonScalar*> &global_not_null_non_scalar_lvalue_ref = global_not_null_non_scalar_lvalue;
const not_null<NonScalar*> &global_not_null_non_scalar_const_lvalue_ref = global_not_null_non_scalar_const_lvalue;
not_null<NonScalar*> &&global_not_null_non_scalar_rvalue_ref = get_not_null_non_scalar();
const not_null<NonScalar*> &&global_not_null_non_scalar_const_rvalue_ref = get_not_null_non_scalar();

not_null<NullableNonScalar> global_not_null_nullable_non_scalar_lvalue = get_not_null_nullable_non_scalar();
const not_null<NullableNonScalar> global_not_null_nullable_non_scalar_const_lvalue = global_not_null_nullable_non_scalar_lvalue;
not_null<NullableNonScalar> &global_not_null_nullable_non_scalar_lvalue_ref = global_not_null_nullable_non_scalar_lvalue;
const not_null<NullableNonScalar> &global_not_null_nullable_non_scalar_const_lvalue_ref = global_not_null_nullable_non_scalar_lvalue_ref;
not_null<NullableNonScalar> &&global_not_null_nullable_non_scalar_rvalue_ref = get_not_null_nullable_non_scalar();
const not_null<NullableNonScalar> &&global_not_null_nullable_non_scalar_const_rvalue_ref = get_not_null_nullable_non_scalar();

struct Member {
  Member();

  int * member_scalar_lvalue; // expected-note{{member field declared as nullable pointer here}}
                              // expected-note@-1{{member field declared as nullable pointer here}}
                              // expected-note@-2{{member field declared as nullable pointer here}}
  int *const member_scalar_const_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                         // expected-note@-1{{member field declared as nullable pointer here}}
  int *volatile member_scalar_volatile_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                               // expected-note@-1{{member field declared as nullable pointer here}}
  int *const volatile member_scalar_const_volatile_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                           // expected-note@-1{{member field declared as nullable pointer here}}
  int *& member_scalar_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                   // expected-note@-1{{member field declared as nullable pointer here}}
                                   // expected-note@-2{{member field declared as nullable pointer here}}
                                   // expected-note@-3{{member field declared as nullable pointer here}}
  int *const& member_scalar_const_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                              // expected-note@-1{{member field declared as nullable pointer here}}
                                              // expected-note@-2{{member field declared as nullable pointer here}}
                                              // expected-note@-3{{member field declared as nullable pointer here}}
  int *volatile& member_scalar_volatile_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                    // expected-note@-1{{member field declared as nullable pointer here}}
  int *const volatile& member_scalar_const_volatile_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                // expected-note@-1{{member field declared as nullable pointer here}}
  int *&& member_scalar_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                    // expected-note@-1{{member field declared as nullable pointer here}}
  int *const&& member_scalar_const_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                               // expected-note@-1{{member field declared as nullable pointer here}}
  int *volatile&& member_scalar_volatile_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                     // expected-note@-1{{member field declared as nullable pointer here}}
  int *const volatile&& member_scalar_const_volatile_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                 // expected-note@-1{{member field declared as nullable pointer here}}

  int (* member_func_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                // expected-note@-1{{member field declared as nullable pointer here}}
  int (*const member_func_const_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                           // expected-note@-1{{member field declared as nullable pointer here}}
  int (*volatile member_func_volatile_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                                 // expected-note@-1{{member field declared as nullable pointer here}}
  int (*const volatile member_func_const_volatile_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                                             // expected-note@-1{{member field declared as nullable pointer here}}
  int (*& member_func_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                     // expected-note@-1{{member field declared as nullable pointer here}}
  int (*const& member_func_const_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                // expected-note@-1{{member field declared as nullable pointer here}}
  int (*volatile& member_func_volatile_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                      // expected-note@-1{{member field declared as nullable pointer here}}
  int (*const volatile& member_func_const_volatile_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                  // expected-note@-1{{member field declared as nullable pointer here}}
  int (*&& member_func_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                      // expected-note@-1{{member field declared as nullable pointer here}}
  int (*const&& member_func_const_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                 // expected-note@-1{{member field declared as nullable pointer here}}
  int (*volatile&& member_func_volatile_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                       // expected-note@-1{{member field declared as nullable pointer here}}
  int (*const volatile&& member_func_const_volatile_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                   // expected-note@-1{{member field declared as nullable pointer here}}

  int (Member::* member_func_pointer_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                                // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*const member_func_pointer_const_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                                           // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*volatile member_func_pointer_volatile_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                                                 // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*const volatile member_func_pointer_const_volatile_lvalue)(); // expected-note{{member field declared as nullable pointer here}}
                                                                             // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*& member_func_pointer_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                     // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*const& member_func_pointer_const_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*volatile& member_func_pointer_volatile_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                      // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*const volatile& member_func_pointer_const_volatile_lvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                                  // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*&& member_func_pointer_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                      // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*const&& member_func_pointer_const_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                 // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*volatile&& member_func_pointer_volatile_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                       // expected-note@-1{{member field declared as nullable pointer here}}
  int (Member::*const volatile&& member_func_pointer_const_volatile_rvalue_ref)(); // expected-note{{member field declared as nullable pointer here}}
                                                                                   // expected-note@-1{{member field declared as nullable pointer here}}

  int Member::*               member_pointer_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                     // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*const          member_pointer_const_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                           // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*volatile       member_pointer_volatile_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                              // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*const volatile member_pointer_const_volatile_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                                    // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*&               member_pointer_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                          // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*const&          member_pointer_const_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*volatile&       member_pointer_volatile_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                   // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*const volatile& member_pointer_const_volatile_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                         // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*&&               member_pointer_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                           // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*const&&          member_pointer_const_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                 // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*volatile&&       member_pointer_volatile_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                    // expected-note@-1{{member field declared as nullable pointer here}}
  int Member::*const volatile&& member_pointer_const_volatile_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                          // expected-note@-1{{member field declared as nullable pointer here}}

  NonScalar *                member_non_scalar_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                       // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * const          member_non_scalar_const_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                             // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * volatile       member_non_scalar_volatile_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                                // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * const volatile member_non_scalar_const_volatile_lvalue; // expected-note{{member field declared as nullable pointer here}}
                                                                      // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar *&                member_non_scalar_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                            // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * const&          member_non_scalar_const_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                  // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * volatile&       member_non_scalar_volatile_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                     // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * const volatile& member_non_scalar_const_volatile_lvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                           // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar *&&                member_non_scalar_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                             // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * const&&          member_non_scalar_const_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                   // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * volatile&&       member_non_scalar_volatile_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                      // expected-note@-1{{member field declared as nullable pointer here}}
  NonScalar * const volatile&& member_non_scalar_const_volatile_rvalue_ref; // expected-note{{member field declared as nullable pointer here}}
                                                                            // expected-note@-1{{member field declared as nullable pointer here}}

  NullableScalar                member_nullable_scalar_lvalue;
  const NullableScalar          member_nullable_scalar_const_lvalue;
  volatile NullableScalar       member_nullable_scalar_volatile_lvalue;
  const volatile NullableScalar member_nullable_scalar_const_volatile_lvalue;
  NullableScalar                &member_nullable_scalar_lvalue_ref;
  const NullableScalar          &member_nullable_scalar_const_lvalue_ref;
  volatile NullableScalar       &member_nullable_scalar_volatile_lvalue_ref;
  const volatile NullableScalar &member_nullable_scalar_const_volatile_lvalue_ref;
  NullableScalar                &&member_nullable_scalar_rvalue_ref;
  const NullableScalar          &&member_nullable_scalar_const_rvalue_ref;
  volatile NullableScalar       &&member_nullable_scalar_volatile_rvalue_ref;
  const volatile NullableScalar &&member_nullable_scalar_const_volatile_rvalue_ref;

  NullableNonScalar                member_nullable_non_scalar_lvalue;
  const NullableNonScalar          member_nullable_non_scalar_const_lvalue;
  volatile NullableNonScalar       member_nullable_non_scalar_volatile_lvalue;
  const volatile NullableNonScalar member_nullable_non_scalar_const_volatile_lvalue;
  NullableNonScalar                &member_nullable_non_scalar_lvalue_ref;
  const NullableNonScalar          &member_nullable_non_scalar_const_lvalue_ref;
  volatile NullableNonScalar       &member_nullable_non_scalar_volatile_lvalue_ref;
  const volatile NullableNonScalar &member_nullable_non_scalar_const_volatile_lvalue_ref;
  NullableNonScalar                &&member_nullable_non_scalar_rvalue_ref;
  const NullableNonScalar          &&member_nullable_non_scalar_const_rvalue_ref;
  volatile NullableNonScalar       &&member_nullable_non_scalar_volatile_rvalue_ref;
  const volatile NullableNonScalar &&member_nullable_non_scalar_const_volatile_rvalue_ref;

  int test_unsafe_deref_member_lvalue_scalar();
  int test_unsafe_deref_member_const_lvalue_scalar();
  int test_unsafe_deref_member_volatile_lvalue_scalar();
  int test_unsafe_deref_member_const_volatile_lvalue_scalar();
  int test_unsafe_deref_member_lvalue_ref_scalar();
  int test_unsafe_deref_member_const_lvalue_ref_scalar();
  int test_unsafe_deref_member_volatile_lvalue_ref_scalar();
  int test_unsafe_deref_member_const_volatile_lvalue_ref_scalar();
  int test_unsafe_deref_member_rvalue_ref_scalar();
  int test_unsafe_deref_member_const_rvalue_ref_scalar();
  int test_unsafe_deref_member_volatile_rvalue_ref_scalar();
  int test_unsafe_deref_member_const_volatile_rvalue_ref_scalar();

  int test_unsafe_deref_member_lvalue_func();
  int test_unsafe_deref_member_const_lvalue_func();
  int test_unsafe_deref_member_volatile_lvalue_func();
  int test_unsafe_deref_member_const_volatile_lvalue_func();
  int test_unsafe_deref_member_lvalue_ref_func();
  int test_unsafe_deref_member_const_lvalue_ref_func();
  int test_unsafe_deref_member_volatile_lvalue_ref_func();
  int test_unsafe_deref_member_const_volatile_lvalue_ref_func();
  int test_unsafe_deref_member_rvalue_ref_func();
  int test_unsafe_deref_member_const_rvalue_ref_func();
  int test_unsafe_deref_member_volatile_rvalue_ref_func();
  int test_unsafe_deref_member_const_volatile_rvalue_ref_func();

  int test_unsafe_deref_member_lvalue_member_func();
  int test_unsafe_deref_member_const_lvalue_member_func();
  int test_unsafe_deref_member_volatile_lvalue_member_func();
  int test_unsafe_deref_member_const_volatile_lvalue_member_func();
  int test_unsafe_deref_member_lvalue_ref_member_func();
  int test_unsafe_deref_member_const_lvalue_ref_member_func();
  int test_unsafe_deref_member_volatile_lvalue_ref_member_func();
  int test_unsafe_deref_member_const_volatile_lvalue_ref_member_func();
  int test_unsafe_deref_member_rvalue_ref_member_func();
  int test_unsafe_deref_member_const_rvalue_ref_member_func();
  int test_unsafe_deref_member_volatile_rvalue_ref_member_func();
  int test_unsafe_deref_member_const_volatile_rvalue_ref_member_func();

  int test_unsafe_deref_member_lvalue_member_pointer();
  int test_unsafe_deref_member_const_lvalue_member_pointer();
  int test_unsafe_deref_member_volatile_lvalue_member_pointer();
  int test_unsafe_deref_member_const_volatile_lvalue_member_pointer();
  int test_unsafe_deref_member_lvalue_ref_member_pointer();
  int test_unsafe_deref_member_const_lvalue_ref_member_pointer();
  int test_unsafe_deref_member_volatile_lvalue_ref_member_pointer();
  int test_unsafe_deref_member_const_volatile_lvalue_ref_member_pointer();
  int test_unsafe_deref_member_rvalue_ref_member_pointer();
  int test_unsafe_deref_member_const_rvalue_ref_member_pointer();
  int test_unsafe_deref_member_volatile_rvalue_ref_member_pointer();
  int test_unsafe_deref_member_const_volatile_rvalue_ref_member_pointer();

  int test_unsafe_arrow_member_lvalue_non_scalar();
  int test_unsafe_arrow_member_const_lvalue_non_scalar();
  int test_unsafe_arrow_member_volatile_lvalue_non_scalar();
  int test_unsafe_arrow_member_const_volatile_lvalue_non_scalar();
  int test_unsafe_arrow_member_lvalue_ref_non_scalar();
  int test_unsafe_arrow_member_const_lvalue_ref_non_scalar();
  int test_unsafe_arrow_member_volatile_lvalue_ref_non_scalar();
  int test_unsafe_arrow_member_const_volatile_lvalue_ref_non_scalar();
  int test_unsafe_arrow_member_rvalue_ref_non_scalar();
  int test_unsafe_arrow_member_const_rvalue_ref_non_scalar();
  int test_unsafe_arrow_member_volatile_rvalue_ref_non_scalar();
  int test_unsafe_arrow_member_const_volatile_rvalue_ref_non_scalar();

  int test_unsafe_deref_member_lvalue_nullable_scalar();
  int test_unsafe_deref_member_const_lvalue_nullable_scalar();
  int test_unsafe_deref_member_volatile_lvalue_nullable_scalar();
  int test_unsafe_deref_member_const_volatile_lvalue_nullable_scalar();
  int test_unsafe_deref_member_lvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_const_lvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_volatile_lvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_const_volatile_lvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_rvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_const_rvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_volatile_rvalue_ref_nullable_scalar();
  int test_unsafe_deref_member_const_volatile_rvalue_ref_nullable_scalar();

  int test_unsafe_deref_member_lvalue_scalar_proof_from_if_invalidation_call();
  int test_safe_deref_member_const_lvalue_scalar_proof_from_if_no_invalidation_call();
  int test_unsafe_deref_member_lvalue_ref_scalar_proof_from_if_invalidation_call();
  int test_unsafe_deref_member_const_lvalue_ref_scalar_proof_from_if_invalidation_call();
};

struct NotNullMember {
  not_null<int *> member_scalar_lvalue;
  const not_null<int *> member_scalar_const_lvalue;
  not_null<int *> &member_scalar_lvalue_ref;
  const not_null<int *> &member_scalar_const_lvalue_ref;
  not_null<int *> &member_scalar_rvalue_ref;
  const not_null<int *> &member_scalar_const_rvalue_ref;

  not_null<int (*)()> member_func_lvalue;
  const not_null<int (*)()> member_func_const_lvalue;
  not_null<int (*)()> &member_func_lvalue_ref;
  const not_null<int (*)()> &member_func_const_lvalue_ref;
  not_null<int (*)()> &&member_func_rvalue_ref;
  const not_null<int (*)()> &&member_func_const_rvalue_ref;

  not_null<int (Member::*)()> member_func_pointer_lvalue;
  const not_null<int (Member::*)()> member_func_pointer_const_lvalue;
  not_null<int (Member::*)()> &member_func_pointer_lvalue_ref;
  const not_null<int (Member::*)()> &member_func_pointer_const_lvalue_ref;
  not_null<int (Member::*)()> &&member_func_pointer_rvalue_ref;
  const not_null<int (Member::*)()> &&member_func_pointer_const_rvalue_ref;

  not_null<int Member::*> member_pointer_lvalue;
  const not_null<int Member::*> member_pointer_const_lvalue;
  not_null<int Member::*> &member_pointer_lvalue_ref;
  const not_null<int Member::*> &member_pointer_const_lvalue_ref;
  not_null<int Member::*> &&member_pointer_rvalue_ref;
  const not_null<int Member::*> &&member_pointer_const_rvalue_ref;

  not_null<NullableScalar>       member_nullable_scalar_lvalue;
  const not_null<NullableScalar> member_nullable_scalar_const_lvalue;
  not_null<NullableScalar>       &member_nullable_scalar_lvalue_ref;
  const not_null<NullableScalar> &member_nullable_scalar_const_lvalue_ref;
  not_null<NullableScalar>       &&member_nullable_scalar_rvalue_ref;
  const not_null<NullableScalar> &&member_nullable_scalar_const_rvalue_ref;

  not_null<NonScalar*>       member_non_scalar_lvalue;
  const not_null<NonScalar*> member_non_scalar_const_lvalue;
  not_null<NonScalar*>       &member_non_scalar_lvalue_ref;
  const not_null<NonScalar*> &member_non_scalar_const_lvalue_ref;
  not_null<NonScalar*>       &member_non_scalar_rvalue_ref;
  const not_null<NonScalar*> &member_non_scalar_const_rvalue_ref;

  not_null<NullableNonScalar>       member_nullable_non_scalar_lvalue;
  const not_null<NullableNonScalar> member_nullable_non_scalar_const_lvalue;
  not_null<NullableNonScalar>       &member_nullable_non_scalar_lvalue_ref;
  const not_null<NullableNonScalar> &member_nullable_non_scalar_const_lvalue_ref;
  not_null<NullableNonScalar>       &&member_nullable_non_scalar_rvalue_ref;
  const not_null<NullableNonScalar> &&member_nullable_non_scalar_const_rvalue_ref;

  int test_safe_deref_member_lvalue_scalar();
  int test_safe_deref_member_const_lvalue_scalar();
  int test_safe_deref_member_lvalue_ref_scalar();
  int test_safe_deref_member_const_lvalue_ref_scalar();
  int test_safe_deref_member_rvalue_ref_scalar();
  int test_safe_deref_member_const_rvalue_ref_scalar();

  int test_safe_deref_member_lvalue_func();
  int test_safe_deref_member_const_lvalue_func();
  int test_safe_deref_member_lvalue_ref_func();
  int test_safe_deref_member_const_lvalue_ref_func();
  int test_safe_deref_member_rvalue_ref_func();
  int test_safe_deref_member_const_rvalue_ref_func();

  int test_safe_deref_member_lvalue_member_func();
  int test_safe_deref_member_const_lvalue_member_func();
  int test_safe_deref_member_lvalue_ref_member_func();
  int test_safe_deref_member_const_lvalue_ref_member_func();
  int test_safe_deref_member_rvalue_ref_member_func();
  int test_safe_deref_member_const_rvalue_ref_member_func();

  int test_safe_deref_member_lvalue_member_pointer();
  int test_safe_deref_member_const_lvalue_member_pointer();
  int test_safe_deref_member_lvalue_ref_member_pointer();
  int test_safe_deref_member_const_lvalue_ref_member_pointer();
  int test_safe_deref_member_rvalue_ref_member_pointer();
  int test_safe_deref_member_const_rvalue_ref_member_pointer();

  int test_safe_deref_member_lvalue_nullable_scalar();
  int test_safe_deref_member_const_lvalue_nullable_scalar();
  int test_safe_deref_member_lvalue_ref_nullable_scalar();
  int test_safe_deref_member_const_lvalue_ref_nullable_scalar();
  int test_safe_deref_member_rvalue_ref_nullable_scalar();
  int test_safe_deref_member_const_rvalue_ref_nullable_scalar();

  int test_safe_deref_member_lvalue_non_scalar();
  int test_safe_deref_member_const_lvalue_non_scalar();
  int test_safe_deref_member_lvalue_ref_non_scalar();
  int test_safe_deref_member_const_lvalue_ref_non_scalar();
  int test_safe_deref_member_rvalue_ref_non_scalar();
  int test_safe_deref_member_const_rvalue_ref_non_scalar();

  int test_safe_deref_member_lvalue_nullable_non_scalar();
  int test_safe_deref_member_const_lvalue_nullable_non_scalar();
  int test_safe_deref_member_lvalue_ref_nullable_non_scalar();
  int test_safe_deref_member_const_lvalue_ref_nullable_non_scalar();
  int test_safe_deref_member_rvalue_ref_nullable_non_scalar();
  int test_safe_deref_member_const_rvalue_ref_nullable_non_scalar();
};

//
// RULE 2
// a value of nullable pointer type cannot be dereferenced unless it is null-safe
//
// testing all value categories, cv qualifiers, local and non-local, on all derefence operators

//
// deref operator

int test_unsafe_deref_lvalue_scalar_param(int *p) { // expected-note{{declared as nullable pointer here}}
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_scalar_param(int *const p) { // expected-note{{declared as nullable pointer here}}
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_scalar_param(int *volatile p) { // expected-note{{declared as nullable pointer here}}
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_scalar_param(int *const volatile p) { // expected-note{{declared as nullable pointer here}}
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_scalar_param(int *& p) { // expected-note{{declared as nullable pointer here}}
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_scalar_param(int *const& p) { // expected-note{{declared as nullable pointer here}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_scalar_param(int *volatile& p) { // expected-note{{declared as nullable pointer here}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_scalar_param(int *const volatile& p) { // expected-note{{declared as nullable pointer here}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_scalar_param(int *&& p) { // expected-note{{declared as nullable pointer here}}
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_scalar_param(int *const&& p) { // expected-note{{declared as nullable pointer here}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_scalar_param(int *volatile&& p) { // expected-note{{declared as nullable pointer here}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_scalar_param(int *const volatile&& p) { // expected-note{{declared as nullable pointer here}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_scalar_member(Member m) {
  return *m.member_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_scalar_member(Member m) {
  return *m.member_scalar_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                        // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_scalar_member(Member m) {
  return *m.member_scalar_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                           // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_scalar_member(Member m) {
  return *m.member_scalar_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_scalar_member(Member m) {
  return *m.member_scalar_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_scalar_global() {
  return *global_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_scalar_global() {
  return *global_scalar_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_scalar_global() {
  return *global_scalar_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_scalar_global() {
  return *global_scalar_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_scalar_global() {
  return *global_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_scalar_global() {
  return *global_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_scalar_global() {
  return *global_scalar_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_scalar_global() {
  return *global_scalar_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_scalar_global() {
  return *global_scalar_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_scalar_global() {
  return *global_scalar_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_scalar_global() {
  return *global_scalar_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_scalar_global() {
  return *global_scalar_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_scalar() {
  return *member_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_scalar() {
  return *member_scalar_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_scalar() {
  return *member_scalar_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_scalar() {
  return *member_scalar_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_ref_scalar() {
  return *member_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_ref_scalar() {
  return *member_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_ref_scalar() {
  return *member_scalar_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_ref_scalar() {
  return *member_scalar_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_rvalue_ref_scalar() {
  return *member_scalar_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_rvalue_ref_scalar() {
  return *member_scalar_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_rvalue_ref_scalar() {
  return *member_scalar_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_rvalue_ref_scalar() {
  return *member_scalar_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{dereference of a nullable pointer}}
}

//
// Function pointers

int test_unsafe_deref_lvalue_func_param(int (*fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_func_param(int (* const fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_func_param(int (* volatile fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_func_param(int (* const volatile fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_func_param(int (*& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_func_param(int (*const& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_func_param(int (*volatile& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_func_param(int (*const volatile& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_func_param(int (*&& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_func_param(int (*const&& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_func_param(int (*volatile&& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_func_param(int (*const volatile&& fp)()) { // expected-note{{declared as nullable pointer here}}
  return fp(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{called here}}
               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_func_member(Member m) {
  return m.member_func_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                 // expected-note@-1{{called here}}
                                 // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_func_member(Member m) {
  return m.member_func_const_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                       // expected-note@-1{{called here}}
                                       // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_func_member(Member m) {
  return m.member_func_volatile_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{called here}}
                                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_func_member(Member m) {
  return m.member_func_const_volatile_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                // expected-note@-1{{called here}}
                                                // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_func_member(Member m) {
  return m.member_func_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                     // expected-note@-1{{called here}}
                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_func_member(Member m) {
  return m.member_func_const_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                           // expected-note@-1{{called here}}
                                           // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_func_member(Member m) {
  return m.member_func_volatile_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{called here}}
                                              // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_func_member(Member m) {
  return m.member_func_const_volatile_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                    // expected-note@-1{{called here}}
                                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_func_member(Member m) {
  return m.member_func_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                     // expected-note@-1{{called here}}
                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_func_member(Member m) {
  return m.member_func_const_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                           // expected-note@-1{{called here}}
                                           // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_func_member(Member m) {
  return m.member_func_volatile_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{called here}}
                                              // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_func_member(Member m) {
  return m.member_func_const_volatile_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                    // expected-note@-1{{called here}}
                                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_func_global() {
  return global_func_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                               // expected-note@-1{{called here}}
                               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_func_global() {
  return global_func_const_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                     // expected-note@-1{{called here}}
                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_func_global() {
  return global_func_volatile_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                        // expected-note@-1{{called here}}
                                        // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_func_global() {
  return global_func_const_volatile_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{called here}}
                                              // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_func_global() {
  return global_func_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                   // expected-note@-1{{called here}}
                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_func_global() {
  return global_func_const_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{called here}}
                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_func_global() {
  return global_func_volatile_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{called here}}
                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_func_global() {
  return global_func_const_volatile_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{called here}}
                                                  // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_func_global() {
  return global_func_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                   // expected-note@-1{{called here}}
                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_func_global() {
  return global_func_const_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{called here}}
                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_func_global() {
  return global_func_volatile_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{called here}}
                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_func_global() {
  return global_func_const_volatile_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{called here}}
                                                  // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_func() {
  return member_func_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                               // expected-note@-1{{called here}}
                               // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_func() {
  return member_func_const_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                     // expected-note@-1{{called here}}
                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_func() {
  return member_func_volatile_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                        // expected-note@-1{{called here}}
                                        // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_func() {
  return member_func_const_volatile_lvalue(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{called here}}
                                              // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_ref_func() {
  return member_func_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                   // expected-note@-1{{called here}}
                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_ref_func() {
  return member_func_const_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{called here}}
                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_ref_func() {
  return member_func_volatile_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{called here}}
                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_ref_func() {
  return member_func_const_volatile_lvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{called here}}
                                                  // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_rvalue_ref_func() {
  return member_func_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                   // expected-note@-1{{called here}}
                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_rvalue_ref_func() {
  return member_func_const_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{called here}}
                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_rvalue_ref_func() {
  return member_func_volatile_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{called here}}
                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_rvalue_ref_func() {
  return member_func_const_volatile_rvalue_ref(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{called here}}
                                                  // expected-note@-2{{dereference of a nullable pointer}}
}

//
// Member function pointers

Member M;

int test_unsafe_deref_lvalue_member_func_param(int (Member::*mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_member_func_param(int (Member::*const mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_member_func_param(int (Member::*volatile mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_member_func_param(int (Member::*const volatile mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_member_func_param(int (Member::*& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_member_func_param(int (Member::*const& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_member_func_param(int (Member::*volatile& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_member_func_param(int (Member::*const volatile& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_member_func_param(int (Member::*&& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_member_func_param(int (Member::*const&& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_member_func_param(int (Member::*volatile&& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_member_func_param(int (Member::*const volatile&& mfp)()) { // expected-note{{declared as nullable pointer here}}
  return (M.*mfp)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_member_func_member(Member m) {
  return (M.*m.member_func_pointer_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_member_func_member(Member m) {
  return (M.*m.member_func_pointer_const_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_member_func_member(Member m) {
  return (M.*m.member_func_pointer_volatile_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_member_func_member(Member m) {
  return (M.*m.member_func_pointer_const_volatile_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_const_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_volatile_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_const_volatile_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                 // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_const_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_volatile_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_member_func_member(Member m) {
  return (M.*m.member_func_pointer_const_volatile_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                 // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_member_func_global() {
  return (M.*global_member_func_pointer_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_member_func_global() {
  return (M.*global_member_func_pointer_const_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_member_func_global() {
  return (M.*global_member_func_pointer_volatile_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_member_func_global() {
  return (M.*global_member_func_pointer_const_volatile_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_const_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_volatile_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_const_volatile_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_const_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_volatile_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_member_func_global() {
  return (M.*global_member_func_pointer_const_volatile_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_member_func() {
  return (M.*member_func_pointer_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_member_func() {
  return (M.*member_func_pointer_const_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_member_func() {
  return (M.*member_func_pointer_volatile_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_member_func() {
  return (M.*member_func_pointer_const_volatile_lvalue)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_ref_member_func() {
  return (M.*member_func_pointer_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_ref_member_func() {
  return (M.*member_func_pointer_const_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_ref_member_func() {
  return (M.*member_func_pointer_volatile_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_ref_member_func() {
  return (M.*member_func_pointer_const_volatile_lvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_rvalue_ref_member_func() {
  return (M.*member_func_pointer_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_rvalue_ref_member_func() {
  return (M.*member_func_pointer_const_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_rvalue_ref_member_func() {
  return (M.*member_func_pointer_volatile_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_rvalue_ref_member_func() {
  return (M.*member_func_pointer_const_volatile_rvalue_ref)(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                               // expected-note@-1{{dereference of a nullable pointer}}
}

//
// Member pointers

int test_unsafe_deref_lvalue_member_pointer_param(int Member::*mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_member_pointer_param(int Member::* const mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_member_pointer_param(int Member::* volatile mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_member_pointer_param(int Member::* const volatile mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_member_pointer_param(int Member::*& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_member_pointer_param(int Member::* const& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_member_pointer_param(int Member::* volatile& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_member_pointer_param(int Member::* const volatile& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_member_pointer_param(int Member::*&& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_member_pointer_param(int Member::* const&& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_member_pointer_param(int Member::* volatile&& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_member_pointer_param(int Member::* const volatile&& mp) { // expected-note{{declared as nullable pointer here}}
  return M.*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_member_pointer_member(Member m) {
  return M.*m.member_pointer_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                     // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_member_pointer_member(Member m) {
  return M.*m.member_pointer_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                           // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_member_pointer_member(Member m) {
  return M.*m.member_pointer_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_member_pointer_member(Member m) {
  return M.*m.member_pointer_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_member_pointer_member(Member m) {
  return M.*m.member_pointer_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_member_pointer_global() {
  return M.*global_member_pointer_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_member_pointer_global() {
  return M.*global_member_pointer_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_member_pointer_global() {
  return M.*global_member_pointer_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_member_pointer_global() {
  return M.*global_member_pointer_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_member_pointer_global() {
  return M.*global_member_pointer_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_member_pointer() {
  return M.*member_pointer_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                   // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_member_pointer() {
  return M.*member_pointer_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_member_pointer() {
  return M.*member_pointer_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_member_pointer() {
  return M.*member_pointer_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                  // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_ref_member_pointer() {
  return M.*member_pointer_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_ref_member_pointer() {
  return M.*member_pointer_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_ref_member_pointer() {
  return M.*member_pointer_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_ref_member_pointer() {
  return M.*member_pointer_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                      // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_rvalue_ref_member_pointer() {
  return M.*member_pointer_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                       // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_rvalue_ref_member_pointer() {
  return M.*member_pointer_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_rvalue_ref_member_pointer() {
  return M.*member_pointer_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                // expected-note@-1{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_rvalue_ref_member_pointer() {
  return M.*member_pointer_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                      // expected-note@-1{{dereference of a nullable pointer}}
}

//
// NullablePointer objects

int test_unsafe_deref_lvalue_nullable_scalar_param(NullableScalar p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_nullable_scalar_param(const NullableScalar p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_nullable_scalar_param(volatile NullableScalar p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_nullable_scalar_param(const volatile NullableScalar p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_nullable_scalar_param(NullableScalar &p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_nullable_scalar_param(const NullableScalar &p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_nullable_scalar_param(volatile NullableScalar &p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_nullable_scalar_param(const volatile NullableScalar &p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_nullable_scalar_param(NullableScalar &&p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_nullable_scalar_param(const NullableScalar &&p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_nullable_scalar_param(volatile NullableScalar &&p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_nullable_scalar_param(const volatile NullableScalar &&p) {
  return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullable pointer obtained via}}
             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                        // expected-note@-1{{nullable pointer obtained via}}
                                        // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                           // expected-note@-1{{nullable pointer obtained via}}
                                           // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{nullable pointer obtained via}}
                                                 // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{nullable pointer obtained via}}
                                      // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{nullable pointer obtained via}}
                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{nullable pointer obtained via}}
                                               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{nullable pointer obtained via}}
                                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{nullable pointer obtained via}}
                                      // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{nullable pointer obtained via}}
                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{nullable pointer obtained via}}
                                               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_nullable_scalar_member(Member m) {
  return *m.member_nullable_scalar_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{nullable pointer obtained via}}
                                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_nullable_scalar_global() {
  return *global_nullable_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_nullable_scalar_global() {
  return *global_nullable_scalar_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{nullable pointer obtained via}}
                                      // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_nullable_scalar_global() {
  return *global_nullable_scalar_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{nullable pointer obtained via}}
                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_nullable_scalar_global() {
  return *global_nullable_scalar_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{nullable pointer obtained via}}
                                               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_lvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_lvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{nullable pointer obtained via}}
                                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_lvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{nullable pointer obtained via}}
                                             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_lvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_rvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_rvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{nullable pointer obtained via}}
                                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_volatile_rvalue_ref_nullable_scalar_global() {
  return *global_nullable_scalar_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{nullable pointer obtained via}}
                                             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_deref_const_volatile_rvalue_ref_scalar_object_global() {
  return *global_nullable_scalar_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_nullable_scalar() {
  return *member_nullable_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_nullable_scalar() {
  return *member_nullable_scalar_const_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_nullable_scalar() {
  return *member_nullable_scalar_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                         // expected-note@-1{{nullable pointer obtained via}}
                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_nullable_scalar() {
  return *member_nullable_scalar_const_volatile_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                               // expected-note@-1{{nullable pointer obtained via}}
                                               // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_lvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_lvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{nullable pointer obtained via}}
                                          // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_lvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{nullable pointer obtained via}}
                                             // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_lvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_const_volatile_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_rvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_rvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_const_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                          // expected-note@-1{{nullable pointer obtained via}}
                                          // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_volatile_rvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                             // expected-note@-1{{nullable pointer obtained via}}
                                             // expected-note@-2{{dereference of a nullable pointer}}
}

int Member::test_unsafe_deref_member_const_volatile_rvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_const_volatile_rvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{dereference of a nullable pointer}}
}

//
// arrow operator

int test_unsafe_arrow_lvalue_non_scalar_param(NonScalar *p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_const_lvalue_non_scalar_param(NonScalar * const p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_volatile_lvalue_non_scalar_param(NonScalar * volatile p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_const_volatile_lvalue_non_scalar_param(NonScalar * const volatile p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_lvalue_ref_non_scalar_param(NonScalar *& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_const_lvalue_ref_non_scalar_param(NonScalar *const& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_volatile_lvalue_ref_non_scalar_param(NonScalar *volatile& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_non_scalar_param(NonScalar *const volatile& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_rvalue_ref_non_scalar_param(NonScalar *&& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_const_rvalue_ref_non_scalar_param(NonScalar *const&& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_volatile_rvalue_ref_non_scalar_param(NonScalar *volatile&& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_non_scalar_param(NonScalar *const volatile&& p) { // expected-note{{declared as nullable pointer here}}
  return p->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p') [amadeus.NullablePointers]}}
                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'p')}}
}

int test_unsafe_arrow_lvalue_non_scalar_member(Member m) {
  return m.member_non_scalar_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue') [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue')}}
}

int test_unsafe_arrow_const_lvalue_non_scalar_member(Member m) {
  return m.member_non_scalar_const_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue') [amadeus.NullablePointers]}}
                                                         // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue')}}
}

int test_unsafe_arrow_volatile_lvalue_non_scalar_member(Member m) {
  return m.member_non_scalar_volatile_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue') [amadeus.NullablePointers]}}
                                                            // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue')}}
}

int test_unsafe_arrow_const_volatile_lvalue_non_scalar_member(Member m) {
  return m.member_non_scalar_const_volatile_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue') [amadeus.NullablePointers]}}
                                                                  // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue')}}
}

int test_unsafe_arrow_lvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue_ref') [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue_ref')}}
}

int test_unsafe_arrow_const_lvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_const_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue_ref') [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue_ref')}}
}

int test_unsafe_arrow_volatile_lvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_volatile_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue_ref') [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue_ref')}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_const_volatile_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue_ref') [amadeus.NullablePointers]}}
                                                                      // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue_ref')}}
}

int test_unsafe_arrow_rvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_rvalue_ref') [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_rvalue_ref')}}
}

int test_unsafe_arrow_const_rvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_const_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_rvalue_ref') [amadeus.NullablePointers]}}
                                                             // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_rvalue_ref')}}
}

int test_unsafe_arrow_volatile_rvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_volatile_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_rvalue_ref') [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_rvalue_ref')}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_non_scalar_member(Member m) {
  return m.member_non_scalar_const_volatile_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_rvalue_ref') [amadeus.NullablePointers]}}
                                                                      // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_rvalue_ref')}}
}

int test_unsafe_arrow_lvalue_non_scalar_global() {
  return global_non_scalar_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_lvalue') [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_lvalue')}}
}

int test_unsafe_arrow_const_lvalue_non_scalar_global() {
  return global_non_scalar_const_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_lvalue') [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_lvalue')}}
}

int test_unsafe_arrow_volatile_lvalue_non_scalar_global() {
  return global_non_scalar_volatile_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_volatile_lvalue') [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_volatile_lvalue')}}
}

int test_unsafe_arrow_const_volatile_lvalue_non_scalar_global() {
  return global_non_scalar_const_volatile_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_volatile_lvalue') [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_volatile_lvalue')}}
}

int test_unsafe_arrow_lvalue_ref_non_scalar_global() {
  return global_non_scalar_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_lvalue_ref') [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_lvalue_ref')}}
}

int test_unsafe_arrow_const_lvalue_ref_non_scalar_global() {
  return global_non_scalar_const_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_lvalue_ref') [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_lvalue_ref')}}
}

int test_unsafe_arrow_volatile_lvalue_ref_non_scalar_global() {
  return global_non_scalar_volatile_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_volatile_lvalue_ref') [amadeus.NullablePointers]}}
                                                              // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_volatile_lvalue_ref')}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_non_scalar_global() {
  return global_non_scalar_const_volatile_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_volatile_lvalue_ref') [amadeus.NullablePointers]}}
                                                                    // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_volatile_lvalue_ref')}}
}

int test_unsafe_arrow_rvalue_ref_non_scalar_global() {
  return global_non_scalar_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_rvalue_ref') [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_rvalue_ref')}}
}

int test_unsafe_arrow_const_rvalue_ref_non_scalar_global() {
  return global_non_scalar_const_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_rvalue_ref') [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_rvalue_ref')}}
}

int test_unsafe_arrow_volatile_rvalue_ref_non_scalar_global() {
  return global_non_scalar_volatile_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_volatile_rvalue_ref') [amadeus.NullablePointers]}}
                                                              // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_volatile_rvalue_ref')}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_non_scalar_global() {
  return global_non_scalar_const_volatile_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_volatile_rvalue_ref') [amadeus.NullablePointers]}}
                                                                    // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from variable 'global_non_scalar_const_volatile_rvalue_ref')}}
}

int Member::test_unsafe_arrow_member_lvalue_non_scalar() {
  return member_non_scalar_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue') [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue')}}
}

int Member::test_unsafe_arrow_member_const_lvalue_non_scalar() {
  return member_non_scalar_const_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue') [amadeus.NullablePointers]}}
                                                       // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue')}}
}

int Member::test_unsafe_arrow_member_volatile_lvalue_non_scalar() {
  return member_non_scalar_volatile_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue') [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue')}}
}

int Member::test_unsafe_arrow_member_const_volatile_lvalue_non_scalar() {
  return member_non_scalar_const_volatile_lvalue->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue') [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue')}}
}

int Member::test_unsafe_arrow_member_lvalue_ref_non_scalar() {
  return member_non_scalar_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue_ref') [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_lvalue_ref')}}
}

int Member::test_unsafe_arrow_member_const_lvalue_ref_non_scalar() {
  return member_non_scalar_const_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue_ref') [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_lvalue_ref')}}
}

int Member::test_unsafe_arrow_member_volatile_lvalue_ref_non_scalar() {
  return member_non_scalar_volatile_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue_ref') [amadeus.NullablePointers]}}
                                                              // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_lvalue_ref')}}
}

int Member::test_unsafe_arrow_member_const_volatile_lvalue_ref_non_scalar() {
  return member_non_scalar_const_volatile_lvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue_ref') [amadeus.NullablePointers]}}
                                                                    // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_lvalue_ref')}}
}

int Member::test_unsafe_arrow_member_rvalue_ref_non_scalar() {
  return member_non_scalar_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_rvalue_ref') [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_rvalue_ref')}}
}

int Member::test_unsafe_arrow_member_const_rvalue_ref_non_scalar() {
  return member_non_scalar_const_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_rvalue_ref') [amadeus.NullablePointers]}}
                                                           // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_rvalue_ref')}}
}

int Member::test_unsafe_arrow_member_volatile_rvalue_ref_non_scalar() {
  return member_non_scalar_volatile_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_rvalue_ref') [amadeus.NullablePointers]}}
                                                              // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_volatile_rvalue_ref')}}
}

int Member::test_unsafe_arrow_member_const_volatile_rvalue_ref_non_scalar() {
  return member_non_scalar_const_volatile_rvalue_ref->member_field; // expected-warning{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_rvalue_ref') [amadeus.NullablePointers]}}
                                                                    // expected-note@-1{{Access to field 'member_field' results in a nullable pointer dereference (loaded from field 'member_non_scalar_const_volatile_rvalue_ref')}}
}

int test_unsafe_arrow_lvalue_nullable_non_scalar_param(NullableNonScalar p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_nullable_non_scalar_param(const NullableNonScalar p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_nullable_non_scalar_param(volatile NullableNonScalar p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_nullable_non_scalar_param(const volatile NullableNonScalar p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_lvalue_ref_nullable_non_scalar_param(NullableNonScalar &p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_ref_nullable_non_scalar_param(const NullableNonScalar &p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_ref_nullable_non_scalar_param(volatile NullableNonScalar &p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_nullable_non_scalar_param(const volatile NullableNonScalar &p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_rvalue_ref_nullable_non_scalar_param(NullableNonScalar &&p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_rvalue_ref_nullable_non_scalar_param(const NullableNonScalar &&p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_rvalue_ref_nullable_non_scalar_param(volatile NullableNonScalar &&p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_nullable_non_scalar_param(const volatile NullableNonScalar &&p) {
  return p->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{nullable pointer obtained via}}
                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_lvalue_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                            // expected-note@-1{{nullable pointer obtained via}}
                                                            // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_const_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                  // expected-note@-1{{nullable pointer obtained via}}
                                                                  // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_volatile_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                     // expected-note@-1{{nullable pointer obtained via}}
                                                                     // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_const_volatile_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                           // expected-note@-1{{nullable pointer obtained via}}
                                                                           // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_lvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{nullable pointer obtained via}}
                                                                // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_const_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                      // expected-note@-1{{nullable pointer obtained via}}
                                                                      // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_volatile_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                         // expected-note@-1{{nullable pointer obtained via}}
                                                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_const_volatile_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                               // expected-note@-1{{nullable pointer obtained via}}
                                                                               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_rvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{nullable pointer obtained via}}
                                                                // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_rvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_const_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                      // expected-note@-1{{nullable pointer obtained via}}
                                                                      // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_rvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_volatile_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                         // expected-note@-1{{nullable pointer obtained via}}
                                                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_nullable_non_scalar_member(Member m) {
  return m.member_nullable_non_scalar_const_volatile_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                               // expected-note@-1{{nullable pointer obtained via}}
                                                                               // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_lvalue_nullable_non_scalar_global() {
  return global_nullable_non_scalar_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{nullable pointer obtained via}}
                                                          // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_nullable_non_scalar_global() {
  return global_nullable_non_scalar_const_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                // expected-note@-1{{nullable pointer obtained via}}
                                                                // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_nullable_non_scalar_global() {
  return global_nullable_non_scalar_volatile_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                                   // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_nullable_non_scalar_global() {
  return global_nullable_non_scalar_const_volatile_lvalue->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                         // expected-note@-1{{nullable pointer obtained via}}
                                                                         // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_lvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                              // expected-note@-1{{nullable pointer obtained via}}
                                                              // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_const_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                    // expected-note@-1{{nullable pointer obtained via}}
                                                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_volatile_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                       // expected-note@-1{{nullable pointer obtained via}}
                                                                       // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_const_volatile_lvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                             // expected-note@-1{{nullable pointer obtained via}}
                                                                             // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_rvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                              // expected-note@-1{{nullable pointer obtained via}}
                                                              // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_rvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_const_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                    // expected-note@-1{{nullable pointer obtained via}}
                                                                    // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_rvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_volatile_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                       // expected-note@-1{{nullable pointer obtained via}}
                                                                       // expected-note@-2{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_nullable_non_scalar_global() {
  return global_nullable_non_scalar_const_volatile_rvalue_ref->member_field; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                                                             // expected-note@-1{{nullable pointer obtained via}}
                                                                             // expected-note@-2{{dereference of a nullable pointer}}
}

//
// arrow star operator

NonScalar P;

int test_unsafe_arrow_lvalue_non_scalar_param(int NonScalar::* mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_non_scalar_param(int NonScalar::* const mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_non_scalar_param(int NonScalar::* volatile mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_non_scalar_param(int NonScalar::* const volatile mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_lvalue_ref_non_scalar_param(int NonScalar::* &mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                 // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_lvalue_ref_non_scalar_param(int NonScalar::* const &mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_lvalue_ref_non_scalar_param(int NonScalar::* volatile &mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_lvalue_ref_non_scalar_param(int NonScalar::* const volatile &mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_rvalue_ref_non_scalar_param(int NonScalar::* &&mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_rvalue_ref_non_scalar_param(int NonScalar::* const &&mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_volatile_rvalue_ref_non_scalar_param(int NonScalar::* volatile &&mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

int test_unsafe_arrow_const_volatile_rvalue_ref_non_scalar_param(int NonScalar::* const volatile &&mp) { // expected-note{{declared as nullable pointer here}}
  return (&P)->*mp; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                    // expected-note@-1{{dereference of a nullable pointer}}
}

//
// RULE 1
// for all types T satisfying NullablePointer, gsl::not_null<T> is never equivalent nullptr and is always safe to dereference.
//

//
// deref operator

int test_safe_deref_lvalue_scalar_param(not_null<int *> p) {
  return *p;
}

int test_safe_deref_const_lvalue_scalar_param(const not_null<int *> p) {
  return *p;
}

int test_safe_deref_lvalue_ref_scalar_param(not_null<int *> &p) {
  return *p;
}

int test_safe_deref_const_lvalue_ref_scalar_param(const not_null<int *> &p) {
  return *p;
}

int test_safe_deref_rvalue_ref_scalar_param(not_null<int *> &&p) {
  return *p;
}

int test_safe_deref_const_rvalue_ref_scalar_param(const not_null<int *> &&p) {
  return *p;
}

int test_safe_deref_lvalue_scalar_member(NotNullMember m) {
  return *m.member_scalar_lvalue;
}

int test_safe_deref_const_lvalue_scalar_member(NotNullMember m) {
  return *m.member_scalar_const_lvalue;
}

int test_safe_deref_lvalue_ref_scalar_member(NotNullMember m) {
  return *m.member_scalar_lvalue_ref;
}

int test_safe_deref_const_lvalue_ref_scalar_member(NotNullMember m) {
  return *m.member_scalar_const_lvalue_ref;
}

int test_safe_deref_rvalue_ref_scalar_member(NotNullMember m) {
  return *m.member_scalar_rvalue_ref;
}

int test_safe_deref_const_rvalue_ref_scalar_member(NotNullMember m) {
  return *m.member_scalar_const_rvalue_ref;
}

int test_safe_deref_lvalue_scalar_global() {
  return *global_not_null_scalar_lvalue;
}

int test_safe_deref_const_lvalue_scalar_global() {
  return *global_not_null_scalar_const_lvalue;
}

int test_safe_deref_lvalue_ref_scalar_global() {
  return *global_not_null_scalar_lvalue_ref;
}

int test_safe_deref_const_lvalue_ref_scalar_global() {
  return *global_not_null_scalar_const_lvalue_ref;
}

int test_safe_deref_rvalue_ref_scalar_global() {
  return *global_not_null_scalar_rvalue_ref;
}

int test_safe_deref_const_rvalue_ref_scalar_global() {
  return *global_not_null_scalar_const_rvalue_ref;
}

int NotNullMember::test_safe_deref_member_lvalue_scalar() {
  return *member_scalar_lvalue;
}

int NotNullMember::test_safe_deref_member_const_lvalue_scalar() {
  return *member_scalar_const_lvalue;
}

int NotNullMember::test_safe_deref_member_lvalue_ref_scalar() {
  return *member_scalar_lvalue_ref;
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_scalar() {
  return *member_scalar_const_lvalue_ref;
}

int NotNullMember::test_safe_deref_member_rvalue_ref_scalar() {
  return *member_scalar_rvalue_ref;
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_scalar() {
  return *member_scalar_const_rvalue_ref;
}

//
// Function pointers

int test_safe_deref_lvalue_func_param(not_null<int (*)()> fp) {
  return fp();
}

int test_safe_deref_const_lvalue_func_param(const not_null<int (*)()> fp) {
  return fp();
}

int test_safe_deref_lvalue_ref_func_param(not_null<int (*)()> &fp) {
  return fp();
}

int test_safe_deref_const_lvalue_ref_func_param(const not_null<int (*)()> &fp) {
  return fp();
}

int test_safe_deref_rvalue_ref_func_param(not_null<int (*)()> &&fp) {
  return fp();
}

int test_safe_deref_const_rvalue_ref_func_param(const not_null<int (*)()> &&fp) {
  return fp();
}

int test_safe_deref_lvalue_func_member(NotNullMember m) {
  return m.member_func_lvalue();
}

int test_safe_deref_const_lvalue_func_member(NotNullMember m) {
  return m.member_func_const_lvalue();
}

int test_safe_deref_lvalue_ref_func_member(NotNullMember m) {
  return m.member_func_lvalue_ref();
}

int test_safe_deref_const_lvalue_ref_func_member(NotNullMember m) {
  return m.member_func_const_lvalue_ref();
}

int test_safe_deref_rvalue_ref_func_member(NotNullMember m) {
  return m.member_func_rvalue_ref();
}

int test_safe_deref_const_rvalue_ref_func_member(NotNullMember m) {
  return m.member_func_const_rvalue_ref();
}

int test_safe_deref_lvalue_func_global() {
  return global_not_null_func_lvalue();
}

int test_safe_deref_const_lvalue_func_global() {
  return global_not_null_func_const_lvalue();
}

int test_safe_deref_lvalue_ref_func_global() {
  return global_not_null_func_lvalue_ref();
}

int test_safe_deref_const_lvalue_ref_func_global() {
  return global_not_null_func_const_lvalue_ref();
}

int test_safe_deref_rvalue_ref_func_global() {
  return global_not_null_func_rvalue_ref();
}

int test_safe_deref_const_rvalue_ref_func_global() {
  return global_not_null_func_const_rvalue_ref();
}

int NotNullMember::test_safe_deref_member_lvalue_func() {
  return member_func_lvalue();
}

int NotNullMember::test_safe_deref_member_const_lvalue_func() {
  return member_func_const_lvalue();
}

int NotNullMember::test_safe_deref_member_lvalue_ref_func() {
  return member_func_lvalue_ref();
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_func() {
  return member_func_const_lvalue_ref();
}

int NotNullMember::test_safe_deref_member_rvalue_ref_func() {
  return member_func_rvalue_ref();
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_func() {
  return member_func_const_rvalue_ref();
}

//
// Member Function pointers

int test_safe_deref_lvalue_member_func_param(not_null<int (Member::*)()> mfp) {
  return (M.*mfp.get())();
}

int test_safe_deref_const_lvalue_member_func_param(const not_null<int (Member::*)()> mfp) {
  return (M.*mfp.get())();
}

int test_safe_deref_lvalue_ref_member_func_param(not_null<int (Member::*)()> &mfp) {
  return (M.*mfp.get())();
}

int test_safe_deref_const_lvalue_ref_member_func_param(const not_null<int (Member::*)()> &mfp) {
  return (M.*mfp.get())();
}

int test_safe_deref_rvalue_ref_member_func_param(not_null<int (Member::*)()> &&mfp) {
  return (M.*mfp.get())();
}

int test_safe_deref_const_rvalue_ref_member_func_param(const not_null<int (Member::*)()> &&mfp) {
  return (M.*mfp.get())();
}

int test_safe_deref_lvalue_member_func_member(NotNullMember m) {
  return (M.*m.member_func_pointer_lvalue.get())();
}

int test_safe_deref_const_lvalue_member_func_member(NotNullMember m) {
  return (M.*m.member_func_pointer_const_lvalue.get())();
}

int test_safe_deref_lvalue_ref_member_func_member(NotNullMember m) {
  return (M.*m.member_func_pointer_lvalue_ref.get())();
}

int test_safe_deref_const_lvalue_ref_member_func_member(NotNullMember m) {
  return (M.*m.member_func_pointer_const_lvalue_ref.get())();
}

int test_safe_deref_rvalue_ref_member_func_member(NotNullMember m) {
  return (M.*m.member_func_pointer_rvalue_ref.get())();
}

int test_safe_deref_const_rvalue_ref_member_func_member(NotNullMember m) {
  return (M.*m.member_func_pointer_const_rvalue_ref.get())();
}

int test_safe_deref_lvalue_member_func_global() {
  return (M.*global_not_null_member_func_lvalue.get())();
}

int test_safe_deref_const_lvalue_member_func_global() {
  return (M.*global_not_null_member_func_const_lvalue.get())();
}

int test_safe_deref_lvalue_ref_member_func_global() {
  return (M.*global_not_null_member_func_lvalue_ref.get())();
}

int test_safe_deref_const_lvalue_ref_member_func_global() {
  return (M.*global_not_null_member_func_const_lvalue_ref.get())();
}

int test_safe_deref_rvalue_ref_member_func_global() {
  return (M.*global_not_null_member_func_rvalue_ref.get())();
}

int test_safe_deref_const_rvalue_ref_member_func_global() {
  return (M.*global_not_null_member_func_const_rvalue_ref.get())();
}

int NotNullMember::test_safe_deref_member_lvalue_member_func() {
  return (M.*member_func_pointer_lvalue.get())();
}

int NotNullMember::test_safe_deref_member_const_lvalue_member_func() {
  return (M.*member_func_pointer_const_lvalue.get())();
}

int NotNullMember::test_safe_deref_member_lvalue_ref_member_func() {
  return (M.*member_func_pointer_lvalue_ref.get())();
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_member_func() {
  return (M.*member_func_pointer_const_lvalue_ref.get())();
}

int NotNullMember::test_safe_deref_member_rvalue_ref_member_func() {
  return (M.*member_func_pointer_rvalue_ref.get())();
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_member_func() {
  return (M.*member_func_pointer_const_rvalue_ref.get())();
}

//
// Member pointers

int test_safe_deref_lvalue_member_pointer_param(not_null<int Member::*> mp) {
  return M.*mp.get();
}

int test_safe_deref_const_lvalue_member_pointer_param(const not_null<int Member::*> mp) {
  return M.*mp.get();
}

int test_safe_deref_lvalue_ref_member_pointer_param(not_null<int Member::*> &mp) {
  return M.*mp.get();
}

int test_safe_deref_const_lvalue_ref_member_pointer_param(const not_null<int Member::*> &mp) {
  return M.*mp.get();
}


int test_safe_deref_rvalue_ref_member_pointer_param(not_null<int Member::*> &&mp) {
  return M.*mp.get();
}

int test_safe_deref_const_rvalue_ref_member_pointer_param(const not_null<int Member::*> &&mp) {
  return M.*mp.get();
}

int test_safe_deref_lvalue_member_pointer_member(NotNullMember m) {
  return M.*m.member_pointer_lvalue.get();
}

int test_safe_deref_const_lvalue_member_pointer_member(NotNullMember m) {
  return M.*m.member_pointer_const_lvalue.get();
}

int test_safe_deref_lvalue_ref_member_pointer_member(NotNullMember m) {
  return M.*m.member_pointer_lvalue_ref.get();
}

int test_safe_deref_const_lvalue_ref_member_pointer_member(NotNullMember m) {
  return M.*m.member_pointer_const_lvalue_ref.get();
}

int test_safe_deref_rvalue_ref_member_pointer_member(NotNullMember m) {
  return M.*m.member_pointer_rvalue_ref.get();
}

int test_safe_deref_const_rvalue_ref_member_pointer_member(NotNullMember m) {
  return M.*m.member_pointer_const_rvalue_ref.get();
}

int test_safe_deref_lvalue_member_pointer_global() {
  return M.*global_not_null_member_pointer_lvalue.get();
}

int test_safe_deref_const_lvalue_member_pointer_global() {
  return M.*global_not_null_member_pointer_const_lvalue.get();
}

int test_safe_deref_lvalue_ref_member_pointer_global() {
  return M.*global_not_null_member_pointer_lvalue_ref.get();
}

int test_safe_deref_const_lvalue_ref_member_pointer_global() {
  return M.*global_not_null_member_pointer_const_lvalue_ref.get();
}

int test_safe_deref_rvalue_ref_member_pointer_global() {
  return M.*global_not_null_member_pointer_rvalue_ref.get();
}

int test_safe_deref_const_rvalue_ref_member_pointer_global() {
  return M.*global_not_null_member_pointer_const_rvalue_ref.get();
}

int NotNullMember::test_safe_deref_member_lvalue_member_pointer() {
  return M.*member_pointer_lvalue.get();
}

int NotNullMember::test_safe_deref_member_const_lvalue_member_pointer() {
  return M.*member_pointer_const_lvalue.get();
}

int NotNullMember::test_safe_deref_member_lvalue_ref_member_pointer() {
  return M.*member_pointer_lvalue_ref.get();
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_member_pointer() {
  return M.*member_pointer_const_lvalue_ref.get();
}

int NotNullMember::test_safe_deref_member_rvalue_ref_member_pointer() {
  return M.*member_pointer_rvalue_ref.get();
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_member_pointer() {
  return M.*member_pointer_const_rvalue_ref.get();
}

//
// NullablePointers scalar

int test_safe_deref_lvalue_nullable_scalar_param(not_null<NullableScalar> p) {
  return *p;
}

int test_safe_deref_const_lvalue_nullable_scalar_param(const not_null<NullableScalar> p) {
  return *p;
}

int test_safe_deref_lvalue_ref_nullable_scalar_param(not_null<NullableScalar> &p) {
  return *p;
}

int test_safe_deref_const_lvalue_ref_nullable_scalar_param(const not_null<NullableScalar> &p) {
  return *p;
}

int test_safe_deref_rvalue_ref_nullable_scalar_param(not_null<NullableScalar> &&p) {
  return *p;
}

int test_safe_deref_const_rvalue_ref_nullable_scalar_param(const not_null<NullableScalar> &&p) {
  return *p;
}

int test_safe_deref_lvalue_nullable_scalar_member(NotNullMember m) {
  return *m.member_nullable_scalar_lvalue;
}

int test_safe_deref_const_lvalue_nullable_scalar_member(NotNullMember m) {
  return *m.member_nullable_scalar_const_lvalue;
}

int test_safe_deref_lvalue_ref_nullable_scalar_member(NotNullMember m) {
  return *m.member_nullable_scalar_lvalue_ref;
}

int test_safe_deref_const_lvalue_ref_nullable_scalar_member(NotNullMember m) {
  return *m.member_nullable_scalar_const_lvalue_ref;
}

int test_safe_deref_rvalue_ref_nullable_scalar_member(NotNullMember m) {
  return *m.member_nullable_scalar_rvalue_ref;
}

int test_safe_deref_const_rvalue_ref_nullable_scalar_member(NotNullMember m) {
  return *m.member_nullable_scalar_const_rvalue_ref;
}

int test_safe_deref_lvalue_nullable_scalar_global() {
  return *global_not_null_nullable_scalar_lvalue;
}

int test_safe_deref_const_lvalue_nullable_scalar_global() {
  return *global_not_null_nullable_scalar_const_lvalue;
}

int test_safe_deref_lvalue_ref_nullable_scalar_global() {
  return *global_not_null_nullable_scalar_lvalue_ref;
}

int test_safe_deref_const_lvalue_ref_nullable_scalar_global() {
  return *global_not_null_nullable_scalar_const_lvalue_ref;
}

int test_safe_deref_rvalue_ref_nullable_scalar_global() {
  return *global_not_null_nullable_scalar_rvalue_ref;
}

int test_safe_deref_const_rvalue_ref_nullable_scalar_global() {
  return *global_not_null_nullable_scalar_const_rvalue_ref;
}

int NotNullMember::test_safe_deref_member_lvalue_nullable_scalar() {
  return *member_nullable_scalar_lvalue;
}

int NotNullMember::test_safe_deref_member_const_lvalue_nullable_scalar() {
  return *member_nullable_scalar_const_lvalue;
}

int NotNullMember::test_safe_deref_member_lvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_lvalue_ref;
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_const_lvalue_ref;
}

int NotNullMember::test_safe_deref_member_rvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_rvalue_ref;
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_nullable_scalar() {
  return *member_nullable_scalar_const_rvalue_ref;
}

//
// arrow operator

int test_safe_deref_lvalue_non_scalar_param(not_null<NonScalar *> p) {
  return p->member_field;
}

int test_safe_deref_const_lvalue_non_scalar_param(const not_null<NonScalar *> p) {
  return p->member_field;
}

int test_safe_deref_lvalue_ref_non_scalar_param(not_null<NonScalar *> &p) {
  return p->member_field;
}

int test_safe_deref_const_lvalue_ref_non_scalar_param(const not_null<NonScalar *> &p) {
  return p->member_field;
}

int test_safe_deref_rvalue_ref_non_scalar_param(not_null<NonScalar *> &&p) {
  return p->member_field;
}

int test_safe_deref_const_rvalue_ref_non_scalar_param(const not_null<NonScalar *> &&p) {
  return p->member_field;
}

int test_safe_deref_lvalue_non_scalar_member(NotNullMember m) {
  return m.member_non_scalar_lvalue->member_field;
}

int test_safe_deref_const_lvalue_non_scalar_member(NotNullMember m) {
  return m.member_non_scalar_const_lvalue->member_field;
}

int test_safe_deref_lvalue_ref_non_scalar_member(NotNullMember m) {
  return m.member_non_scalar_lvalue_ref->member_field;
}

int test_safe_deref_const_lvalue_ref_non_scalar_member(NotNullMember m) {
  return m.member_non_scalar_const_lvalue_ref->member_field;
}

int test_safe_deref_rvalue_ref_non_scalar_member(NotNullMember m) {
  return m.member_non_scalar_rvalue_ref->member_field;
}

int test_safe_deref_const_rvalue_ref_non_scalar_member(NotNullMember m) {
  return m.member_non_scalar_const_rvalue_ref->member_field;
}

int test_safe_deref_lvalue_non_scalar_global() {
  return global_not_null_non_scalar_lvalue->member_field;
}

int test_safe_deref_const_lvalue_non_scalar_global() {
  return global_not_null_non_scalar_const_lvalue->member_field;
}

int test_safe_deref_lvalue_ref_non_scalar_global() {
  return global_not_null_non_scalar_lvalue_ref->member_field;
}

int test_safe_deref_const_lvalue_ref_non_scalar_global() {
  return global_not_null_non_scalar_const_lvalue_ref->member_field;
}

int test_safe_deref_rvalue_ref_non_scalar_global() {
  return global_not_null_non_scalar_rvalue_ref->member_field;
}

int test_safe_deref_const_rvalue_ref_non_scalar_global() {
  return global_not_null_non_scalar_const_rvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_lvalue_non_scalar() {
  return member_non_scalar_lvalue->member_field;
}

int NotNullMember::test_safe_deref_member_const_lvalue_non_scalar() {
  return member_non_scalar_const_lvalue->member_field;
}

int NotNullMember::test_safe_deref_member_lvalue_ref_non_scalar() {
  return member_non_scalar_lvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_non_scalar() {
  return member_non_scalar_const_lvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_rvalue_ref_non_scalar() {
  return member_non_scalar_rvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_non_scalar() {
  return member_non_scalar_const_rvalue_ref->member_field;
}

int test_safe_deref_lvalue_nullable_non_scalar_param(not_null<NullableNonScalar> p) {
  return p->member_field;
}

int test_safe_deref_const_lvalue_nullable_non_scalar_param(const not_null<NullableNonScalar> p) {
  return p->member_field;
}

int test_safe_deref_lvalue_ref_nullable_non_scalar_param(not_null<NullableNonScalar> &p) {
  return p->member_field;
}

int test_safe_deref_const_lvalue_ref_nullable_non_scalar_param(const not_null<NullableNonScalar> &p) {
  return p->member_field;
}

int test_safe_deref_rvalue_ref_nullable_non_scalar_param(not_null<NullableNonScalar> &&p) {
  return p->member_field;
}

int test_safe_deref_const_rvalue_ref_nullable_non_scalar_param(const not_null<NullableNonScalar> &&p) {
  return p->member_field;
}

int test_safe_deref_lvalue_nullable_non_scalar_member(NotNullMember m) {
  return m.member_nullable_non_scalar_lvalue->member_field;
}

int test_safe_deref_const_lvalue_nullable_non_scalar_member(NotNullMember m) {
  return m.member_nullable_non_scalar_const_lvalue->member_field;
}

int test_safe_deref_lvalue_ref_nullable_non_scalar_member(NotNullMember m) {
  return m.member_nullable_non_scalar_lvalue_ref->member_field;
}

int test_safe_deref_const_lvalue_ref_nullable_non_scalar_member(NotNullMember m) {
  return m.member_nullable_non_scalar_const_lvalue_ref->member_field;
}

int test_safe_deref_rvalue_ref_nullable_non_scalar_member(NotNullMember m) {
  return m.member_nullable_non_scalar_rvalue_ref->member_field;
}

int test_safe_deref_const_rvalue_ref_nullable_non_scalar_member(NotNullMember m) {
  return m.member_nullable_non_scalar_const_rvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_lvalue_nullable_non_scalar() {
  return member_nullable_non_scalar_lvalue->member_field;
}

int NotNullMember::test_safe_deref_member_const_lvalue_nullable_non_scalar() {
  return member_nullable_non_scalar_const_lvalue->member_field;
}

int NotNullMember::test_safe_deref_member_lvalue_ref_nullable_non_scalar() {
  return member_nullable_non_scalar_lvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_const_lvalue_ref_nullable_non_scalar() {
  return member_nullable_non_scalar_const_lvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_rvalue_ref_nullable_non_scalar() {
  return member_nullable_non_scalar_rvalue_ref->member_field;
}

int NotNullMember::test_safe_deref_member_const_rvalue_ref_nullable_non_scalar() {
  return member_nullable_non_scalar_const_rvalue_ref->member_field;
}

int test_safe_deref_lvalue_nullable_non_scalar_global() {
  return global_not_null_nullable_non_scalar_lvalue->member_field;
}

int test_safe_deref_const_lvalue_nullable_non_scalar_global() {
  return global_not_null_nullable_non_scalar_const_lvalue->member_field;
}

int test_safe_deref_lvalue_ref_nullable_non_scalar_global() {
  return global_not_null_nullable_non_scalar_lvalue_ref->member_field;
}

int test_safe_deref_const_lvalue_ref_nullable_non_scalar_global() {
  return global_not_null_nullable_non_scalar_const_lvalue_ref->member_field;
}

int test_safe_deref_rvalue_ref_nullable_non_scalar_global() {
  return global_not_null_nullable_non_scalar_rvalue_ref->member_field;
}

int test_safe_deref_const_rvalue_ref_nullable_non_scalar_global() {
  return global_not_null_nullable_non_scalar_const_rvalue_ref->member_field;
}

//
// FIXME Add ->* test cases


//
// RULE 5
// always null-safe prvalues

struct Object {
  Object &test_safe_deref_this() {
    return *this;
  }

  int test_safe_member_this() {
    return this->member;
  }

  int test_safe_conversion_to_non_nullable_this() {
    not_null<Object*> p = this;
    return p->member;
  }

  int test_safe_arg_implicit_to_non_nullable_this() {
    return take_non_nullable(this);
  }

  int test_safe_assignment_to_non_nullable_this(not_null<Object*> p) {
    p = this;
    return p->member;
  }

  int test_safe_deref_builtin_address_of() {
    return *(&member);
  }

  int test_safe_member_builtin_address_of() {
    Object o;
    return (&o)->member;
  }

  int test_safe_convertion_to_non_nullable_builtin_adress_of() {
    Object o;
    not_null<Object*> p = &o;
    return p->member;
  }

  int test_safe_arg_implicit_to_non_nullable_builtin_address_of() {
    Object o;
    return take_non_nullable(&o);
  }

  int test_safe_assignment_to_non_nullable_builtin_address_of(not_null<Object*> p) {
    Object o;
    p = &o;
    return p->member;
  }

  static Object &test_safe_deref_builtin_throw_new() {
    return *(new Object {});
  }

  static int test_safe_member_builtin_throw_new() {
    return (new Object {})->member;
  }

  int test_safe_conversion_to_non_nullable_builtin_throw_new() {
    not_null<Object*> p = new Object{};
    return p->member;
  }

  int test_safe_arg_implicit_to_non_nullable_builtin_throw_new() {
    return take_non_nullable(new Object{});
  }

  int test_safe_assignment_to_non_nullable_builtin_throw_new(not_null<Object*> p) {
    p = new Object {};
    return p->member;
  }

  int test_safe_deref_array_to_pointer() {
    int x[2] = {1, 2};

    return *x;
  }

  int test_safe_mebmer_array_to_pointer() {
    Object x[2] = {Object{}, Object{}};
    return x->member;
  }

  int test_safe_conversion_to_non_nullable_array_to_pointer() {
    int x[2] = {1, 2};
    not_null<int*> p = x;
    return *p;
  }

  int test_safe_arg_implicit_to_non_nullable_array_to_pointer() {
    Object x[2] = {Object{}, Object{}};
    return take_non_nullable(x);
  }

  //FIXME create sufficient tests involving ElementRegions
  int test_safe_assignment_to_non_nullable_array_to_pointer(not_null<Object*> p) {
    Object x[2] = {Object{}, Object{}};
    p = x;
    return p->member;
  }

  int test_safe_deref_function_to_pointer() {
    auto f = static_member_fun;
    return f();
  }

  int test_safe_conversion_to_non_nullable_function_to_pointer() {
    auto f = static_member_fun;
    not_null<decltype(f)> p = f;
    return p();
  }

  int test_safe_arg_implicit_to_non_nullable_function_to_pointer() {
    return take_non_nullable(static_member_fun);
  }

  int test_safe_assignment_to_non_nullable_function_to_pointer(not_null<int(*)()> p) {
    p = static_member_fun;
    return p();
  }

  int take_non_nullable(not_null<Object*> p);
  int take_non_nullable(not_null<int(*)()> p);
  int member;
  static int static_member_fun();
};

int test_safe_deref_conversion_from_non_nullable(not_null<int*> p) {
  int *up = p;
  return *up;
}

int test_safe_member_conversion_from_non_nullable(not_null<Object*> p) {
  Object *up = p;
  return up->member;
}

int take_nullable(int *p);

int test_safe_arg_implicit_to_nullable(not_null<int*> p) {
  return take_nullable(p);
}

//
// RULE 6
// All other rvalues are never null-safe

int test_unsafe_deref_rvalue_no_proof() {
  if (get_scalar()) { // expected-note{{Assuming the condition is true}}
                      // expected-note@-1{{Taking true branch}}
    return *get_scalar(); // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                          // expected-note@-1{{called here}}
                          // expected-note@-2{{dereference of a nullable pointer}}
  }
  return 0;
}

// FIXME add tests other pointer types

//
// RULE 7
// an lvalue reference to volatile nullable pointer is never null-safe

int test_unsafe_deref_volatile_lvalue_ref_no_proof_param(int * volatile &p) { // expected-note{{declared as nullable pointer here}}
  if (p) return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
  // expected-note@-1{{Assigning value}}
  // expected-note@-2{{Assuming 'p' is non-null}}
  // expected-note@-3{{Taking true branch}}
  // expected-note@-4{{nullable-safety proof established from this point}}
  // expected-note@-5{{dereference of a nullable pointer}}
  return 0;
}

int test_unsafe_deref_volatile_const_lvalue_ref_no_proof_param(int * const volatile &p) { // expected-note{{declared as nullable pointer here}}
  if (p) return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
  // expected-note@-1{{Assigning value}}
  // expected-note@-2{{Assuming 'p' is non-null}}
  // expected-note@-3{{Taking true branch}}
  // expected-note@-4{{nullable-safety proof established from this point}}
  // expected-note@-5{{dereference of a nullable pointer}}
  return 0;
}

// FIXME add tests for other pointer types / global / member

//
// RULE 8
// a null-safety proof for a nullable pointer v is established by:
//

//
// RULE 8.1
// initializing from a null-safe value when v is a local nullable pointer lvalue or a local reference to nullable pointer (v is a local variable)
//

int test_safe_deref_proof_from_null_safe_init_param(int *p) {
  if (p) {
    int *u = p;
    return *u;
  }
  return 0;
}

// FIXME add tests for other pointer types / global / member

//
// RULE 8.2
// assignment from a null-safe value when v is a local nullable pointer lvalue or a local non-const nullable pointer lvalue reference
//

int test_safe_deref_proof_from_null_safe_assignment_param(int *p) {
  int *u = nullptr;
  if (p) {
    u = p;
    return *u;
  }
  return 0;
}

//
// RULE 8.3
// when `v` is one of:
// a. lvalue
// b. lvalue reference non-volatile
// c. lvalue reference to a local, non-escaped volatile pointer (see ...)
// d. rvalue reference
// within a boolean expression:
//

//
// RULE 8.3.1
// `e1 && e2` where `e1` is `true` when `v != nullptr` a null-safety proof for `v` is established in `e2`
//

bool test_safe_deref_lvalue_proof_from_boolean_and_param(int *p) {
  return p && *p > 0;
}

bool test_unsafe_deref_lvalue_misproof_from_boolean_and_param(int *p) { // expected-note{{declared as nullable pointer here}}
  return !p && *p > 0;  // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
                        // expected-note@-1{{nullptr dereference of a nullable pointer}}
                        // expected-note@-2{{Assuming 'p' is null}}
                        // expected-note@-3{{Left side of '&&' is true}}
}

//
// RULE 8.3.2
// `e1 || e2` where `e1` is `false` when `v != nullptr` a null-safety proof `v` is established in `e2`
//

bool test_safe_deref_lvalue_proof_from_boolean_or_param(int *p) {
  return !p || *p > 0;
}

bool test_unsafe_deref_lvalue_misproof_from_boolean_or_param(int *p) { // expected-note{{declared as nullable pointer here}}
  return p || *p > 0;  // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
                       // expected-note@-1{{nullptr dereference of a nullable pointer}}
                       // expected-note@-2{{Assuming 'p' is null}}
                       // expected-note@-3{{Left side of '||' is false}}
}

//
// RULE 8.4
// when `v` is one of:
// a. lvalue
// b. lvalue reference non-volatile
// c. lvalue reference to a local, non-escaped volatile pointer (see ...)
// d. rvalue reference
//
// branching in the form of:
//

//
// RULE 8.4.1
// an assertion `assert(condition)` where `condition` is `true` when `v != nullptr`

int test_safe_deref_lvalue_proof_via_assert_param(int *p) {
  assert(p);
  return *p;
}

// FIXME add tests for other pointer types / global / member

//
// RULE 8.4.2
// a conditional expression `c ? a : b` where the result of `c` depends on `v == nullptr`
//

//
// RULE 8.4.2.1
// when `c` is `true` when `v != nullptr` a null-safety proof for `v` is established in `a`.
//

int test_safe_deref_lvalue_proof_via_conditional_true_branch_param(int *p) {
  return p ? *p : 0;
}

int test_unsafe_deref_lvalue_misproof_via_conditional_true_branch_param(int *p) { // expected-note{{declared as nullable pointer here}}
  return p ? 0 : *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{Assuming 'p' is null}}
                     // expected-note@-2{{'?' condition is false}}
                     // expected-note@-3{{nullptr dereference of a nullable pointer}}
}

// FIXME add tests for other pointer types / global / member

//
// RULE 8.4.2.2
// when `c` is `false` when `v != nullptr` a null-safety proof for `v` is established in `b`
//

int test_safe_deref_lvalue_proof_via_conditional_false_branch_param(int *p) {
  return !p ? 0 : *p;
}

int test_unsafe_deref_lvalue_misproof_via_conditional_false_branch_param(int *p) { // expected-note{{declared as nullable pointer here}}
  return !p ? *p : 0; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
                     // expected-note@-1{{Assuming 'p' is null}}
                     // expected-note@-2{{'?' condition is true}}
                     // expected-note@-3{{nullptr dereference of a nullable pointer}}
}

// FIXME add tests for other pointer types / global / member

//
// RULE 8.4.3
// an if statement `if (c) s` when `c` is `true` when `v != nullptr` a null-safety proof for `v` is established in `s`
//

int test_safe_deref_lvalue_proof_via_if_true_branch_param(int *p) {
  if (p) {
    return *p;
  }
  return 0;
}

int test_unssafe_deref_lvalue_misproof_via_if_true_branch_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is null}}
           // expected-note@-1{{Taking false branch}}
    return 0;
  }
  return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
             // expected-note@-1{{nullptr dereference of a nullable pointer}}
}

//
// RULE 8.4.4
// an if statement `if (c) s1 else s2` when `c` is `false` when `v == nullptr` a null-safety proof for `v` is established in `s2`
//



//
// RULE 8.3.3.1.1
// if the false sub-statement terminates with a return statement (in all control-flow paths), and v is not initialized in c,
// then the scope of null-safety proof for v applies to all statements following if
//

int test_safe_deref_lvalue_proof_via_if_true_branch_outside_param(int *p) {
  if (p) {

  } else {
    return 0;
  }

  return *p;
}

//
// RULE 8.3.3.2
// when c is true if v == nullptr the a null-safety proof for v is established in the false substatement
//

int test_safe_deref_lvalue_proof_via_if_false_branch_param(int *p) {
  if (!p) {
    return 0;
  }
  return *p;
}

int test_unssafe_deref_lvalue_misproof_via_if_false_branch_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (!p) { // expected-note{{Assuming 'p' is null}}
            // expected-note@-1{{Taking true branch}}
    return *p; // expected-warning{{nullptr dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{nullptr dereference of a nullable pointer}}
  }
  return 0;
}

//
// RULE 8.3.3.2.1
// if the true sub-statement terminates with a return statement (in all control-flow paths) and v is not initialized in c,
// the scope of null-safety proof for v applies to all subsequent statements following if
//

int test_safe_deref_lvalue_proof_via_if_false_branch_outside_param(int *p) {
  if (!p) {
    return 0;
  } else {

  }

  return *p;
}

//
// RULE 8.3.4
// a switch statement where the result of the condition expression c depends on the result of v == nullptr
//

//
// RULE 8.3.4.1
// when c is false if v == nullptr then a null-safety proof for v is established within the case statement that matches c
// and any subsequent case statement that is reachable from the absence of a break statement.
//

int test_safe_deref_lvalue_proof_via_switch_not_null_condition_int_case_param(int *p) {
  switch ((long)p) {
  case 1:
    return *p;
  default:
    return 0;
  }
}

int test_safe_deref_lvalue_proof_via_switch_not_null_condition_int_case_outside_param(int *p) {
  switch ((long)p) {
  case 1:
    break;
  default:
    return 0;
  }
  return *p;
}


//
// RULE 8.3.4.2
// when c is true if v == nullptr then a null-safety proof for v is established within all case statements that do not match c
// and any subsequent case statemets that are reachable from the absence of a break statement.
//

int test_safe_deref_lvalue_proof_via_switch_null_condition_int_case_param(int *p) {
  switch ((long)p) {
  case 0:
    return 0;
  default:
    return *p;
  }
}

int test_safe_deref_lvalue_proof_via_switch_null_condition_int_case_outside_param(int *p) {
  switch ((long)p) {
  case 0:
    return 0;
  default:
    break;
  }
  return *p;
}


//
// RULE 12
// A null-safety proof is not permanent and can be invalidated by subsequent statements; a nullable pointer that has its null-safety proof invalidated is no longer null-safe.
// A null-safety proof is invalidated by:
//

//
// RULE 12.1
// Assignment of `nullptr` to a nullable pointer invalidates any null-safety proofs for the pointer.
//

int test_unsafe_deref_lvalue_proof_from_non_null_init_invalidation_param(not_null<int*> q, int *r) {
  int v = 0;
  int *p = q; // expected-note{{declared as nullable pointer here}}

  v = *p;

  p = r; // expected-note{{Value assigned to 'p'}}

  v = *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
          // expected-note@-1{{dereference of a nullable pointer}}
  return v;
}

//
// RULE 12.2
// Given a nullable pointer `v` of class type (not a raw pointer) any mutation of `v` invalidates any null-safety proof for `v`.
//

NonScalar test_unsafe_deref_lvalue_nullable_non_scalar_proof_from_if_invalidation_member_field_mutation_param(NullableNonScalar p) {
  if (p) { // expected-note{{Taking true branch}}
    p.member_field = 42;
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
               // expected-note@-2{{nullable pointer obtained via}}
  }
  return {};
}

NonScalar test_unsafe_deref_lvalue_nullable_non_scalar_proof_from_if_invalidation_non_const_member_func_param(NullableNonScalar p) {
  if (p) { // expected-note{{Taking true branch}}
    p.non_const_member_func();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
               // expected-note@-2{{nullable pointer obtained via}}
  }
  return {};
}

NonScalar test_safe_deref_lvalue_nullable_non_scalar_proof_from_if_const_member_func_param(NullableNonScalar p) {
  if (p) {
    p.const_member_func();
    return *p;
  }
  return {};
}

NonScalar test_unsafe_deref_lvalue_nullable_non_scalar_proof_from_if_invalidation_member_field_mutation_param(NullableNonScalar p, NullableNonScalar q) {
  if (p) { // expected-note{{Taking true branch}}
    p = q;
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
               // expected-note@-2{{nullable pointer obtained via}}
  }
  return {};
}

//
// RULE 12.3
// A null-safety proof established on a non-local, non-const nullable pointer lvalue, or rvalue reference, is invalidated by any call expression.
//

//FIXME test rvalue references

int Member::test_unsafe_deref_member_lvalue_scalar_proof_from_if_invalidation_call() {
  if (member_scalar_lvalue) { // expected-note{{Assuming field 'member_scalar_lvalue' is non-null}}
                              // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Value assigned to field 'member_scalar_lvalue'}}
    return *member_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                  // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int Member::test_safe_deref_member_const_lvalue_scalar_proof_from_if_no_invalidation_call() {
  if (member_scalar_const_lvalue) {
    any_call();
    return *member_scalar_const_lvalue;
  }
  return 0;
}

int test_safe_deref_member_lvalue_scalar_proof_from_if_no_invalidation_call_param(Member m) {
  if (m.member_scalar_lvalue) {
    any_call();
    return *m.member_scalar_lvalue;
  }
  return 0;
}

int test_safe_deref_member_const_lvalue_scalar_proof_from_if_no_invalidation_call_param(Member m) {
  if (m.member_scalar_lvalue) {
    any_call();
    return *m.member_scalar_lvalue;
  }
  return 0;
}

int test_unsafe_global_lvalue_scalar_proof_from_if_invalidation_call() {
  if (global_scalar_lvalue) { // expected-note{{Assuming 'global_scalar_lvalue' is non-null}}
                              // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Value assigned to 'global_scalar_lvalue'}}
    return *global_scalar_lvalue; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                  // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_global_const_lvalue_scalar_proof_from_if_no_invalidation_call() {
  if (global_scalar_const_lvalue) {
    any_call();
    return *global_scalar_const_lvalue;
  }
  return 0;
}

//
// RULE 12.4
// A null-safety proof established on nullable pointer lvalue reference `v`, regardless of const qualification, that:
// a. is not bound to an rvalue
// b. is bound to value that is not local to the function, lambda, or initializer expression in which the null-safety proof is established
// c. is a parameter to the function in which the null-safety proof is established
// is invalidated by any call expression.
//

int test_safe_deref_const_lvalue_ref_bound_to_rvalue_scalar_proof_from_if_no_invalidation_call() {
  int *const& p = get_scalar();
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int Member::test_unsafe_deref_member_lvalue_ref_scalar_proof_from_if_invalidation_call() {
  if (member_scalar_lvalue_ref) { // expected-note{{Assuming field 'member_scalar_lvalue_ref' is non-null}}
                                  // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Assigning value}}
    return *member_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                      // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int Member::test_unsafe_deref_member_const_lvalue_ref_scalar_proof_from_if_invalidation_call() {
  if (member_scalar_const_lvalue_ref) { // expected-note{{Assuming field 'member_scalar_const_lvalue_ref' is non-null}}
                                        // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Assigning value}}
    return *member_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                            // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_unsafe_deref_member_lvalue_ref_scalar_proof_from_if_invalidation_call_param(Member m) {
  if (m.member_scalar_lvalue_ref) { // expected-note{{Assuming field 'member_scalar_lvalue_ref' is non-null}}
                                    // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Assigning value}}
    return *m.member_scalar_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                        // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_unsafe_deref_member_const_lvalue_ref_scalar_proof_from_if_invalidation_call_param(Member m) {
  if (m.member_scalar_const_lvalue_ref) { // expected-note{{Assuming field 'member_scalar_const_lvalue_ref' is non-null}}
                                          // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Assigning value}}
    return *m.member_scalar_const_lvalue_ref; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                                              // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_ref_scalar_local_binding_proof_from_if_no_invalidation_call_param(int *q) {
  int *&p = q;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_const_lvalue_ref_scalar_local_binding_proof_from_if_no_invalidation_call_param(int *q) {
  int *const &p = q;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_ref_scalar_proof_from_if_invalidation_call_param(int *&p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Assigning value}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_unsafe_deref_const_lvalue_ref_scalar_proof_from_if_invalidation_call_param(int *const& p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{Assigning value}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

//
// RULE 12.5
// A null-safety proof established on an lvalue reference lambda capture, regardless of const qualification, is invalidated upon any call expression within the lambda body.
//

int test_unsafe_deref_lvalue_ref_scalar_lamda_capture_proof_from_if_outside_invalidation_call_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) {
    return [&p]() {
      any_call();
      return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                 // expected-note@-1{{dereference of a nullable pointer}}
    }();
  }
  return 0;
}

int test_unsafe_deref_lvalue_ref_scalar_lamda_capture_proof_from_if_inside_invalidation_call_param(int *p) { // expected-note{{declared as nullable pointer here}}
  return [&p]() {
    if (p) { // expected-note{{Assuming 'p' is non-null}}
             // expected-note@-1{{Taking true branch}}
      any_call(); // expected-note{{nullable-safety proof established from this point}}
                  // expected-note@-1{{Assigning value}}
      return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
                 // expected-note@-1{{dereference of a nullable pointer}}
    }
    return 0;
  }();
}

//
// RULE 12.6
// Any call expression within a function/lambda body or global variable/member initializer expression invalidates the null-safety proof for all nullable pointers that have escaped the scope of the function/lambda body or global variable/member field initializer expression.
// A nullable pointer `v` escapes the scope by:
//

//
// RULE 12.6.1
// assigning the address of `v` to a non-local (possibly nullable) pointer or reference to a non-const nullable pointer or const volatile nullable pointer.
//

not_null<int**> make_scalar_non_nullable_lvalue_2();

int **global_scalar_lvalue_2;
int * const *global_scalar_const_lvalue_2;
not_null<int**> global_scalar_non_nullable_lvalue_2 = make_scalar_non_nullable_lvalue_2();
not_null<int*const*> global_scalar_non_nullable_const_lvalue_2 = make_scalar_non_nullable_lvalue_2();

int ***global_scalar_lvalue_3;
int *const **global_scalar_const_lvalue_3;

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_lvalue_2 = &p; // expected-note{{nullable-safety proof established from this point}}
                                 // expected-note@-1{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_lvalue_2 = &p;
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  global_scalar_lvalue_2 = &p; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_before_proof_via_if_param(int *p) {
  global_scalar_lvalue_2 = &p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_non_nullable_lvalue_2 = &p; // expected-note{{nullable-safety proof established from this point}}
                                              // expected-note@-1{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_non_nullable_lvalue_2 = &p;
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  global_scalar_non_nullable_lvalue_2 = &p; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_before_proof_any_call_via_if_param(int *p) {
  global_scalar_non_nullable_lvalue_2 = &p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_after_proof_any_call_reprove_via_if_param(int *p) {
  if (p) {
    global_scalar_lvalue_2 = &p;
    any_call();
    if (p)
        return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_before_proof_any_call_reprove_via_if_param(int *p) {
  global_scalar_lvalue_2 = &p;
  if (p) {
    any_call();
    if (p)
        return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_after_proof_any_call_reprove_via_if_param(int *p) {
  if (p) {
    global_scalar_non_nullable_lvalue_2 = &p;
    any_call();
    if (p)
        return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_before_proof_any_call_reprove_via_if_param(int *p) {
  global_scalar_non_nullable_lvalue_2 = &p;
  if (p) {
    any_call();
    if (p)
        return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_non_nullable_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    global_scalar_non_nullable_const_lvalue_2 = &p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_non_nullable_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_non_nullable_const_lvalue_2 = &p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_non_nullable_before_proof_any_call_via_if_param(int *p) {
  global_scalar_non_nullable_const_lvalue_2 = &p;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_non_nullable_before_proof_via_if_param(int *p) {
  global_scalar_non_nullable_const_lvalue_2 = &p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    global_scalar_const_lvalue_2 = &p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_const_lvalue_2 = &p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_before_proof_any_call_via_if_param(int *p) {
  global_scalar_const_lvalue_2 = &p;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_const_before_proof_via_if_param(int *p) {
  global_scalar_const_lvalue_2 = &p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_assignment_to_global_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    int *q = p;
    global_scalar_lvalue_2 = &q;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_assignment_to_global_after_proof_via_if_param(int *p) {
  if (p) {
    int *q = p;
    global_scalar_lvalue_2 = &q;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_assignment_to_global_before_proof_any_call_via_if_param(int *p) {
  int *q = p;
  global_scalar_lvalue_2 = &q;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_assignment_to_global_before_proof_via_if_param(int *p) {
  int *q = p;
  global_scalar_lvalue_2 = &q;
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p && global_scalar_lvalue_3) { // expected-note{{Assuming 'p' is non-null}}
                                     // expected-note@-1{{Left side of '&&' is true}}
                                     // expected-note@-2{{Assuming 'global_scalar_lvalue_3' is non-null}}
                                     // expected-note@-3{{Taking true branch}}
    *global_scalar_lvalue_3 = &p; // expected-note{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_after_proof_any_call_reprove_via_if_param(int *p) {
  if (p && global_scalar_lvalue_3) {
    *global_scalar_lvalue_3 = &p;
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_after_proof_via_if_param(int *p) {
  if (p && global_scalar_lvalue_3) {
    *global_scalar_lvalue_3 = &p;
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (global_scalar_lvalue_3) { // expected-note{{Assuming 'global_scalar_lvalue_3' is non-null}}
                                // expected-note@-1{{Taking true branch}}
    *global_scalar_lvalue_3 = &p; // expected-note{{pointer escapes function scope}}
  }
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_before_proof_any_call_reprove_via_if_param(int *p) {
  if (global_scalar_lvalue_3) {
    *global_scalar_lvalue_3 = &p;
  }
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_before_proof_via_if_param(int *p) {
  if (global_scalar_lvalue_3) {
    *global_scalar_lvalue_3 = &p;
  }
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_const_after_proof_any_call_via_if_param(int *p) {
  if (p && global_scalar_const_lvalue_3) {
    *global_scalar_const_lvalue_3 = &p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_const_after_proof_via_if_param(int *p) {
  if (p && global_scalar_const_lvalue_3) {
    *global_scalar_const_lvalue_3 = &p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_const_before_proof_via_if_param(int *p) {
  if (global_scalar_const_lvalue_3) {
    *global_scalar_const_lvalue_3 = &p;
  }
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_2_const_before_proof_any_call_via_if_param(int *p) {
  if (global_scalar_const_lvalue_3) {
    *global_scalar_const_lvalue_3 = &p;
  }
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_ref_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    global_scalar_lvalue_ref = p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_ref_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_lvalue_ref = p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_ref_before_proof_any_call_via_if_param(int *p) {
  global_scalar_lvalue_ref = p;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_ref_before_proof_via_if_param(int *p) {
  global_scalar_lvalue_ref = p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_ref_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    global_not_null_scalar_lvalue_ref = p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_ref_after_proof_via_if_param(int *p) {
  if (p) {
    global_not_null_scalar_lvalue_ref = p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_ref_before_proof_any_call_via_if_param(int *p) {
  if (p)
    global_not_null_scalar_lvalue_ref = p;

  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_assignment_to_global_non_nullable_ref_before_proof_via_if_param(int *p) {
  if (p)
    global_not_null_scalar_lvalue_ref = p;

  if (p) {
    return *p;
  }
  return 0;
}

//
// RULE 12.6.2
// passing the address of `v` as a call expression parameter of (possibly nullable) pointer to non-const and non-volatile nullable pointer type.
//

void call_take_scalar_lvalue_pointer_pointer(int **p);
void call_take_scalar_lvalue_pointer_const_pointer(int *const*p);
void call_take_not_null_scalar_lvalue_pointer_pointer(not_null<int**> p);
void call_take_not_null_scalar_lvalue_pointer_const_pointer(not_null<int*const*> p);
void call_take_scalar_lvalue_pointer_ref(int *&p);
void call_take_scalar_lvalue_pointer_const_ref(int *const &p);

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_call_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_pointer_pointer(&p); // expected-note{{nullable-safety proof established from this point}}
                                                 // expected-note@-1{{pointer escapes function scope}}
                                                 // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_call_after_proof_reprove_via_if_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_pointer(&p);
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_call_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_pointer_pointer(&p); // expected-note{{nullable-safety proof established from this point}}
                                                 // expected-note@-1{{pointer escapes function scope}}
                                                 // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_call_after_proof_any_call_reprove_via_if_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_pointer(&p);
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_pointer_pointer(&p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_address_call_before_proof_via_if_any_call_param(int *p) { // expected-note{{declared as nullable pointer here}}
  call_take_scalar_lvalue_pointer_pointer(&p); // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_address_call_before_proof_via_if_any_call_reprove_param(int *p) {
  call_take_scalar_lvalue_pointer_pointer(&p);
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_const_pointer(&p);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_escape_via_address_call_after_proof_via_if_any_call_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_const_pointer(&p);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_pointer_const_pointer(&p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  call_take_scalar_lvalue_pointer_const_pointer(&p);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) {
    int *q = p;
    call_take_scalar_lvalue_pointer_const_pointer(&q);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_call_after_proof_via_if_any_callparam(int *p) {
  if (p) {
    int *q = p;
    call_take_scalar_lvalue_pointer_const_pointer(&q);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_call_before_proof_via_if_param(int *p) {
  int *q = p;
  call_take_scalar_lvalue_pointer_const_pointer(&q);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_alias_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  int *q = p;
  call_take_scalar_lvalue_pointer_const_pointer(&q);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_not_null_scalar_lvalue_pointer_pointer(&p); // expected-note{{nullable-safety proof established from this point}}
                                                 // expected-note@-1{{pointer escapes function scope}}
                                                 // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_after_proof_reprove_via_if_param(int *p) {
  if (p) {
    call_take_not_null_scalar_lvalue_pointer_pointer(&p);
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_not_null_scalar_lvalue_pointer_pointer(&p); // expected-note{{nullable-safety proof established from this point}}
                                                 // expected-note@-1{{pointer escapes function scope}}
                                                 // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_after_proof_any_call_reprove_via_if_param(int *p) {
  if (p) {
    call_take_not_null_scalar_lvalue_pointer_pointer(&p);
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_not_null_scalar_lvalue_pointer_pointer(&p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_before_proof_via_if_any_call_param(int *p) { // expected-note{{declared as nullable pointer here}}
  call_take_not_null_scalar_lvalue_pointer_pointer(&p); // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_not_null_pointer_escape_via_address_call_before_proof_via_if_any_call_reprove_param(int *p) {
  call_take_not_null_scalar_lvalue_pointer_pointer(&p);
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_pointer_const_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) {
    call_take_not_null_scalar_lvalue_pointer_const_pointer(&p);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_pointer_const_escape_via_address_call_after_proof_via_if_any_call_param(int *p) {
  if (p) {
    call_take_not_null_scalar_lvalue_pointer_const_pointer(&p);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_pointer_const_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_not_null_scalar_lvalue_pointer_const_pointer(&p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_pointer_const_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  call_take_not_null_scalar_lvalue_pointer_const_pointer(&p);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

//
// RULE 12.6.3
// passing `v` as a call expression parameter of non-const,non-volatile lvalue reference to a nullable pointer
//

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_ref_call_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_pointer_ref(p); // expected-note{{nullable-safety proof established from this point}}
                                            // expected-note@-1{{pointer escapes function scope}}
                                            // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_ref_call_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_ref(p);
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_ref_call_after_proof_via_if_any_call_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_pointer_ref(p); // expected-note{{nullable-safety proof established from this point}}
                                            // expected-note@-1{{pointer escapes function scope}}
                                            // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_ref_call_after_proof_via_if_any_call_reprove_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_ref(p);
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_ref_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_pointer_ref(p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_ref_call_before_proof_via_if_any_call_param(int *p) { // expected-note{{declared as nullable pointer here}}
  call_take_scalar_lvalue_pointer_ref(p); // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_ref_call_before_proof_via_if_any_call_reprove_param(int *p) {
  call_take_scalar_lvalue_pointer_ref(p);
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_const_ref_call_after_proof_via_if_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_const_ref(p);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_const_ref_call_after_proof_via_if_any_call_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_pointer_const_ref(p);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_const_ref_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_pointer_const_ref(p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_const_ref_call_before_proof_via_if_any_call_param(int *p) {
  call_take_scalar_lvalue_pointer_const_ref(p);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

//
// RULE 12.6.4
// using the address of `v` in direct- or copy-list-initialization of objects of class type or aggregate initialization, to initialize a (possibly nullable) pointer to a (possibly const) non-volatile nullable pointer.
//

struct PtrClsByVal {
  PtrClsByVal(int** p);
  int dummy;
};

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByVal o { &p }; // expected-note{{nullable-safety proof established from this point}}
                     // expected-note@-1{{pointer escapes function scope}}
                     // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByVal o { &p };
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByVal o { &p }; // expected-note{{nullable-safety proof established from this point}}
                     // expected-note@-1{{pointer escapes function scope}}
                     // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByVal o { &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByVal o = { &p }; // expected-note{{nullable-safety proof established from this point}}
                       // expected-note@-1{{pointer escapes function scope}}
                       // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByVal o = { &p };
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByVal o = { &p }; // expected-note{{nullable-safety proof established from this point}}
                       // expected-note@-1{{pointer escapes function scope}}
                       // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByVal o = { &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrClsByVal o { &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  PtrClsByVal o { &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_pointer_before_proof_via_if_param(int *p) {
  PtrClsByVal o { &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrClsByVal o = { &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  PtrClsByVal o = { &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_pointer_before_proof_via_if_param(int *p) {
  PtrClsByVal o = { &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct PtrClsByConstVal {
  PtrClsByConstVal(int*const* p);
  int dummy;
};

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    PtrClsByConstVal o { &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrClsByConstVal o { &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    PtrClsByConstVal o = { &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrClsByConstVal o = { &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_pointer_before_proof_any_call_via_if_param(int *p) {
  PtrClsByConstVal o { &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_pointer_before_proof_via_if_param(int *p) {
  PtrClsByConstVal o { &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_pointer_before_proof_any_call_via_if_param(int *p) {
  PtrClsByConstVal o = { &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_pointer_before_proof_via_if_param(int *p) {
  PtrClsByConstVal o = { &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct NotNullPtrClsByVal {
  NotNullPtrClsByVal(not_null<int**> p);
  int dummy;
};

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    NotNullPtrClsByVal o { &p }; // expected-note{{nullable-safety proof established from this point}}
                     // expected-note@-1{{pointer escapes function scope}}
                     // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    NotNullPtrClsByVal o { &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    NotNullPtrClsByVal o { &p }; // expected-note{{nullable-safety proof established from this point}}
                     // expected-note@-1{{pointer escapes function scope}}
                     // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    NotNullPtrClsByVal o { &p };
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_pointer_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    NotNullPtrClsByVal o = { &p }; // expected-note{{nullable-safety proof established from this point}}
                       // expected-note@-1{{pointer escapes function scope}}
                       // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    NotNullPtrClsByVal o = { &p };
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    NotNullPtrClsByVal o = { &p }; // expected-note{{nullable-safety proof established from this point}}
                       // expected-note@-1{{pointer escapes function scope}}
                       // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    NotNullPtrClsByVal o = { &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  NotNullPtrClsByVal o { &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  NotNullPtrClsByVal o { &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrClsByVal o { &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  NotNullPtrClsByVal o = { &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrClsByVal o = { &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct NotNullPtrClsByConstVal {
  NotNullPtrClsByConstVal(not_null<int*const*> p);
  int dummy;
};

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    NotNullPtrClsByConstVal o { &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    NotNullPtrClsByConstVal o { &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    NotNullPtrClsByConstVal o = { &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    NotNullPtrClsByConstVal o = { &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_const_pointer_before_proof_any_call_via_if_param(int *p) {
  NotNullPtrClsByConstVal o { &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_not_null_const_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrClsByConstVal o { &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_const_pointer_before_proof_any_call_via_if_param(int *p) {
  NotNullPtrClsByConstVal o = { &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_not_null_const_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrClsByConstVal o = { &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct PtrAggregateByVal {
  int dummy;
  int** scalar_lvalue;
};

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrAggregateByVal o = { 42, &p }; // expected-note{{nullable-safety proof established from this point}}
                                 // expected-note@-1{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    PtrAggregateByVal o = { 42, &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrAggregateByVal o = { 42, &p };
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrAggregateByVal o { 42, &p }; // expected-note{{nullable-safety proof established from this point}}
                               // expected-note@-1{{pointer escapes function scope}}
    any_call();
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    PtrAggregateByVal o { 42, &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrAggregateByVal o { 42, &p };
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrAggregateByVal o = { 42, &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  PtrAggregateByVal o = { 42, &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_pointer_before_proof_via_if_param(int *p) {
  PtrAggregateByVal o = { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrAggregateByVal o { 42, &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  PtrAggregateByVal o { 42, &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_pointer_before_proof_via_if_param(int *p) {
  PtrAggregateByVal o { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct PtrAggregateByConstVal {
  int dummy;
  int*const* scalar_lvalue;
};

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    PtrAggregateByConstVal o = { 42, &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrAggregateByConstVal o = { 42, &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    PtrAggregateByConstVal o { 42, &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrAggregateByConstVal o { 42, &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_const_pointer_before_proof_any_callvia_if_param(int *p) {
  PtrAggregateByConstVal o = { 42, &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_const_pointer_before_proof_via_if_param(int *p) {
  PtrAggregateByConstVal o = { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_const_pointer_before_proof_via_if_param(int *p) {
  PtrAggregateByConstVal o { 42, &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_const_pointer_before_proof_any_call_via_if_param(int *p) {
  PtrAggregateByConstVal o { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct NotNullPtrAggregateByVal {
  int dummy;
  not_null<int**> scalar_lvalue;
};

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    NotNullPtrAggregateByVal o = { 42, &p }; // expected-note{{nullable-safety proof established from this point}}
                                 // expected-note@-1{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    NotNullPtrAggregateByVal o = { 42, &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    NotNullPtrAggregateByVal o = { 42, &p };
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_pointer_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    NotNullPtrAggregateByVal o { 42, &p }; // expected-note{{nullable-safety proof established from this point}}
                               // expected-note@-1{{pointer escapes function scope}}
    any_call();
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_pointer_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    NotNullPtrAggregateByVal o { 42, &p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    NotNullPtrAggregateByVal o { 42, &p };
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  NotNullPtrAggregateByVal o = { 42, &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  NotNullPtrAggregateByVal o = { 42, &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrAggregateByVal o = { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_pointer_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  NotNullPtrAggregateByVal o { 42, &p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_pointer_before_proof_any_call_via_if_reprove_param(int *p) {
  NotNullPtrAggregateByVal o { 42, &p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrAggregateByVal o { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

struct NotNullPtrAggregateByConstVal {
  int dummy;
  not_null<int*const*> scalar_lvalue;
};

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    NotNullPtrAggregateByConstVal o = { 42, &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    NotNullPtrAggregateByConstVal o = { 42, &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    NotNullPtrAggregateByConstVal o { 42, &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    NotNullPtrAggregateByConstVal o { 42, &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_const_pointer_before_proof_any_callvia_if_param(int *p) {
  NotNullPtrAggregateByConstVal o = { 42, &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_1_init_not_null_const_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrAggregateByConstVal o = { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_const_pointer_before_proof_via_if_param(int *p) {
  NotNullPtrAggregateByConstVal o { 42, &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_aggregate_2_init_not_null_const_pointer_before_proof_any_call_via_if_param(int *p) {
  NotNullPtrAggregateByConstVal o { 42, &p };
  if (p) {
    return *p;
  }
  return 0;
}

//
// RULE 12.6.5
// using `v` in direct- or copy-list-initialization of objects of class type or aggregate initialization, to bind a reference to a non-const, non-volatile nullable pointer.
//

struct PtrClsByRef {
  PtrClsByRef(int*& p);
  int dummy;
};

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByRef o { p }; // expected-note{{nullable-safety proof established from this point}}
                         // expected-note@-1{{pointer escapes function scope}}
                         // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByRef o { p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByRef o { p }; // expected-note{{nullable-safety proof established from this point}}
                         // expected-note@-1{{pointer escapes function scope}}
                         // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByRef o { p };
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_after_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByRef o = { p }; // expected-note{{nullable-safety proof established from this point}}
                           // expected-note@-1{{pointer escapes function scope}}
                           // expected-note@-2{{nullable-safety proof invalidated}}
    any_call();
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_after_proof_any_call_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByRef o = { p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrClsByRef o = { p }; // expected-note{{nullable-safety proof established from this point}}
                           // expected-note@-1{{pointer escapes function scope}}
                           // expected-note@-2{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    PtrClsByRef o = { p };
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrClsByRef o { p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_before_proof_any_call_via_if_reprove_param(int *p) {
  PtrClsByRef o { p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_ref_before_proof_via_if_param(int *p) {
  PtrClsByRef o { p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_before_proof_any_call_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrClsByRef o = { p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_before_proof_any_call_via_if_reprove_param(int *p) {
  PtrClsByRef o = { p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_ref_before_proof_via_if_param(int *p) {
  PtrClsByRef o = { p };
  if (p) {
    return *p;
  }
  return 0;
}

struct PtrClsByConstRef {
  PtrClsByConstRef(int*const& p);
  int dummy;
};

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_ref_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    PtrClsByConstRef o { p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_ref_after_proof_via_if_param(int *p) {
  if (p) {
    PtrClsByConstRef o { p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_ref_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    PtrClsByConstRef o = { p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_ref_after_proof_via_if_param(int *p) {
  if (p) {
    PtrClsByConstRef o = { p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_ref_before_proof_any_call_via_if_param(int *p) {
  PtrClsByConstRef o { p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_direct_list_init_const_ref_before_proof_via_if_param(int *p) {
  PtrClsByConstRef o { p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_ref_before_proof_any_call_via_if_param(int *p) {
  PtrClsByConstRef o = { p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_escape_via_class_copy_list_init_const_ref_before_proof_via_if_param(int *p) {
  PtrClsByConstRef o = { p };
  if (p) {
    return *p;
  }
  return 0;
}

struct PtrAggregateByRef {
  int dummy;
  int*& scalar_lvalue_ref;
};

int test_unsafe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_1_init_pointer_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrAggregateByRef o = { 42, p }; // expected-note{{nullable-safety proof established from this point}}
                                   // expected-note@-1{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_1_init_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    PtrAggregateByRef o = { 42, p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_2_init_pointer_after_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    PtrAggregateByRef o { 42, p }; // expected-note{{nullable-safety proof established from this point}}
                                 // expected-note@-1{{pointer escapes function scope}}
    any_call(); // expected-note{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_2_init_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) {
    PtrAggregateByRef o { 42, p };
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_1_init_pointer_before_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrAggregateByRef o = { 42, p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_1_init_pointer_before_proof_via_if_reprove_param(int *p) {
  PtrAggregateByRef o = { 42, p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_2_init_pointer_before_proof_via_if_param(int *p) { // expected-note{{declared as nullable pointer here}}
  PtrAggregateByRef o { 42, p }; // expected-note{{pointer escapes function scope}}
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    any_call(); // expected-note{{nullable-safety proof established from this point}}
                // expected-note@-1{{nullable-safety proof invalidated}}
    return *p; // expected-warning{{dereference of a nullable pointer [amadeus.NullablePointers]}}
               // expected-note@-1{{dereference of a nullable pointer}}
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_2_init_pointer_before_proof_via_if_reprove_param(int *p) {
  PtrAggregateByRef o { 42, p };
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

struct PtrAggregateByConstRef {
  int dummy;
  int*const& scalar_lvalue_ref;
};

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_1_init_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrAggregateByConstRef o = { 42, p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_2_init_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    PtrAggregateByConstRef o { 42, p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_1_init_pointer_before_proof_via_if_param(int *p) {
  PtrAggregateByConstRef o = { 42, p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_ref_escape_via_aggregate_2_init_pointer_before_proof_via_if_param(int *p) {
  PtrAggregateByConstRef o { 42, p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

//
// RULE 12.7
// A nullable pointer `v` escape via pointer or reference to a non-const volatile nullable pointer invalidates any null-safety proof for `v` and renders subsequent null-safety proofs impossible.
// A nullable pointer `v` escapes the scope as volatile by:
//

//
// RULE 12.7.1
// assigning the address of `v` to a non-local (possibly nullable) pointer to a non-const volatile nullable pointer.
//

int *volatile* global_scalar_volatile_lvalue_2;
int *const volatile* global_scalar_const_volatile_lvalue_2;
not_null<int*volatile*> make_global_scalar_volatile_not_null_lvalue_2();
not_null<int*volatile*> global_scalar_volatile_not_null_lvalue_2 = make_global_scalar_volatile_not_null_lvalue_2();
not_null<int*const volatile*> global_scalar_const_volatile_not_null_lvalue_2 = make_global_scalar_volatile_not_null_lvalue_2();

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_after_proof_any_call_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                          // expected-note@-1{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_after_proof_any_call_via_if_perma_unsafe_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                          // expected-note@-1{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                          // expected-note@-1{{non-nullable pointer escapes as volatile}}
    return *p;

  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_after_proof_via_if_perma_unsafe_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                          // expected-note@-1{{non-nullable pointer escapes as volatile}}
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_before_proof_any_call_via_if_param(int *p) {
  global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                        // expected-note@-1{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_before_proof_any_call_via_if_perma_unsafe_param(int *p) {
  global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                        // expected-note@-1{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_before_proof_via_if_param(int *p) {
  global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                        // expected-note@-1{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_before_proof_via_if_perma_unsafe_param(int *p) {
  global_scalar_volatile_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                        // expected-note@-1{{non-nullable pointer escapes as volatile}}
  if (p) {
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_volatile_escape_via_address_assignment_to_global_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    global_scalar_const_volatile_lvalue_2 = &p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_after_proof_any_call_via_if_perma_unsafe_param(int *p) {
  if (p) {
    global_scalar_const_volatile_lvalue_2 = &p;
    any_call();
    if (p)
      return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_const_volatile_lvalue_2 = &p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_after_proof_via_if_perma_unsafe_param(int *p) {
  if (p) {
    global_scalar_const_volatile_lvalue_2 = &p;
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_before_proof_any_call_via_if_param(int *p) {
  global_scalar_const_volatile_lvalue_2 = &p;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_before_proof_any_call_via_if_perma_unsafe_param(int *p) {
  global_scalar_const_volatile_lvalue_2 = &p;
  if (p) {
    any_call();
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_before_proof_via_if_param(int *p) {
  global_scalar_const_volatile_lvalue_2 = &p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_escape_via_address_assignment_to_global_before_proof_via_if_perma_unsafe_param(int *p) {
  global_scalar_const_volatile_lvalue_2 = &p;
  if (p) {
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_after_proof_any_call_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_after_proof_any_call_via_if_perma_unsafe_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_after_proof_via_if_perma_unsafe_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{nullable pointer obtained via}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_before_proof_any_call_via_if_param(int *p) {
  global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{nullable pointer obtained via}}
                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_before_proof_any_call_via_if_perma_unsafe_param(int *p) {
  global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{nullable pointer obtained via}}
                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_before_proof_via_if_param(int *p) {
  global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{nullable pointer obtained via}}
                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_before_proof_via_if_perma_unsafe_param(int *p) {
  global_scalar_volatile_not_null_lvalue_2 = &p; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                 // expected-note@-1{{nullable pointer obtained via}}
                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_volatile_not_null_escape_via_address_assignment_to_global_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    global_scalar_const_volatile_not_null_lvalue_2 = &p;
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_after_proof_any_call_via_if_perma_unsafe_param(int *p) {
  if (p) {
    global_scalar_const_volatile_not_null_lvalue_2 = &p;
    any_call();
    if (p)
      return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_after_proof_via_if_param(int *p) {
  if (p) {
    global_scalar_const_volatile_not_null_lvalue_2 = &p;
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_after_proof_via_if_perma_unsafe_param(int *p) {
  if (p) {
    global_scalar_const_volatile_not_null_lvalue_2 = &p;
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_before_proof_any_call_via_if_param(int *p) {
  global_scalar_const_volatile_not_null_lvalue_2 = &p;
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_before_proof_any_call_via_if_perma_unsafe_param(int *p) {
  global_scalar_const_volatile_not_null_lvalue_2 = &p;
  if (p) {
    any_call();
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_before_proof_via_if_param(int *p) {
  global_scalar_const_volatile_not_null_lvalue_2 = &p;
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_pointer_const_volatile_not_null_escape_via_address_assignment_to_global_before_proof_via_if_perma_unsafe_param(int *p) {
  global_scalar_const_volatile_not_null_lvalue_2 = &p;
  if (p) {
    if (p)
        return *p;
    return 0;
  }
  return 0;
}

//
// RULE 12.7.2
// passing the address of `v` as a call expression parameter of (possibly nullable) pointer to non-const volatile nullable pointer type.
//

void call_take_scalar_lvalue_volatile_pointer_pointer(int *volatile*p); // expected-note{{declared returning nullable pointer here}}
                                                                        // expected-note@-1{{declared returning nullable pointer here}}
                                                                        // expected-note@-2{{declared returning nullable pointer here}}
                                                                        // expected-note@-3{{declared returning nullable pointer here}}
                                                                        // expected-note@-4{{declared returning nullable pointer here}}
                                                                        // expected-note@-5{{declared returning nullable pointer here}}
                                                                        // expected-note@-6{{declared returning nullable pointer here}}
void call_take_scalar_lvalue_volatile_pointer_const_pointer(int *const volatile* p);
void call_take_not_null_scalar_lvalue_volatile_pointer_pointer(not_null<int*volatile*> p); // expected-note{{declared returning nullable pointer here}}
                                                                        // expected-note@-1{{declared returning nullable pointer here}}
                                                                        // expected-note@-2{{declared returning nullable pointer here}}
                                                                        // expected-note@-3{{declared returning nullable pointer here}}
                                                                        // expected-note@-4{{declared returning nullable pointer here}}
                                                                        // expected-note@-5{{declared returning nullable pointer here}}
                                                                        // expected-note@-6{{declared returning nullable pointer here}}
void call_take_not_null_scalar_lvalue_volatile_pointer_const_pointer(not_null<int*const volatile*> p);

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{called here}}
                                                          // expected-note@-2{{non-nullable pointer escapes as volatile}}
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_after_proof_cannot_reprove_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{called here}}
                                                          // expected-note@-2{{non-nullable pointer escapes as volatile}}
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_after_proof_any_call_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{called here}}
                                                          // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_after_proof_any_call_cannot_reprove_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                          // expected-note@-1{{called here}}
                                                          // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{called here}}
                                                        // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{called here}}
                                                        // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_address_call_before_proof_via_if_any_call_reprove_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                        // expected-note@-1{{called here}}
                                                        // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_const_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_volatile_pointer_const_pointer(&p);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_const_escape_via_address_call_after_proof_via_if_any_call_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_volatile_pointer_const_pointer(&p);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_const_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_const_pointer(&p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_const_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_const_pointer(&p);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_alias_escape_via_address_call_after_proof_via_if_any_callparam(int *p) {
  if (p) {
    int * volatile q = p;
    call_take_scalar_lvalue_volatile_pointer_const_pointer(&q);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_alias_escape_via_address_call_before_proof_via_if_param(int *p) {
  int *volatile q = p;
  call_take_scalar_lvalue_volatile_pointer_const_pointer(&q);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_alias_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  int *volatile q = p;
  call_take_scalar_lvalue_volatile_pointer_const_pointer(&q);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                   // expected-note@-1{{called here}}
                                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    return *p;

  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_after_proof_cannot_reprove_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                   // expected-note@-1{{called here}}
                                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_after_proof_any_call_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                   // expected-note@-1{{called here}}
                                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_after_proof_any_call_cannot_reprove_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                   // expected-note@-1{{called here}}
                                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                 // expected-note@-1{{called here}}
                                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                 // expected-note@-1{{called here}}
                                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_not_null_scalar_volatile_pointer_escape_via_address_call_before_proof_via_if_any_call_reprove_param(int *p) {
  call_take_not_null_scalar_lvalue_volatile_pointer_pointer(&p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                                 // expected-note@-1{{called here}}
                                                                 // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_volatile_pointer_const_escape_via_address_call_after_proof_via_if_param(int *p) {
  if (p) {
    call_take_not_null_scalar_lvalue_volatile_pointer_const_pointer(&p);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_volatile_pointer_const_escape_via_address_call_after_proof_via_if_any_call_param(int *p) {
  if (p) {
    call_take_not_null_scalar_lvalue_volatile_pointer_const_pointer(&p);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_volatile_pointer_const_escape_via_address_call_before_proof_via_if_param(int *p) {
  call_take_not_null_scalar_lvalue_volatile_pointer_const_pointer(&p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_not_null_scalar_volatile_pointer_const_escape_via_address_call_before_proof_via_if_any_call_param(int *p) {
  call_take_not_null_scalar_lvalue_volatile_pointer_const_pointer(&p);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

//
// RULE 12.7.3
// passing `v` as a call expression parameter of non-const volatile lvalue reference to a nullable pointer.
//

void call_take_scalar_lvalue_volatile_pointer_ref(int *volatile&p); // expected-note{{declared returning nullable pointer here}}
                                                                    // expected-note@-1{{declared returning nullable pointer here}}
                                                                    // expected-note@-2{{declared returning nullable pointer here}}
                                                                    // expected-note@-3{{declared returning nullable pointer here}}
                                                                    // expected-note@-4{{declared returning nullable pointer here}}
                                                                    // expected-note@-5{{declared returning nullable pointer here}}
                                                                    // expected-note@-6{{declared returning nullable pointer here}}
void call_take_scalar_lvalue_volatile_pointer_const_ref(int *const volatile&p);

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{called here}}
                                                     // expected-note@-2{{non-nullable pointer escapes as volatile}}
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_after_proof_via_if_cannot_reprove_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{called here}}
                                                     // expected-note@-2{{non-nullable pointer escapes as volatile}}
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_after_proof_via_if_any_call_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{called here}}
                                                     // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_after_proof_via_if_any_call_reprove_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                     // expected-note@-1{{called here}}
                                                     // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{called here}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_before_proof_via_if_any_call_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{called here}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_ref_call_before_proof_via_if_any_call_reprove_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_ref(p); // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                                   // expected-note@-1{{called here}}
                                                   // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_const_ref_call_after_proof_via_if_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_volatile_pointer_const_ref(p);
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_const_ref_call_after_proof_via_if_any_call_param(int *p) {
  if (p) {
    call_take_scalar_lvalue_volatile_pointer_const_ref(p);
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_const_ref_call_before_proof_via_if_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_const_ref(p);
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_const_ref_call_before_proof_via_if_any_call_param(int *p) {
  call_take_scalar_lvalue_volatile_pointer_const_ref(p);
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

//
// RULE 12.7.4
// using the address of the pointer in direct- or copy-list-initialization of objects of class type or aggregate initialization to initialize a (possibly nullable) pointer to a non-const volatile nullable pointer.
//

struct VolatilePtrClsByVal {
  VolatilePtrClsByVal(int*volatile* p);
  int dummy;
};

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_after_proof_via_if_cannot_reprove_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_after_proof_any_call_via_if_cannot_reprove_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_after_proof_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{non-nullable pointer escapes as volatile}}
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_after_proof_via_if_reprove_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{non-nullable pointer escapes as volatile}}
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_after_proof_any_call_via_if_cannot_reprove_param(int *p) {
  if (p) { // expected-note{{Assuming 'p' is non-null}}
           // expected-note@-1{{Taking true branch}}
    VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                    // expected-note@-1{{nullable pointer obtained via}}
                                    // expected-note@-2{{non-nullable pointer escapes as volatile}}
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_before_proof_any_call_via_if_param(int *p) {
  VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_before_proof_any_call_via_if_cannot_reprove_param(int *p) {
  VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_pointer_before_proof_via_if_param(int *p) {
  VolatilePtrClsByVal o { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                // expected-note@-1{{nullable pointer obtained via}}
                                // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_before_proof_any_call_via_if_param(int *p) {
  VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_before_proof_any_call_via_if_cannot_reprove_param(int *p) {
  VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    any_call();
    if (p)
      return *p;
  }
  return 0;
}

int test_unsafe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_pointer_before_proof_via_if_param(int *p) {
  VolatilePtrClsByVal o = { &p }; // expected-warning{{non-nullable pointer escapes as volatile [amadeus.NullablePointers]}}
                                  // expected-note@-1{{nullable pointer obtained via}}
                                  // expected-note@-2{{non-nullable pointer escapes as volatile}}
  if (p) {
    return *p;
  }
  return 0;
}

struct VolatilePtrClsByConstVal {
  VolatilePtrClsByConstVal(int*const volatile* p);
  int dummy;
};

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    VolatilePtrClsByConstVal o { &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    VolatilePtrClsByConstVal o { &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_const_pointer_after_proof_any_call_via_if_param(int *p) {
  if (p) {
    VolatilePtrClsByConstVal o = { &p };
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_const_pointer_after_proof_via_if_param(int *p) {
  if (p) {
    VolatilePtrClsByConstVal o = { &p };
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_const_pointer_before_proof_any_call_via_if_param(int *p) {
  VolatilePtrClsByConstVal o { &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_direct_list_init_const_pointer_before_proof_via_if_param(int *p) {
  VolatilePtrClsByConstVal o { &p };
  if (p) {
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_const_pointer_before_proof_any_call_via_if_param(int *p) {
  VolatilePtrClsByConstVal o = { &p };
  if (p) {
    any_call();
    return *p;
  }
  return 0;
}

int test_safe_deref_lvalue_scalar_volatile_pointer_escape_via_class_copy_list_init_const_pointer_before_proof_via_if_param(int *p) {
  VolatilePtrClsByConstVal o = { &p };
  if (p) {
    return *p;
  }
  return 0;
}
