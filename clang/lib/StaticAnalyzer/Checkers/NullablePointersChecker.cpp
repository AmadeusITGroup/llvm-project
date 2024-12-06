// nullable pointers in C++
// Makes C++ null-safe - preventing dereferences of nullptr at compile time - by following
// the same approach as Kotlin: distinguishing between pointers that can have nullptr as value
// and those that are never nullptr and preventing dereferences of nullable pointers without
// proof of null-safety (e.g. via an if guard).
//
// null-safe C++ rules and definitions:
// 0. a NullablePointer is any type T defined s.t.
//    1. an object of type T is comparable to nullptr
//    2. an object of type T is contextually convertable to bool; the effect of the conversion is
//    false when under 0.1. the object is equivalent to nullptr and true otherwise
//    3. given a value of type T, m a member of type U, mp is a pointer to member of type U, any of the following is defined:
//       - *v
//       - v->m
//       - v->*m
//
//    Any raw pointer T* is a NullablePointer. Smart pointers are also NullablePointers.
//    Note that this is a weaker version of NullablePointer named requirement of standard C++.
//
//    We refer to types satisfying NullablePointer as nullable pointers.
// 1. for all types T satisfying NullablePointer, gsl::not_null<T> is never equivalent nullptr and is always safe to dereference.
//    The operators *, -> and ->* will never dereference nullptr.
//    It is refferred to as a non-nullable pointer type.
// 2. a value of nullable pointer type cannot be dereferenced unless it is null-safe
// 3. a NullablePointer value is null-safe iff, either
//    1. it is a known null-safe value (a value known to never be null, see 5.)
//    2. a valid null-safety proof has been established for the value (see 8.)
//
//    Due to 3.2. null-safety is emphemeral.
// 4. a non-nullable pointer cannot be constructed or assigned a value from a nullable pointer
//    unless that nullable pointer is null-safe
// 5. the following prvalues are always null-safe:
//    1. the this pointer
//    2. the result of the built-in address-of operator &a
//    3. the result of the built-in throwable new
//    4. the result of array-to-pointer or function-to-pointer conversion
//    5. the result of the gsl::not_null<T>::operator T* conversion
//
//      Implicit conversion from a non-nullable to a nullable pointer always yields
//      a null-safe nullable pointer.
// 6. all other rvalues are never null-safe ??? rvalue references ??? probably rvalue references are treated as lvalues???
//
//    null-safety proofs cannot be estabilished for rvalues not included in 5.
// 7. an lvalue reference to volatile nullable pointer is never null-safe
//
//    Null-safery proofs cannot be established for volatile pointer lvalue references.
// 8. a null-safety proof for a nullable pointer v is established by:
//    1. initializing from a null-safe value when v is a local nullable pointer lvalue or a local reference to nullable pointer (v is a local variable)
//
//       The scope of the null-safety proof does not exceed the scope of the local lvalue or reference.
//
//       Due to non-nullable to nullable conversion this includes initializing a nullable pointer
//       from a non-nullable (see 5.5).
//    2. assignment from a null-safe value when v is a local nullable pointer lvalue or a local non-const nullable pointer lvalue reference
//
//       The scope of the null-safety proof does not exceed the scope of the local lvalue or reference.
//
//       Due to non-nullable to nullable conversion this includes assigning a non-nullable pointer
//       to a nullable (see 6.5).
//    3. when v is one of:
//       a. lvalue
//       b. lvalue reference to non-volatile
//       c. lvalue reference to a local volatile pointer that has not escaped (see 12.5)
//       d. rvalue reference ???
//
//       branching in the form of:
//       1. an assertion that is false when v == nullptr
//
//          The null-safety proof is established for every subsequent statement.
//       2. a conditional expression a ? b : c, where the result of a depends on the result of v == nullptr
//          1. when a is false if v == nullptr then a null-safety proof for v is established in b
//          2. when a is true if v == nullptr then a null-safety proof for v is established in c
//
//          The scope of the null-safety proof does not exceed the scope of the sub-expression in which it is established.
//       3. an if statement where the result of the condition expression c depends on the result of v == nullptr
//          1. when c is false if v == nullptr then a null-safety proof for v is established in the true substatement
//             1. if the false sub-statement terminates with a return statement (in all control-flow paths), and v is not initialized in c,
//             then the scope of null-safety proof for v applies to all statements following if
//          2. when c is true if v == nullptr the a null-safety proof for v is established in the false substatement
//             1. if the true sub-statement terminates with a return statement (in all control-flow paths) and v is not initialized in c,
//             then the scope of null-safety proof for v applies to all subsequent statements following if
//       4. a switch statement where the result of the condition expression c depends on the result of v == nullptr
//          1. when c is false if v == nullptr then a null-safety proof for v is established within the case statement that matches c
//             and any subsequent case statement that is reachable from the absence of a break statement.
//          2. when c is true if v == nullptr then a null-safety proof for v is established within all case statements that do not match c
//             and any subsequent case statemets that are reachable from the absence of a break statement.
//       5. a while statement where the condition expression c depends on the the result of v == nullptr
//          1. when c is false if v == nullptr then a null-safety proof for v is established within the sub-statement
//          2. when c is true if v == nullptr
//             and there are no branches terminating in a break within the loop body
//             then a null-safety proof for v is established for subsequent statements of while
//       6. a do-while statement where the condition expression c depends on the result of v == nullptr
//          1. when c is true when v == nulptr
//             and there are no control-flow paths terminating in a break
//             then a null-safety proof for v is established for every subsequent statement following do-while
//       7. a for statement, for (a ; b ; any) block
//          1. the b expression depends on the result of v == nullptr
//             1. when b is false if v == nullptr then a null-safety proof for v is established within block
//                and the scope of the null-safety proof does not exceed that of block
//    B. establishing a null-safety proof on a nullable pointer value also establishes a null-safety proof for every local aliasing value
//       ; all aliases established within the same function scope, or global initializer scope
// 9. the scope of a null-safety proof begins from the point it is established
//
//    For example, assignment from a null-safe does not establish a null-safety proof for the value
//    prior to the assignment.
// 10. the scope of a null-safety proof does not exceed the following:
//    1. the scope of the initializing expression of a non-local variable
//    2. the scope of the function or lambda expression body in which the proof is established
//
//    Null-safety proofs established within a function are not returned to a caller via return nor via throw.
//    Null-safety proofs established for a function call parameter are not passed into the callee.
//    Null-safery proofs are not propagated by lambda captures.
//    Class-scope null-safety proofs do not exist.
// 11. global variables of nullable pointer type are never null-safe FIXME This is false, they are simply invalidated via any call
//     FIXME already covered by 12.2
//
//    Static local variables can be null-safe under the same rules for local variables.
// 12. a null-safety proof is not permanent and can be invalidated by subsequent statements;
//    a nullable pointer that has its null-safety proof invalidated is no longer null-safe
//    1. assignment of nullptr to a nullable pointer invalidates any null-safety proof that may hold on the nullable pointer
//    2. a null-safety proof established on a non-local value or reference is invalidated by any call expression
//
//       Non-local refers to object members and global variables; it does not apply to lamda captures for which see 10.4
//    3. a null-safety proof established on a function argument that is a non-volatile lvalue reference, regardless of const qualification,
//       is invalidated by any call expression
//
//       This applies regardless of const qualification of the lvalue reference.
//       For rvalue reference arguments the invalidation rules for lvalues apply.
//       There can be no null-safety proof for volatile nullable pointer lvalue references (see 8.)
//    4. a null-safety proof established on a non-volatile lvalue reference lambda capture, regardless of const qualification,
//       is invalidated upon any call expression within the lamda body
//
//       lvalue captures are not invalidated by this rule, but rules for pointer escapes still apply.
//    5. any call expression within a function or lambda body, global variable/member initializer expression
//       invalidates the null-safety proofs of all nullable pointers which have escaped the scope of the body or initializer expression
//
//       a pointer escapes the scope by:
//       1. assigning the address of the pointer to a non-local pointer or reference to a non-const pointer or const volatile pointer ???
//          assigning to a non-local const pointer does not invalidate a null-safety proof
//       2. passing the address of the pointer as a call expression parameter of pointer to non-const pointer or const volatile pointer ???
//       3. using the address of the pointer in direct- or copy-list-initilization of objects of class type
//          or aggregate initialization to initialize a pointer to a non-const pointer or const volatile pointer ???
//       4. passing a non-const lvalue reference or a const volatile lvalue reference to the pointer as a call expression parameter
//
//          Passing an lvalue reference to the pointer does not constitute a pointer escape.
//
//       A pointer escape can happen at any point, prior or after the establishment of a null-safety proof.
//       "Address of the pointer" includes the address directly as well as any pointer whose value is the address of said pointer.
//
//
//    Call expressions include invoking functions, methods, constructors, labmda expressions, objects with overloaded call operators, destructors.

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h"
#include "clang/AST/ParentMap.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/StmtVisitor.h"
#include "llvm/ADT/SetOperations.h"

#include <numeric>
#include <variant>

// #define DEBUG_DUMP 1

#ifdef DEBUG_DUMP
#define DUMP(Stmt) do { Stmt; } while (false)
#else
#define DUMP(Stmt) do { } while(false)
#endif

using namespace clang;
using namespace ento;

namespace {

// TODO!  Use something more efficient than string comparison
// for detection.
const StringRef NotNullPtrQualifiedName {"gsl::not_null"};
const StringRef MakeNotNullQualifiedName {"gsl::make_not_null"};
const StringRef StdForwardQualifiedName {"std::forward"};
const StringRef StdMoveQualifiedName {"std::move"};

template <typename T>
void dump_line(const T &V, StringRef What) {
#ifdef DEBUG_DUMP
  llvm::outs() << What << ": ";
  V.dump();
  llvm::outs() << "\n";
#endif
}

template <typename T>
void dump_color(const T &V, StringRef What) {
#ifdef DEBUG_DUMP
  llvm::outs() << What << "::\n";
  V.dumpColor();
  llvm::outs() << "\n";
#endif
}

template <typename T>
void dump_block(const T &V, StringRef What) {
#ifdef DEBUG_DUMP
  llvm::outs() << What << "::\n";
  V.dump();
  llvm::outs() << "\n";
#endif
}

void dump_type(QualType T, StringRef What) {
#ifdef DEBUG_DUMP
  llvm::outs() << What << " type: ";
  llvm::outs() << T.getAsString() << "\n";
  T.dump();
  llvm::outs() << "\n";
#endif
}

enum class NullableKind { NonNullable, Nullable, VolatileNullable };
// enum class NullConstraint { IsNull, IsNotNull, Unknown };
using NullConstraint = UnknownNullableSVal::NullConstraint;

NullConstraint negateNullConstraint(NullConstraint C) {
  switch (C) {
  case NullConstraint::IsNull:
    return NullConstraint::IsNotNull;
  case NullConstraint::IsNotNull:
    return NullConstraint::IsNull;
  default:
    return C;
  }
}

StringRef nullabilityToString(NullableKind N) {
  switch (N) {
  case NullableKind::NonNullable:
    return "NonNullable";
  case NullableKind::Nullable:
    return "Nullable";
  case NullableKind::VolatileNullable:
    return "VolatileNullable";
  }
}

StringRef nullConstraintToString(NullConstraint C) {
  switch (C) {
  case NullConstraint::IsNotNull:
    return "not-null";
  case NullConstraint::IsNull:
    return "null";
  case NullConstraint::Unknown:
    return "unknown";
  }
}

bool isLoopStmt(const Stmt *S) {
  return isa_and_nonnull<ForStmt, WhileStmt, DoStmt>(S);
}

const Stmt *getLoopCondition(const Stmt *S) {
  assert(isLoopStmt(S) && "statement must be a loop");

  switch (S->getStmtClass()) {
  case Stmt::WhileStmtClass: {
    const auto *L = dyn_cast<WhileStmt>(S);
    return L->getCond();
  }
  case Stmt::DoStmtClass: {
    const auto *L = dyn_cast<DoStmt>(S);
    return L->getCond();
  }
  case Stmt::ForStmtClass: {
    const auto *L = dyn_cast<ForStmt>(S);
    return L->getCond();
  }
  default:
    llvm_unreachable("not a loop statement");
  }

  return nullptr;
}

const Type *getNakedType(QualType T) {
  const auto *TT = T.getNonReferenceType()->getUnqualifiedDesugaredType();
  if (TT->isReferenceType()) return TT->getPointeeType()->getUnqualifiedDesugaredType();
  return TT;
}

const CXXRecordDecl *getCXXRecordFromType(QualType T) {
  return getNakedType(T)->getAsCXXRecordDecl();
}

bool isNonNullableType(QualType T) {
  const auto *R = getCXXRecordFromType(T);
  if (!R) return false;
  return R->getQualifiedNameAsString() == NotNullPtrQualifiedName;
}

const FieldDecl *getNonNullableField(QualType T) {
  const auto *R = getCXXRecordFromType(T);
  if (!R) return nullptr;

  auto F = llvm::find_if(R->fields(), [](const FieldDecl* FD) { return FD->getName() == "ptr_"; });
  if (F == R->field_end()) return nullptr;
  return *F;
}

bool isExprANotNullPtr(const Expr *E) {
  if (!E) return false;

  return isNonNullableType(E->getType());
}

bool isRawPointerType(QualType T) {
  return T->isPointerType() || T->isMemberPointerType();
}

bool isNakedRawPointerType(QualType T) {
  const auto *NT = getNakedType(T);
  return NT->isPointerType() || NT->isMemberPointerType();
}

bool isLocType(QualType T) {
  return T->isLValueReferenceType() || T->isPointerType() || T->isMemberPointerType();
}

bool isConstLocType(QualType T) {
  return isLocType(T) && T->getPointeeType().isConstQualified();
}

unsigned int getLocTypeRank(QualType T) {
  if (!isLocType(T)) return 0;

  unsigned int R = T->isLValueReferenceType() ? 1 : 0;
  do {
    T = T->getPointeeType();
    R++;
  } while (isLocType(T));

  return R - 1;
}

bool isNullableObject(const CXXRecordDecl *R) {
  if (!R) return false;

  const bool HasDefaultCtor = llvm::any_of(R->ctors(), [](const CXXConstructorDecl *Ctor) {
    return Ctor->isDefaultConstructor() && Ctor->getAccess() == AS_public;
  });
  if (!HasDefaultCtor) {
    DUMP(llvm::outs() << "no default constructor\n");
    return false;
  }

  const bool HasCopyCtor = llvm::any_of(R->ctors(), [](const CXXConstructorDecl *Ctor) {
    return Ctor->isCopyConstructor() && Ctor->getAccess() == AS_public;
  });
  if (!HasCopyCtor && !(R->hasSimpleCopyConstructor() || R->hasTrivialCopyConstructor())) {
    DUMP(llvm::outs() << "no copy constructor\n");
    return false;
  }

  const bool HasCopyAssignment = llvm::any_of(R->ctors(), [](const CXXConstructorDecl *Ctor) {
    return Ctor->isCopyAssignmentOperator() && Ctor->getAccess() == AS_public;
  });
  if (!HasCopyAssignment && !(R->hasSimpleCopyAssignment() || R->hasTrivialCopyAssignment())) {
    DUMP(llvm::outs() << "no copy assignment\n");
    return false;
  }

  const bool HasNullPtrConversionCtor = llvm::any_of(R->ctors(), [](const CXXConstructorDecl *Ctor) {
    return Ctor->isConvertingConstructor(true) && Ctor->getAccess() == AS_public && !Ctor->param_empty() && (*Ctor->param_begin())->getType().getCanonicalType()->isNullPtrType();
  });
  if (!HasNullPtrConversionCtor) {
    DUMP(llvm::outs() << "no nullptr conversion\n");
    return false;
  }

  const bool HasBoolConversion = llvm::any_of(R->getVisibleConversionFunctions(), [](const NamedDecl *Conv) {
    const auto *C = dyn_cast<CXXConversionDecl>(Conv);
    if (!C) return false;
    return C->getReturnType().getCanonicalType()->isBooleanType();
  });
  if (!HasBoolConversion) {
    DUMP(llvm::outs() << "no bool conversion\n");
    return false;
  }

  const bool HasEQOp = llvm::any_of(R->friends(), [R](const FriendDecl *Friend) {
    const auto *N = Friend->getFriendDecl();
    const auto *F = dyn_cast<FunctionDecl>(N);
    if (!F) return false;

    if (F->isOverloadedOperator() && F->getOverloadedOperator() == OO_EqualEqual) return false;

    auto LHST = F->parameters()[0]->getType();
    auto RHST = F->parameters()[1]->getType();
    if (LHST.isNull() && RHST.isNull()) return false;

    const auto *LHSR = getCXXRecordFromType(LHST);
    const auto *RHSR = getCXXRecordFromType(RHST);

    return (LHSR && RHSR && LHSR == RHSR && LHSR == R) || (LHSR && LHSR == R && RHST->isNullPtrType()) || (RHSR && RHSR == R && LHST->isNullPtrType());
  });
  if (!HasEQOp) {
    DUMP(llvm::outs() << "no == operator\n");
    return false;
  }

  const bool HasInEQOp = llvm::any_of(R->friends(), [R](const FriendDecl *Friend) {
    const auto *N = Friend->getFriendDecl();
    const auto *F = dyn_cast<FunctionDecl>(N);
    if (!F) return false;

    if (F->isOverloadedOperator() && F->getOverloadedOperator() == OO_ExclaimEqual) return false;

    auto LHST = F->parameters()[0]->getType();
    auto RHST = F->parameters()[1]->getType();
    if (LHST.isNull() && RHST.isNull()) return false;

    const auto *LHSR = getCXXRecordFromType(LHST);
    const auto *RHSR = getCXXRecordFromType(RHST);

    return (LHSR && RHSR && LHSR == RHSR && LHSR == R) || (LHSR && LHSR == R && RHST->isNullPtrType()) || (RHSR && RHSR == R && LHST->isNullPtrType());
  });
  if (!HasInEQOp) {
    DUMP(llvm::outs() << "no != operator\n");
    return false;
  }

  return true;
}

bool isNullableObjectType(QualType T) {
  const auto *R = getCXXRecordFromType(T);
  if (!R) return false;
  return isNullableObject(R);
}

bool isNullablePointerType(QualType T) {
  return isNakedRawPointerType(T) || isNullableObjectType(T);
}

const MemRegion *getLValLocRegion(const MemRegion *R) {
  if (!R) return nullptr;
  switch (R->getKind()) {
    // these are regions of lvalues
  case MemRegion::FieldRegionKind:
  case MemRegion::ParamVarRegionKind:
  case MemRegion::NonParamVarRegionKind:
    return R;
  // case MemRegion::SymbolicRegionKind: {
  //   auto Sym = R->getAs<SymbolicRegion>()->getSymbol();
  //   if (!Sym) return nullptr;
  //   return getLValLocRegion(Sym->getOriginRegion());
  // }
  default:
    return nullptr;
  }
}

const MemRegion *getLValLocRegion(DefinedOrUnknownSVal Val) {
  if (!isa<Loc>(Val)) return nullptr;
  if (Val.getSubKind() != loc::MemRegionValKind) return nullptr;
  const auto *R = Val.castAs<loc::MemRegionVal>().getRegion();
  return getLValLocRegion(R);
}

bool isLValLoc(DefinedOrUnknownSVal Val) {
  return getLValLocRegion(Val);
  // if (!isa<Loc>(Val)) return false;
  // if (Val.getSubKind() != loc::MemRegionValKind) return false;
  // const auto *R = Val.castAs<loc::MemRegionVal>().getRegion();
  // if (!R) return false;
  // switch (R->getKind()) {
  //   // these are regions of lvalues
  //   case MemRegion::FieldRegionKind:
  //   case MemRegion::ParamVarRegionKind:
  //   case MemRegion::NonParamVarRegionKind:
  //     return true;
  //   default:
  //     return false;
  // }
}

const MemRegion *getGLValLocRegion(const MemRegion *R) {
  if (!R) return nullptr;
  switch (R->getKind()) {
    // these are regions of lvalues
  case MemRegion::FieldRegionKind:
  case MemRegion::ParamVarRegionKind:
  case MemRegion::NonParamVarRegionKind:
    // these are xvalue regions
  case MemRegion::CXXTempObjectRegionKind:
  case MemRegion::CXXLifetimeExtendedObjectRegionKind:
    return R;
  // case MemRegion::SymbolicRegionKind: {
  //   auto Sym = R->getAs<SymbolicRegion>()->getSymbol();
  //   if (!Sym) return nullptr;
  //   return getGLValLocRegion(Sym->getOriginRegion());
  // }
  default:
    return nullptr;
  }
}

const MemRegion *getGLValLocRegion(DefinedOrUnknownSVal Val) {
  if (!isa<Loc>(Val)) return nullptr;
  if (Val.getSubKind() != loc::MemRegionValKind) return nullptr;
  const auto *R = Val.castAs<loc::MemRegionVal>().getRegion();
  return getGLValLocRegion(R);
}

bool isGLValLoc(DefinedOrUnknownSVal Val) {
  return getGLValLocRegion(Val);
  // if (!isa<Loc>(Val)) return false;
  // if (Val.getSubKind() != loc::MemRegionValKind) return false;
  // const auto *R = Val.castAs<loc::MemRegionVal>().getRegion();
  // if (!R) return false;
  // switch (R->getKind()) {
  //   // these are regions of lvalues
  //   case MemRegion::FieldRegionKind:
  //   case MemRegion::ParamVarRegionKind:
  //   case MemRegion::NonParamVarRegionKind:
  //   // these are xvalue regions
  //   case MemRegion::CXXTempObjectRegionKind:
  //   case MemRegion::CXXLifetimeExtendedObjectRegionKind:
  //     return true;
  //   default:
  //     return false;
  // }
}

bool isLambdaRefCaptureRegion(const MemRegion *R) {
  if (!R) return false;
  if (R->getKind() != MemRegion::SymbolicRegionKind) return false;
  const auto *SR = cast<SymbolicRegion>(R);
  const auto *S = dyn_cast<SymbolRegionValue>(SR->getSymbol());
  if (!S) return false;

  if (!S->getType()->isReferenceType()) return false;

  const auto *RR = S->getRegion();
  if (RR->getKind() != MemRegion::FieldRegionKind) return false;
  const auto *FR = cast<FieldRegion>(RR);

  return FR->getDecl()->getParent()->isLambda();
}

QualType getRegionValueType(const MemRegion *R) {
  if (!R) return {};

  if (R->getKind() >= MemRegion::BEGIN_TYPED_VALUE_REGIONS && R->getKind() <= MemRegion::END_TYPED_VALUE_REGIONS) {
    const auto *TR = cast<TypedValueRegion>(R);
    return TR->getValueType();
  }
  if (R->getKind() == MemRegion::SymbolicRegionKind) {
    const auto *SR = cast<SymbolicRegion>(R);
    const auto *S = SR->getSymbol();
    if (!S) return {};
    return S->getType();
  }

  return {};
}


bool isGlobalRegion(const MemRegion *R) {
  if (!R) return false;
  if (isLambdaRefCaptureRegion(R)) return true;
  auto &RM = R->getMemRegionManager();
  return R->isSubRegionOf(RM.getGlobalsRegion(MemRegion::GlobalSystemSpaceRegionKind))
    || R->isSubRegionOf(RM.getGlobalsRegion(MemRegion::GlobalImmutableSpaceRegionKind))
    || R->isSubRegionOf(RM.getGlobalsRegion(MemRegion::GlobalInternalSpaceRegionKind))
    || R->isSubRegionOf(RM.getUnknownRegion());
}

bool isGlobalSVal(SVal V) {
  return isGlobalRegion(V.getAsRegion());
}

enum class PointerAddressEscapeKind {
  NonVolatile,
  Volatile
};

std::optional<PointerAddressEscapeKind> getPointerAddressEscapeKind(const MemRegion *TR, const MemRegion *FR) {
  if (!isGlobalRegion(TR)) return std::nullopt;
  const auto TRT = getRegionValueType(TR);
  dump_type(TRT, "region to");
  const auto FRT = getRegionValueType(FR);
  dump_type(FRT, "region from");
  const bool IsEscapeToLValRef = TRT->isLValueReferenceType() && isNakedRawPointerType(TRT->getPointeeType()) && isNakedRawPointerType(FRT);
  const bool IsEscapeToLVal = isNakedRawPointerType(TRT) && isNakedRawPointerType(TRT->getPointeeType()) && !TRT->getPointeeType().isConstQualified() && isNakedRawPointerType(FRT);
  if (IsEscapeToLVal || IsEscapeToLValRef) {
    return TRT->getPointeeType().isVolatileQualified() ? PointerAddressEscapeKind::Volatile : PointerAddressEscapeKind::NonVolatile;
  }

  return std::nullopt;
}

struct PointerAddressEscape {
  enum Kind {
    NonVolatile,
    Volatile
  };

  Kind Kind;
  const MemRegion *Region;
};



bool isLocalRegion(const MemRegion *R, const LocationContext *LCtx) {
  if (!R) return false;

  dump_line(*R, "member expr val region");
  // if (const auto *SR = R->getAs<SymbolicRegion>()) {
  //   dump_line(*SR, "symbolic region");
  //   if (const auto *S = SR->getSymbol()) {
  //     dump_line(*S, "symbol");
  //     if (const auto *RS = dyn_cast<SymbolRegionValue>(S)) {
  //       dump_line(*RS, "symbol region value");
  //       dump_line(*RS->getRegion(), "region");
  //       R = RS->getRegion();
  //     }
  //   }
  // }
  if (const auto *FR = R->getAs<FieldRegion>()) {
    DUMP(llvm::outs() << "field region\n");
    if (const auto *SR = FR->getBaseRegion()) {
      dump_line(*SR, "base region");
      if (const auto *SymR = SR->getAs<SymbolicRegion>()) {
        if (const auto *Sym = SymR->getSymbol()) {
          if (const auto *SymV = dyn_cast<SymbolRegionValue>(Sym)) {
            if (SymV->getRegion()->getKind() == MemRegion::CXXThisRegionKind) {
              return FR->getValueType().getCanonicalType().isConstQualified();
            }
          }
        }
      }
      if (const auto *BR = SR->getAs<CXXBaseObjectRegion>()) {
        dump_line(*BR, "base object region");
        if (const auto *PR = BR->getAs<ParamVarRegion>()) {
          dump_line(*PR, "param var region");
        }
      }
    }
    if (const auto *SR = FR->getSuperRegion()) {
      dump_line(*SR, "super region");
      if (const auto *PR = SR->getAs<VarRegion>()) {
        dump_line(*PR, "var region");
        if (const auto *SR = PR->getSuperRegion()) {
          dump_line(*SR, "super region");
        }
      }
    }
  }

  const auto *STC = LCtx->getStackFrame();
  if (!STC) return false;
  auto &RM = R->getMemRegionManager();
  const auto *StackArgsRegion = RM.getStackArgumentsRegion(STC);
  const auto *FrameRegion = RM.getStackLocalsRegion(STC);
  if (StackArgsRegion || FrameRegion)
    DUMP(llvm::outs() << "checking stack\n");

  return (StackArgsRegion && R->isSubRegionOf(StackArgsRegion)) || (FrameRegion && R->isSubRegionOf(FrameRegion));
}

bool isLocalVal(SVal V, const LocationContext *LCtx) {
  return isLocalRegion(V.getAsRegion(), LCtx);
}

NullConstraint getNullConstraint(SVal V, ProgramStateRef State) {
  if (V.isUndef()) return NullConstraint::Unknown;
  if (V.isUnknownNullable()) return NullConstraint::Unknown;

  auto Val = V.castAs<DefinedOrUnknownSVal>();

  if (auto U = Val.getAs<UnknownNullableSVal>()) {
    return U->getConstraint();
  }

  // if (auto MP = Val.getAs<nonloc::PointerToMember>()) {
  //   if (MP->isUnknownMemberPointer()) return NullConstraint::Unknown;
  //   return MP->isNullMemberPointer() ? NullConstraint::IsNull : NullConstraint::IsNotNull;
  // }

  if (const auto *R = Val.getAsRegion()) {
    if (const auto *ER = R->getAs<ElementRegion>()) {
      if (const auto *SR = ER->getSuperRegion()) {
        if (const auto *TSR = SR->getAs<TypedValueRegion>()) {
          // dump_type(TSR->getValueType(), "element region super");

          return TSR->getValueType()->isArrayType() ? NullConstraint::IsNotNull : NullConstraint::Unknown;
        }
      }
    }
    if (const auto *CR = R->getAs<CodeTextRegion>()) {
      return NullConstraint::IsNotNull;
    }
  }

  ConditionTruthVal Nullness = State->isNull(Val);
  if (Nullness.isConstrainedFalse())
    return NullConstraint::IsNotNull;
  if (Nullness.isConstrainedTrue())
    return NullConstraint::IsNull;
  if (isGLValLoc(Val)) {
    // addresses of lvalues are never null
    dump_line(Val, "is a Loc of a glvalue");
    return NullConstraint::IsNotNull;
  }
  return NullConstraint::Unknown;
}

bool isNonNullableGLValLoc(DefinedOrUnknownSVal Val) {
  if (!isGLValLoc(Val)) {DUMP(llvm::outs() << "not glval loc\n"); return false;}
  const auto *R = Val.castAs<loc::MemRegionVal>().getRegionAs<FieldRegion>();
  if (!R) {DUMP(llvm::outs() << "no region of glval loc\n"); return false;}
  const auto *F = R->getDecl();
  if (!F) return false;
  const auto *P = F->getParent();
  if (!P) return false;

  return P->getQualifiedNameAsString() == NotNullPtrQualifiedName;
}

bool isNullableObjectMemberFieldLoc(Loc L) {
  const auto *R = L.castAs<loc::MemRegionVal>().getRegionAs<FieldRegion>();
  if (!R) return false;
  const auto *F = R->getDecl();
  if (!F) return false;
  const auto *P = dyn_cast<CXXRecordDecl>(F->getParent());
  if (!P) return false;

  return isNullableObject(P);
}


inline bool isRegionNotNullPtrField(const MemRegion* R) {
  const auto *F = dyn_cast_or_null<FieldRegion>(R);
  if (!F) return false;

  const auto *D = F->getDecl();
  if (!D) return false;

  const auto *C = D->getParent();
  if (!C) return false;

  return C->getQualifiedNameAsString() == NotNullPtrQualifiedName;
}

// bool isDereferenceExpr(const Expr *E, const CheckerContext &C) {
//   switch (E->getStmtClass()) {
//     case Stmt::MemberExprClass:
//       return true;
//     default:
//       const auto &PMap = C.getLocationContext()->getParentMap();
//       const auto *Parent = PMap.getParentIgnoreParenImpCasts(const_cast<Expr*>(E));
//       if (!Parent) return false;
//       switch (Parent->getStmtClass()) {
//         default:
//           break;
//         case Stmt::UnaryOperatorClass:
//             const auto *Op = dyn_cast<UnaryOperator>(Parent);
//             return Op->getOpcode() == UnaryOperatorKind::UO_Deref;
//       }

//       return false;
//   }
// }

inline bool isDeclRefExprToReference(const Expr *E) {
  if (const auto *DRE = dyn_cast<DeclRefExpr>(E))
    return DRE->getDecl()->getType()->isReferenceType();
  return false;
}

// const Expr *getDereferenceExpr(const Stmt *S, bool IsBind=false){
//   const Expr *E = nullptr;

//   // Walk through lvalue casts to get the original expression
//   // that syntactically caused the load.
//   if (const Expr *expr = dyn_cast<Expr>(S))
//     E = expr->IgnoreParenLValueCasts();

//   if (IsBind) {
//     const VarDecl *VD;
//     const Expr *Init;
//     std::tie(VD, Init) = parseAssignment(S);
//     if (VD && Init)
//       E = Init;
//   }
//   return E;
// }

bool isInterProcedural(const FunctionDecl *FD) {
  auto FDName = FD->getQualifiedNameAsString();
  DUMP(llvm::outs() << "function name: " << FDName << "\n");
  if (FDName == MakeNotNullQualifiedName) return true;
  if (StringRef(FDName).startswith(NotNullPtrQualifiedName)) return true;
  if (FDName == StdForwardQualifiedName) return true;
  if (FDName == StdMoveQualifiedName) return true;
  return false;
}

bool isInterProcedural(const CallEvent &CE) {
  const auto *D = CE.getDecl();
  if (!D) return false;
  const auto *F = D->getAsFunction();
  if (!F) return false;
  return isInterProcedural(F);
}

bool isStdAddressOf(const CallEvent &CE) {
  const auto *D = CE.getDecl();
  if (!D) return false;
  const auto *F = D->getAsFunction();
  if (!F) return false;
  return F->getQualifiedNameAsString() == "std::addressof";
}

bool ignoreEscapeAnalysis(const FunctionDecl *FD) {
  auto FDName = FD->getQualifiedNameAsString();
  if (FDName == MakeNotNullQualifiedName) return true;
  if (FDName == StdForwardQualifiedName) return true;
  if (FDName == StdMoveQualifiedName) return true;
  if (StringRef(FDName).startswith(NotNullPtrQualifiedName)) return true;
  return false;
}

bool ignoreEscapeAnalysis(const CallEvent &CE) {
  const auto *D = CE.getDecl();
  if (!D) return false;
  const auto *F = D->getAsFunction();
  if (!F) return false;

  return ignoreEscapeAnalysis(F);
}

ProgramStateRef constrainPtr(ProgramStateRef State, CheckerContext &C, Loc L, QualType T, bool isNull) {
  auto &SVB = C.getSValBuilder();
  auto Zero = SVB.makeZeroVal(T).castAs<Loc>();

  auto Constraint = SVB.evalBinOp(State, BO_EQ, L, Zero, C.getASTContext().BoolTy);
  return State->assume(Constraint.castAs<DefinedSVal>(), isNull);
}

ProgramStateRef constrainPtrNotNull(ProgramStateRef State, CheckerContext &C, Loc L, QualType T) {
  return constrainPtr(State, C, L, T, false);
}

class NullablePointersChecker : public Checker<check::Location,
                                               check::BeginFunction,
                                               // check::EndFunction,
                                               check::Bind,
                                               check::PreStmt<UnaryOperator>,
                                               check::PostStmt<UnaryOperator>,
                                               check::PreStmt<BinaryOperator>,
                                               check::PreStmt<MemberExpr>,
                                               check::PostStmt<MemberExpr>,
                                               check::PostStmt<MaterializeTemporaryExpr>,
                                               check::PostStmt<DeclStmt>,
                                               // check::PostStmt<ImplicitCastExpr>,
                                               check::PreCall,
                                               check::PostCall,
                                               check::BranchCondition,
                                               check::LoopCondition,
                                               check::PostStmt<BreakStmt>,
                                               check::PointerEscape,
                                               eval::Call,
                                               eval::Assume> {
  BugType BugNullablePointerDereference{this, "dereference of a nullable pointer", categories::LogicError};
  BugType BugNullalbeNullptrDeference{this, "nullptr dereference of a nullable pointer", categories::LogicError};
  BugType BugNonNullableHasNull{this, "non-nullable pointer has nullptr value", categories::LogicError};
  BugType BugNullableToNonNullable{this, "converting a nullable pointer to non-nullable", categories::LogicError};
  BugType BugNullptrToNonNullable{this, "assigning nullptr to non-nullable", categories::LogicError};
  BugType BugUndefinedNullable{this, "nullable pointer is undefined", categories::LogicError};
  BugType BugUndefinedNonNullable{this, "non-nullable pointer is undefined", categories::LogicError};
  BugType BugVolatilePointerEscape{this, "non-nullable pointer escapes as volatile", categories::LogicError};

  void reportNonNullableFromNullable(ProgramStateRef State, const CallEvent &Call, CheckerContext &C) const;
  void reportBug(const BugType &BT, ProgramStateRef State, const Stmt *S, CheckerContext &C) const;

  // class NullableParamVisitor : public BugReporterVisitor {
  // public:
  //   NullableParamVisitor(const Expr *E) : DerefExpr(E) {}

  //   void Profile(llvm::FoldingSetNodeID &ID) const override {
  //     static int X = 0;
  //     ID.AddPointer(&X);
  //     ID.AddPointer(DerefExpr);
  //   }

  //   PathDiagnosticPieceRef VisitNode(const ExplodedNode *N,
  //                                    BugReporterContext &BRC,
  //                                    PathSensitiveBugReport &BR) override {
  //     if (!N) return nullptr;
  //     return nullptr;
  //   }
  // private:
  //   const Expr *DerefExpr;
  //   mutable bool IsContrained = false;
  // };

  class NullableProofVisitor : public BugReporterVisitor {
  public:
    NullableProofVisitor(SVal PointerLoc) : PointerLoc(PointerLoc) {}

    void Profile(llvm::FoldingSetNodeID &ID) const override {
      static int X = 0;
      ID.AddPointer(&X);
      ID.Add(PointerLoc);
      // ID.AddPointer(PointerLocRegion);
    }

    PathDiagnosticPieceRef VisitNode(const ExplodedNode *N,
                                     BugReporterContext &BRC,
                                     PathSensitiveBugReport &BR) override;
  private:
    SVal PointerLoc;
    // mutable const CFGBlock *NullableSafetyProofBlock = nullptr;
    // mutable std::shared_ptr<PathDiagnosticEventPiece> DelayedDiagnostic;
  };

  class NullableFixItVisitor : public BugReporterVisitor {
  public:
    NullableFixItVisitor(Loc PointerLoc, const Expr *DerefExpr) : PointerLoc(PointerLoc), DerefExpr(DerefExpr) {}

    PathDiagnosticPieceRef VisitNode(const ExplodedNode *N,
                                     BugReporterContext &BRC,
                                     PathSensitiveBugReport &BR) override;
    void finalizeVisitor(BugReporterContext &BRC,
                         const ExplodedNode *EndPathNode,
                         PathSensitiveBugReport &BR) override;

    void Profile(llvm::FoldingSetNodeID &ID) const override {
      static int X = 0;
      ID.AddPointer(&X);
      ID.Add(PointerLoc);
      ID.AddPointer(DerefExpr);
    }
  private:
    Loc PointerLoc;
    const Expr *DerefExpr;
    mutable bool IsConstrained = false;
  };

  class NullableSourceVisitor : public BugReporterVisitor {
  public:
    NullableSourceVisitor(const Expr *E) : RefExpr(E) {}

    void Profile(llvm::FoldingSetNodeID &ID) const override {
      static int X = 0;
      ID.AddPointer(&X);
      ID.AddPointer(RefExpr);
    }

    PathDiagnosticPieceRef VisitNode(const ExplodedNode *N,
                                     BugReporterContext &BRC,
                                     PathSensitiveBugReport &BR) override {
      if (!N) return nullptr;
      DUMP(llvm::outs() << "NullableSourceVisitor::VisitNode ProgramPoint BEGIN\n");
      DUMP(N->getLocation().dump());
      DUMP(llvm::outs() << "\nEND\n");
      if (Emitted) return nullptr;
      // if (N->getLocation().getKind() != ProgramPoint::PostStmtKind) return nullptr;
      const auto *S = N->getStmtForDiagnostics();
      if (!S) return nullptr;
      if (S != RefExpr) return nullptr;

      Emitted = true;
      switch (RefExpr->getStmtClass()) {
      case Stmt::DeclRefExprClass: {
        const auto *DR = dyn_cast<DeclRefExpr>(RefExpr);
        const auto *D = DR->getFoundDecl();
        const auto *FD = D->getAsFunction();

        if (FD) {
          const auto Loc = FD->getReturnTypeSourceRange().getBegin();
          PathDiagnosticLocation Pos(Loc.isValid() ? Loc : FD->getBeginLoc(), BRC.getSourceManager());
          return std::make_shared<PathDiagnosticEventPiece>(Pos, "declared returning nullable pointer here", true);
        }

        PathDiagnosticLocation Pos(D, BRC.getSourceManager());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "declared as nullable pointer here", true);
      }
      case Stmt::BinaryOperatorClass: {
        const auto *BO = dyn_cast<BinaryOperator>(RefExpr);
        if (BO->getOpcode() != BO_PtrMemD) return nullptr;

        BR.addVisitor<NullableSourceVisitor>(BO->getRHS()->IgnoreUnlessSpelledInSource());

        PathDiagnosticLocation Pos(BO, BRC.getSourceManager(), N->getLocationContext());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "member access through pointer-to-member here", true);
      }
      case Stmt::MemberExprClass: {
        const auto *ME = dyn_cast<MemberExpr>(RefExpr);
        const auto *MD = ME->getMemberDecl();

        PathDiagnosticLocation Pos(MD, BRC.getSourceManager());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "member field declared as nullable pointer here", true);
      }
      case Stmt::CXXMemberCallExprClass: {
        const auto *CE = dyn_cast<CXXMemberCallExpr>(RefExpr);
        const auto *TE = CE->getCallee()->IgnoreUnlessSpelledInSource();
        dump_color(*TE, "callee expr");

        BR.addVisitor<NullableSourceVisitor>(TE);

        PathDiagnosticLocation Pos(CE, BRC.getSourceManager(), N->getLocationContext());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "member function called here", true);
      }
      case Stmt::CallExprClass: {
        const auto *CE = dyn_cast<CallExpr>(RefExpr);
        const auto *TE = CE->getCallee()->IgnoreUnlessSpelledInSource();
        dump_color(*TE, "callee expr");

        BR.addVisitor<NullableSourceVisitor>(TE);

        PathDiagnosticLocation Pos(CE, BRC.getSourceManager(), N->getLocationContext());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "called here", true);
      }
      default:
        PathDiagnosticLocation Pos(S, BRC.getSourceManager(), N->getLocationContext());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "nullable pointer obtained via", true);
      }
    }
  private:
    const Expr *RefExpr;
    mutable bool Emitted = false;
  };
public:
  void checkLocation(SVal location, bool isLoad, const Stmt* S,
                     CheckerContext &C) const;
  void checkBind(SVal L, SVal V, const Stmt *S, CheckerContext &C) const;
  void checkBeginFunction(CheckerContext &C) const;
  // void checkEndFunction(CheckerContext &C) const;
  void checkPreStmt(const UnaryOperator *S, CheckerContext &C) const;
  void checkPostStmt(const UnaryOperator *S, CheckerContext &C) const;
  void checkPreStmt(const BinaryOperator *S, CheckerContext &C) const;
  void checkPreStmt(const MemberExpr *S, CheckerContext &C) const;
  void checkPostStmt(const MemberExpr *S, CheckerContext &C) const;
  void checkPostStmt(const MaterializeTemporaryExpr *S, CheckerContext &C) const;
  void checkPostStmt(const DeclStmt *S, CheckerContext &C) const;
  void checkPostStmt(const ImplicitCastExpr *S, CheckerContext &C) const;
  void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
  void checkPostCall(const CallEvent &Call, CheckerContext &C) const;
  void checkBranchCondition(const Stmt *S, CheckerContext &C) const;
  void checkLoopCondition(const Stmt *L, const Stmt *S, CheckerContext &C) const;
  void checkPostStmt(const BreakStmt *S, CheckerContext &C) const;
  ProgramStateRef checkPointerEscape(ProgramStateRef State,
                                     const InvalidatedSymbols &Escaped,
                                     const CallEvent *Call,
                                     PointerEscapeKind Kind) const;
  ProgramStateRef checkPointerAddressEscapes(ProgramStateRef State, Loc L, SVal V, const ASTContext &C) const;
  ProgramStateRef evalAssume(ProgramStateRef State, SVal Cond, bool Assumption) const;
  bool evalCall(const CallEvent &CE, CheckerContext &C) const;

  void checkPointer(SVal V, const Stmt *S, ProgramStateRef State, CheckerContext &C) const;
  bool checkCalleeExpr(const Expr *E, ProgramStateRef State, CheckerContext &C) const;

  static void AddDerefSource(raw_ostream &os,
                             SmallVectorImpl<SourceRange> &Ranges,
                             const Expr *Ex, const ProgramState *state,
                             const LocationContext *LCtx,
                             bool loadedFrom = false);
};

#ifdef DEBUG_DUMP
#define CHECK_RETURN do { llvm::outs() << "NULLABLE POINTERS\n\n"; return; } while(false)
#define CHECK_RETURN_LOG(m) do { llvm::outs() << m << "\n"; CHECK_RETURN; } while(false)
#else
#define CHECK_RETURN return
#define CHECK_RETURN_LOG(m) return
#endif

enum class ConstraintValidity {
Valid,
AlwaysValid,
Invalid,
AlwaysInvalid
};

struct EscapedNullableState {
  NullConstraint Constraint;
  ConstraintValidity Validity;

  void Profile(llvm::FoldingSetNodeID &ID) const {
    ID.AddInteger(static_cast<int>(Constraint));
    ID.AddInteger(static_cast<int>(Validity));
  }

  bool operator==(const EscapedNullableState &R) const {
    return Constraint == R.Constraint && Validity == R.Validity;
  }

  bool operator<(const EscapedNullableState &R) const {
    return Constraint < R.Constraint && Validity < R.Validity;
  }
};

struct NullableObjectStateKey {
  using KeyData = llvm::PointerUnion<const MemRegion *, SymbolRef>;
  KeyData Data;

  explicit operator bool() const {
    return !Data.isNull();
  }

  void Profile(llvm::FoldingSetNodeID &ID) const {
    ID.AddPointer(Data.getOpaqueValue());
  }

  bool operator==(const NullableObjectStateKey &O) const {
    return Data == O.Data;
  }

  bool operator<(const NullableObjectStateKey &O) const {
    return Data < O.Data;
  }

  void dump() const {
    if (Data.is<const MemRegion *>()) {
      if (const auto *R = Data.get<const MemRegion*>()) R->dump();
      return;
    }

    if (Data.is<SymbolRef>()) {
      if (const auto *S = Data.get<SymbolRef>()) S->dump();
      return;
    }
  }
};

struct LoopState {
  // struct Constraint {
  //   SymbolRef Sym;
  //   bool Assumption;

  //   bool operator==(const Constraint &O) const {
  //     return Sym == O.Sym && Assumption == O.Assumption;
  //   }
  //   bool operator<(const Constraint &O) const {
  //     return Sym < O.Sym && Assumption < O.Assumption;
  //   }

  //   void Profile(llvm::FoldingSetNodeID &ID) const {
  //     ID.AddPointer(Sym);
  //     ID.AddBoolean(Assumption);
  //   }
  // };
  // const ExplodedNode *Node;
  // const SVal LoopCondition;
  // const llvm::ImmutableSet<Constraint> Constraints;
  const Stmt *Loop;

  void Profile(llvm::FoldingSetNodeID &ID) const {
    ID.AddPointer(Loop);
    // LoopCondition.Profile(ID);
  }

  // ProgramStateRef record(ProgramStateRef State, Constraint C) const;
  // bool isInvariant(ProgramStateRef State, const Stmt *S, CheckerContext &C) const;

  // ProgramStateRef weaken(ProgramStateRef State) const;

  // static ProgramStateRef push(ProgramStateRef State, const ExplodedNode *N, SVal LoopCondition);
  static ProgramStateRef push(ProgramStateRef State, const Stmt* Loop);
  static ProgramStateRef pop(ProgramStateRef State);
  static std::optional<LoopState> top(ProgramStateRef State);
};



struct CallEventKey {
  const FunctionDecl *F;
  const llvm::ImmutableList<SVal> ArgVals;

  CallEventKey(ProgramStateRef State, const AnyFunctionCall &CE);

  void Profile(llvm::FoldingSetNodeID &ID) const {
    ID.AddPointer(F);
    ArgVals.Profile(ID);
  }

  bool operator==(const CallEventKey &RHS) const {
    return F == RHS.F && ArgVals.isEqual(RHS.ArgVals);
  }

  bool operator<(const CallEventKey &RHS) const {
    const bool L = F == RHS.F;
    if (!L) return false;

    bool R = false;
    const auto LE = ArgVals.end();
    const auto RE = RHS.ArgVals.end();
    auto LI = ArgVals.begin();
    auto RI = RHS.ArgVals.begin();

    while (LI != LE && RI != RE) {
      R = LI->getAsRegion() < RI->getAsRegion();

      ++LI;
      ++RI;
      if (LI == LE && RI == RE) R = false;
      if (LI == LE && RI != RE) R = true;
      if (LI != LE && RI == RE) R = false;
    }

    return R;
  }
};


enum class Weakening {
  Variant,
  Invariant
};

constexpr std::string_view to_string(Weakening W) {
  switch (W) {
  case Weakening::Variant: return "Variant";
  case Weakening::Invariant: return "Invariant";
  }
}

llvm::raw_ostream& operator<<(llvm::raw_ostream &O, Weakening W) {
  O << to_string(W);
  return O;
}

}


REGISTER_MAP_WITH_PROGRAMSTATE(NullabilityMap, const MemRegion*, NullableKind)
REGISTER_MAP_WITH_PROGRAMSTATE(EscapedNullable, const MemRegion*, EscapedNullableState)
REGISTER_SET_WITH_PROGRAMSTATE(NonLocalNullable, const MemRegion*)

REGISTER_LIST_FACTORY_WITH_PROGRAMSTATE(CallEventArgVals, SVal)
REGISTER_MAP_WITH_PROGRAMSTATE(ConstantCallEvents, CallEventKey, SymbolRef)

// REGISTER_SET_WITH_PROGRAMSTATE(LambdaRefCaptures, const ValueDecl*)

REGISTER_MAP_WITH_PROGRAMSTATE(DNOConstraintMap, NullableObjectStateKey, NullConstraint)
REGISTER_SET_FACTORY_WITH_PROGRAMSTATE(DNOAliasSet, NullableObjectStateKey)
REGISTER_MAP_WITH_PROGRAMSTATE(DNOAliasMap, NullableObjectStateKey, DNOAliasSet)
REGISTER_MAP_WITH_PROGRAMSTATE(PtrToDNOAliasMap, SymbolRef, DNOAliasSet)
REGISTER_SET_FACTORY_WITH_PROGRAMSTATE(SymAliasSet, SymbolRef)
REGISTER_MAP_WITH_PROGRAMSTATE(DNOToPtrAliasMap, NullableObjectStateKey, SymAliasSet)
REGISTER_SET_WITH_PROGRAMSTATE(PtrGuards, SymbolRef)
REGISTER_TRAIT_WITH_PROGRAMSTATE(AliasGuard, bool)

// REGISTER_SET_FACTORY_WITH_PROGRAMSTATE(LoopConstraintSet, LoopState::Constraint)
REGISTER_LIST_WITH_PROGRAMSTATE(LoopStack, LoopState)
// REGISTER_MAP_WITH_PROGRAMSTATE(LoopConstraintMap, const ExplodedNode*, LoopConstraintSet)

// REGISTER_SET_WITH_PROGRAMSTATE(WeakenSet, const Stmt *)
REGISTER_MAP_WITH_PROGRAMSTATE(WeakenMap, const Stmt *, Weakening)
REGISTER_LIST_FACTORY_WITH_PROGRAMSTATE(SynonymList, const Stmt *)
REGISTER_MAP_WITH_PROGRAMSTATE(WeakenSynonyms, const Stmt *, SynonymList)

namespace {


llvm::ImmutableList<SVal> getCallArgSVals(ProgramStateRef State, const AnyFunctionCall &CE) {
  auto &F = State->get_context<CallEventArgVals>();
  auto Args = F.getEmptyList();

  for (unsigned int i = 0; i < CE.getNumArgs(); ++i) {
    Args = F.add(CE.getArgSVal(i), Args);
  }

  return Args;
}

CallEventKey::CallEventKey(ProgramStateRef State, const AnyFunctionCall &CE)
  : F(CE.getDecl()), ArgVals(getCallArgSVals(State, CE))
{ }

// ProgramStateRef LoopState::record(ProgramStateRef State, Constraint C) const {
//   auto &F = State->get_context<LoopConstraintSet>();
//   return State->set<LoopConstraintMap>(Node, F.add(Constraints, C));
// }

bool areStatesEqual(ProgramStateRef SL, ProgramStateRef SR, CheckerContext &C) {
  return C.getStateManager().haveEqualEnvironments(SL, SR) && C.getStateManager().haveEqualStores(SL, SR);
}

bool isStmtContained(const Stmt *Ancestor, const Stmt *Descendant, ASTContext &C) {
  using namespace clang::ast_matchers;

  const auto Matcher = anyOf(equalsNode(Descendant), hasDescendant(equalsNode(Descendant)));
  const auto Results = match(stmt(Matcher), *Ancestor, C);

  return !Results.empty();
}
bool isDeclContained(const Stmt *Ancestor, const Decl *Descendant, ASTContext &C) {
  using namespace clang::ast_matchers;

  const auto Results = match(stmt(hasDescendant(equalsNode(Descendant))), *Ancestor, C);

  return !Results.empty();
}

llvm::SmallSetVector<const DeclRefExpr *, 3> getModifiedDeclRefs(const Stmt *S, ASTContext &C) {
  // mods(S)
  // - DeclRefExprs appearing on lhs of built-in operators: =, +=, -=, *=, /=, %=, ^=, &=, |=, ++, --
  // - DeclRefExprs appearing on rhs of built-in operators: ++, --
  // - DeclRefExprs passed as lvalue refs to calls
  // - DeclRefExprs of objects that have non-const method calls
  using namespace clang::ast_matchers;

  const auto DeclRefExprMatcher = declRefExpr(to(anyOf(varDecl(), fieldDecl()))).bind("declref");
  const auto OperatorMatcher = binaryOperator(isAssignmentOperator(), hasLHS(ignoringImplicit(DeclRefExprMatcher)));

  llvm::SmallSetVector<const DeclRefExpr *, 3> Mods;
  const auto Results = match(stmt(forEachDescendant(OperatorMatcher)), *S, C);
  for (const auto &R : Results) {
    const auto *E = R.getNodeAs<DeclRefExpr>("declref");
    Mods.insert(E);
  }

  return Mods;
}

using StmtSetVector = llvm::SmallSetVector<const Stmt*, 3>;
AST_MATCHER_P(Stmt, equalsAnyNode, StmtSetVector, Other) {
  return Other.contains(&Node);
}

const Stmt *getGuardedLoop(const Expr *E, ASTContext &C) {
  using namespace clang::ast_matchers;

  const auto LoopMatcher = stmt(anyOf(whileStmt(), doStmt(), forStmt())).bind("loop");
  const auto LoopGuardMatcher = callExpr(hasAncestor(LoopMatcher));
  return selectFirst<Stmt>("loop", match(LoopGuardMatcher, *E, C));
}

bool isNullSafeExpr(const Expr *E) {
  if (!isRawPointerType(E->getType())) return false;

  E = E->IgnoreParens();

  switch (E->getStmtClass()) {
  case Stmt::CXXNewExprClass: {
    const auto *NewOp = cast<CXXNewExpr>(E)->getOperatorNew();
    if (!NewOp) return false;
    auto ExcSpec = NewOp->getExceptionSpecType();
    return ExcSpec != EST_BasicNoexcept && ExcSpec != EST_NoexceptTrue;
  }
  case Stmt::CXXThisExprClass:
    return true;
  case Stmt::UnaryOperatorClass:
    return cast<UnaryOperator>(E)->getOpcode() == UO_AddrOf;
  case Stmt::ImplicitCastExprClass: {
    const auto *CE = cast<ImplicitCastExpr>(E);
    const auto CK = CE->getCastKind();
    if (CK == CK_ArrayToPointerDecay || CK == CK_FunctionToPointerDecay) return true;
    if (CK != CK_UserDefinedConversion) return false;
    const auto *ME = dyn_cast<CXXMemberCallExpr>(CE->getSubExpr());
    if (!ME) return false;
    return isNonNullableType(ME->getObjectType());
  }
  default:
    return false;
  }

  return false;
}

// struct FreeField {
//   llvm::PointerUnion<const FieldDecl*, const IndirectFieldDecl*> Field;
//   bool IsArrow;
//   const Expr *Base;
// };

// using FreeTy = std::variant<const VarDecl*, FreeField>;
// using FreeSetTy = llvm::SmallSetVector<FreeTy, 10>;

// FreeSetTy getFreeNonConstDecls(const Stmt *S, ASTContext &C) {
//   using namespace clang::ast_matchers;

//   FreeSetTy Free;

//   const auto VarDeclMatcher = declRefExpr(to(varDecl(unless(hasType(isConstQualified()))).bind("varDecl")));
//   const auto FieldDeclRefExprMatcher = memberExpr(
//     member(valueDecl(anyOf(fieldDecl(unless(hasType(isConstQualified()))).bind("fieldDecl"),
//                            indirectFieldDecl(unless(hasType(isConstQualified()))).bind("indirectFieldDecl"))))).bind("memberExpr");

//   const auto DeclMatcher = expr(anyOf(VarDeclMatcher, FieldDeclRefExprMatcher));

//   const auto Results = match(stmt(forEachDescendant(DeclMatcher)), *S, C);
//   for (const auto &Result : Results) {
//     if (const auto *Var = Result.getNodeAs<VarDecl>("varDecl")) {
//       if (isDeclContained(S, Var, C)) continue;
//       Free.insert(Var);
//     }
//     if (const auto *Field = Result.getNodeAs<FieldDecl>("fieldDecl")) {
//       if (const auto *Member = Result.getNodeAs<MemberExpr>("memberExpr")) {
//         Free.insert(FreeField { Field, Member->isArrow(), Member->getBase() });
//       }
//     }
//     if (const auto *Field = Result.getNodeAs<IndirectFieldDecl>("indirectFieldDecl")) {
//       if (const auto *Member = Result.getNodeAs<MemberExpr>("memberExpr")) {
//         Free.insert(FreeField { Field, Member->isArrow(), Member->getBase() });
//       }
//     }
//   }

//   return Free;
// }

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

using ModExprs_t = SmallVector<const Expr*, 5>;

using WeakenExprs_t = SmallVector<std::pair<const Expr*, Weakening>, 5>;

struct ModsVisitor : ConstStmtVisitor<ModsVisitor> {
  // determines what kind of weakening to perform
  // on lvalues used in a loop
  enum class ModKind {
    None, // no weakening
    Unsafe, // maximum weakening; rebind to new unconstrained symbolic value
    Safe, // invariant weakening; rebind to new symbolic value and if pointer retain previous null cosntraint
    ConditionalSafe, // undetermined; depends on ModKind of other lvalues
                     // this is resolved at the end of mods analysis to one of the other values:
                     // - if all dependencies are None then None
                     // - if all dependencies are Safe then Safe
                     // - if any dependency is Unsafe then Unsafe
                     // - for each ConditionalSafe dependency
                     //   - if it is mutual then Safe
                     //   - otherwise as above
  };
  struct Loc {
    using OfTy = llvm::SmallSetVector<const ValueDecl*, 3>;
    OfTy Of;
    ModKind Modified = ModKind::None;
    bool IsConstant = false;

    Loc() = default;
    Loc(OfTy &&Of, ModKind Modified, bool IsConstant) : Of(std::move(Of)), Modified(Modified), IsConstant(IsConstant) {}
    Loc(OfTy &&Of, ModKind Modified) : Loc(std::move(Of), Modified, false) {}
    Loc(ModKind Modified) : Loc({}, Modified, false) {}
  };
  using LocRef = std::shared_ptr<Loc>;
  struct NonLoc {};
  using NonLocRef = std::shared_ptr<NonLoc>;
  using Val = std::variant<NonLocRef, LocRef>;
  using ValGroup = SmallVector<Val, 3>;
  using LocGroup = SmallVector<LocRef, 3>;
  std::map<LocRef, ValGroup> LocEnv;
  std::map<const Expr *, ValGroup>  ExprEnv;
  std::map<const ValueDecl *, LocGroup> ValEnv;

  LocGroup LocDependencies; // records locs required to evaluate current expr
  std::map<LocRef, LocGroup> ConditionalSafetyDependencies; // records mod dependencies of locs for resolution of ConditionalSafe

  const Stmt *Scope = nullptr;
  bool Modifying = false;

  ProgramStateRef State;
  const LocationContext *LCtx;

  ModsVisitor(ProgramStateRef State, const LocationContext *LCtx) : State(State), LCtx(LCtx) {}

  auto ModifiedDecls() {
    return SmallVector<const ValueDecl*, 3> {
      llvm::make_first_range(llvm::make_filter_range(ValEnv, [](const auto &VL) {
        return llvm::any_of(VL.second, [](const auto &L) { return L->Modified == ModKind::Unsafe; });
      }))
    };
  }

  auto ModifiedLocExprs() {
    return ModExprs_t {
      llvm::make_first_range(llvm::make_filter_range(ExprEnv, [](const auto &EV) {
        return llvm::any_of(EV.second, [](const auto &V) {
          if (const auto *L = std::get_if<LocRef>(&V))
            return L->get()->Modified != ModKind::None;
          return false;
        });
      }))
    };
  }

  auto WeakenLocExprs() {
    auto ModLocs = llvm::make_filter_range(ExprEnv, [](const auto &EV) {
      return llvm::any_of(EV.second, [](const auto &V) {
        if (const auto *L = std::get_if<LocRef>(&V))
          return L->get()->Modified != ModKind::None;
        return false;
      });
    });
    auto WeakenLocs = llvm::map_range(ModLocs, [](const auto &EV) {
      const auto WK = [&EV]() {
        const auto &[_, Vs] = EV;
        if (Vs.empty()) return Weakening::Variant;

        const bool Unsafe = llvm::any_of(Vs, [](const auto &V) {
          if (const auto *L = std::get_if<LocRef>(&V))
            return L->get()->Modified == ModKind::Unsafe;
          return false;
        });
        if (Unsafe) return Weakening::Variant;

        const bool Safe = llvm::all_of(Vs, [](const auto &V) {
          if (const auto *L = std::get_if<LocRef>(&V))
            return L->get()->Modified == ModKind::Safe;
          return false;
        });
        if (Safe) return Weakening::Invariant;

        return Weakening::Variant;
      }();
      return std::make_pair(EV.first, WK);
    });

    return WeakenExprs_t { WeakenLocs };
  }

  auto ModifiedSynonyms(const Expr *E) {
    auto ExprLocs = llvm::make_filter_range(ExprEnv[E], [](const auto &V) {
      return std::get_if<LocRef>(&V);
    });
    return ModExprs_t {
      llvm::make_first_range(llvm::make_filter_range(ExprEnv, [&ExprLocs](const auto &EV) {
        return llvm::any_of(EV.second, [&ExprLocs](const auto &V) {
          return llvm::any_of(ExprLocs, [&V](const auto &V_) { return V == V_; });
        });
      }))
    };
  }

  unsigned int locRank(QualType T) {
    unsigned int R = 0;
    while (isLocType(T)) {
      T = T->getPointeeType();
      R++;
    }

    return R;
  }

  LocGroup locsWithVal(Val V) {
    LocGroup Ls;
    for (auto &LVs : LocEnv) {
      if (llvm::find(LVs.second, V)) Ls.push_back(LVs.first);
    }

    return Ls;
  }

  void modifyReachable(Val V, ModKind MK) {
    std::visit(overloaded {
        [&](NonLocRef V) {
          for (auto &L : locsWithVal(V)) modify(L, MK);
        },
        [&](LocRef L) {
          if (!L->IsConstant)
            modify(L, MK);
          for (auto &V : LocEnv[L])
            modifyReachable(V, MK);
        }
      }, V);
}

  void modifyReachable(const ValGroup &Vs, ModKind MK) {
    for (auto &V : Vs)
      modifyReachable(V, MK);
  }

  void modify(const LocRef &L, ModKind MK) {
    if (Modifying)
      L->Modified = MK;
  }

  ValGroup locAsVal(LocGroup Ls) {
    ValGroup Vs;
    for (auto &L : Ls) Vs.push_back(L);
    return Vs;
  }

  void bindUndef(const ValueDecl *VD) {
    bindVal(VD, { std::make_shared<NonLoc>() });
  }

  void bindNonLoc(const ValueDecl *VD) {
    bindVal(VD, { std::make_shared<NonLoc>() });
  }

  void bindLoc(const ValueDecl *VD) {
    auto L = std::make_shared<Loc>();
    bindVal(VD, { L });
    LocEnv.emplace(L, ValGroup { std::make_shared<NonLoc>() });
  }

  LocRef bindExpr(const Expr *E) {
    auto R = locRank(E->getType());
    if (!R) {
      ExprEnv.emplace(E, ValGroup { std::make_shared<NonLoc>() });
      return nullptr;
    }

    auto L = std::make_shared<Loc>();
    ExprEnv.emplace(E, ValGroup { L });
    do {
      auto V = R-- ? Val { L = std::make_shared<Loc>() } : Val { std::make_shared<NonLoc>() };
      LocEnv.emplace(L, ValGroup { V });
    } while (R);
    return L;
  }

  void bindVal(const ValueDecl *VD, ValGroup Vs) {
    auto L = makeVarLoc(VD);
    ValEnv.emplace(VD, LocGroup { {L} });
    LocEnv.emplace(L, Vs);
  }

  LocRef makeVarLoc(const ValueDecl *VD) {
    Loc::OfTy Of;
    Of.insert(VD);
    return std::make_shared<Loc>(
      std::move(Of),
      ModKind::None,
      VD->getType().isConstQualified() || (VD->getType()->isReferenceType() && VD->getType()->getPointeeType().isConstQualified()));
  }

  LocGroup getLoc(const Expr *E) {
    LocGroup Ls;
    for (auto &V : ExprEnv[E])
      if (auto *L = std::get_if<LocRef>(&V)) Ls.push_back(*L);

    return Ls;
  }

  void VisitCXXNullPtrLiteralExpr(const CXXNullPtrLiteralExpr *E) {
    auto L = std::make_shared<Loc>(ModKind::Unsafe);
    ExprEnv.emplace(E, ValGroup { L });
    LocDependencies.push_back(L);
  }

  void VisitCXXThisExpr(const CXXThisExpr *E) {
    auto L = bindExpr(E);
    assert(L && "binding this expr to a null loc");
    L->Modified = ModKind::None;
    LocDependencies.push_back(L);
  }

  void VisitDeclRefExpr(const DeclRefExpr *E) {
    const auto *VD = E->getDecl();
    if (ExprEnv.count(E)) {
      LocDependencies.append(ValEnv[VD]);
      return;
    }

    // do not know this declref in the env
    // create a binding
    // FIXME! use locRank for the bindings
    if (!isLocType(VD->getType())) bindNonLoc(VD);
    else bindLoc(VD);

    auto &Ls = ValEnv[VD];
    ExprEnv.emplace(E, ValGroup { Ls.begin(), Ls.end() });
    LocDependencies.append(Ls);
    return;
  }

  void VisitCXXNewExpr(const CXXNewExpr *E) {
    auto L = bindExpr(E);
    LocDependencies.push_back(L);
  }

  //TODO! explicit and implicit const casting?
  void VisitImplicitCastExpr(const ImplicitCastExpr *E) {
    Visit(E->getSubExpr());

    switch (E->getCastKind()) {
    case CK_LValueToRValue: {
      // We no longer depend on lvalue but on the rvalue
      LocDependencies.clear();
      ValGroup Vs;
      for (auto &SubV : ExprEnv[E->getSubExpr()])
        if (auto *L = std::get_if<LocRef>(&SubV))
          for (auto &V : LocEnv[*L]) {
            Vs.push_back(V);
            LocDependencies.push_back(*L);
          }

      ExprEnv.emplace(E, Vs);
      return;
    }
    case CK_ArrayToPointerDecay:
    case CK_FunctionToPointerDecay: {
      auto L = bindExpr(E);
      LocDependencies.push_back(L);
      return;
    }
    case CK_UserDefinedConversion: {
      const auto *ME = dyn_cast<CXXMemberCallExpr>(E->getSubExpr());
      assert(ME && "user defined conversion without member call");
      const auto MK = isNonNullableType(ME->getObjectType()) ? ModKind::None : ModKind::Unsafe;
      auto L = bindExpr(E);
      L->Modified = MK;
      if (MK == ModKind::Unsafe)
        LocDependencies.push_back(L);
      return;
    }
    default:
      ExprEnv.emplace(E, ExprEnv[E->getSubExpr()]);
      return;
    }

    // if (E->getCastKind() != CK_LValueToRValue) {
    //   ExprEnv.emplace(E, ExprEnv[E->getSubExpr()]);
    //   return;
    // }

    // ValGroup Vs;
    // for (auto &SubV : ExprEnv[E->getSubExpr()])
    //   if (auto *L = std::get_if<LocRef>(&SubV))
    //     for (auto &V : LocEnv[*L])
    //       Vs.push_back(V);

    // ExprEnv.emplace(E, Vs);
  }

  void VisitUnaryAddrOf(const UnaryOperator *O) {
    Visit(O->getSubExpr());

    ExprEnv[O] = ExprEnv[O->getSubExpr()];
    // null-safety of &x does not depend on x
    LocDependencies.clear();
  }

  void VisitUnaryDeref(const UnaryOperator *O) {
    Visit(O->getSubExpr());

    ExprEnv[O] = ExprEnv[O->getSubExpr()];
    // for (auto &SubV : ExprEnv[O])
    //   if (auto *L = std::get_if<LocRef>(&SubV))
    //     LocDependencies.push_back(*L);
  }

  void VisitUnaryModify(const UnaryOperator *O) {
    Visit(O->getSubExpr());

    auto Ls = getLoc(O->getSubExpr());
    auto Vs = ExprEnv[O->getSubExpr()];

    for (auto &L: Ls) {
      // modify as Safe (invariant weakening)
      modify(L, ModKind::Safe);
      LocEnv[L] = Vs;
    }

    ExprEnv[O] = ExprEnv[O->getSubExpr()];
  }

  void VisitUnaryPostInc(const UnaryOperator *O) {
    VisitUnaryModify(O);
  }

  void VisitUnaryPostDec(const UnaryOperator *O) {
    VisitUnaryModify(O);
  }

  void VisitUnaryPreInc(const UnaryOperator *O) {
    VisitUnaryModify(O);
  }

  void VisitUnaryPreDec(const UnaryOperator *O) {
    VisitUnaryModify(O);
  }

  void VisitBinAssign(const BinaryOperator *O) {
    Visit(O->getLHS());
    auto LocDependencies_ = LocDependencies;
    LocDependencies.clear();
    Visit(O->getRHS());
    auto RHSLocDependencies = LocDependencies;
    LocDependencies_.append(LocDependencies);
    LocDependencies = std::move(LocDependencies_);

    auto Ls = getLoc(O->getLHS());
    auto Vs = ExprEnv[O->getRHS()];

    const auto MK = [&RHSLocDependencies,&Ls,O]() {
      if (!isRawPointerType(O->getLHS()->getType())) return ModKind::Safe;
      if (isa<CompoundAssignOperator>(O)) return ModKind::Safe;
      // if (RHSLocDependencies.empty()) return isNullSafeExpr(O->getRHS()) ? ModKind::Safe : ModKind::Unsafe;
      if (RHSLocDependencies.empty()) return ModKind::Safe;
      // if it depends on itself then it's safe; e.g. p = p + 1;
      for (auto &L: Ls)
        llvm::erase_value(RHSLocDependencies, L);
      if (RHSLocDependencies.empty()) return ModKind::Safe;
      // if it depends on others that are safe then the mod is safe
      // if it depends on others we need to decide on how others are modified
      return ModKind::ConditionalSafe;
    }();

    for (auto &L: Ls) {
      modify(L, MK);
      LocEnv[L] = Vs;
      if (MK == ModKind::ConditionalSafe)
        // we've filtered out Ls out of RHSLocDependencies at this point
        ConditionalSafetyDependencies[L].append(RHSLocDependencies);
    }

    ExprEnv[O] = ExprEnv[O->getLHS()];
  }

  void VisitCompoundAssignOperator(const CompoundAssignOperator *O) {
    VisitBinAssign(O);
  }

  void VisitCallExpr(const CallExpr *E) {
    assert(E->getCallee() && "CallExpr must call something");
    assert(E->getDirectCallee() && "CallExpr must call a function");
    const auto *Callee = E->getDirectCallee();
    const bool IsMethod = isa<CXXMethodDecl>(Callee);

    // dump_color(*Callee, "direct callee");

    for (unsigned int I = 0; I < E->getNumArgs(); ++I) {
      //FIXME Arg eval is in indeterminate order, so we need to
      //merge their resulting LocEvns
      Visit(E->getArg(I));

      if (IsMethod) {
        const auto *M = cast<CXXMethodDecl>(Callee);
        if (I == 0 && !M->isStatic() && M->isConst()) continue;
        if (I == 0 && !M->isStatic()) {
          modifyReachable(ExprEnv[E->getArg(I)], ModKind::Unsafe);
          continue;
        }
        if (I > 0 && !M->isStatic()) {
          if (isConstLocType(Callee->getParamDecl(I - 1)->getType())) continue;
          modifyReachable(ExprEnv[E->getArg(I)], ModKind::Unsafe);
          continue;
        }
      }

      if (isConstLocType(Callee->getParamDecl(I)->getType())) continue;
      modifyReachable(ExprEnv[E->getArg(I)], ModKind::Unsafe);
    }

    bindExpr(E);
    LocDependencies.clear();
  }

  void VisitVarDecl(const VarDecl *VD) {
    if (!VD->hasInit()) return bindUndef(VD);

    const auto *Init = VD->getInit();
    Visit(Init);

    if (!ExprEnv.count(Init)) return bindUndef(VD);

    if (VD->getType()->isLValueReferenceType()) {
      auto Ls = getLoc(Init);
      for (auto &L : Ls) L->Of.insert(VD);
      ValEnv.emplace(VD, Ls);
      return;
    }

    bindVal(VD, ExprEnv[Init]);

    auto &L = ValEnv[VD].front();
    L->Modified = ModKind::ConditionalSafe;
    ConditionalSafetyDependencies[L].append(LocDependencies);
    LocDependencies.clear();
  }

  void VisitDeclStmt(const DeclStmt *DS) {
    for (const auto *D : DS->decls()) {
      if (D->getKind() != Decl::Var) continue;
      VisitVarDecl(cast<VarDecl>(D));
    }
  }

  void VisitBranch(const Stmt *TrueS, const Stmt *FalseS) {
    auto TrueLocEnv = LocEnv;
    auto FalseLocEnv = LocEnv;

    if (TrueS) {
      LocEnv = TrueLocEnv;
      Visit(TrueS);
      TrueLocEnv = LocEnv;
    }

    if (FalseS) {
      LocEnv = FalseLocEnv;
      Visit(FalseS);
      FalseLocEnv = LocEnv;
    }

    LocEnv = TrueLocEnv;
    for (auto &LV : FalseLocEnv) {
      LocEnv[LV.first].append(LV.second);
    }
  }

  void VisitIfStmt(const IfStmt *S) {
    if (S->getConditionVariableDeclStmt()) Visit(S->getConditionVariableDeclStmt());
    if (S->getCond()) {
      Visit(S->getCond());
    }

    VisitBranch(S->getThen(), S->getElse());
  }

  void VisitBinLAnd(const BinaryOperator *O) {
    Visit(O->getLHS());
    Visit(O->getRHS());
    VisitBranch(O->getLHS(), O->getRHS());
  }

  void VisitBinLOr(const BinaryOperator *O) {
    Visit(O->getLHS());
    Visit(O->getRHS());
    VisitBranch(O->getLHS(), O->getRHS());
  }

  void VisitConditionalOperator(const ConditionalOperator *O) {
    Visit(O->getCond());
    Visit(O->getTrueExpr());
    Visit(O->getFalseExpr());

    VisitBranch(O->getTrueExpr(), O->getFalseExpr());
  }

  void resolveConditionalSafety(LocRef L, LocGroup &LocStack) {
    if (L->Modified != ModKind::ConditionalSafe) return;

    auto &Deps = ConditionalSafetyDependencies[L];
    assert(!Deps.empty() && "conditionaly safe modified lvalue without any dependencies");

    // remove any self-dependnecy, although there shouldn't be any at this point
    // TODO replace with assertion
    llvm::erase_value(Deps, L);
    if (Deps.empty()) {
      L->Modified = ModKind::Safe;
      return;
    }

    for (const auto &DepL : Deps) {
      assert(DepL && "conditional null-safety dependency is null");
      if (DepL->Modified != ModKind::ConditionalSafe) continue;
      if (llvm::is_contained(LocStack, DepL)) {
        // mutual dependency; resolve to safe
        DepL->Modified = ModKind::Safe;
      }
      LocStack.push_back(L);
      resolveConditionalSafety(DepL, LocStack);
      LocStack.pop_back();
    }

    assert(llvm::none_of(Deps, [](const auto &L) { return L->Modified == ModKind::ConditionalSafe; }) && "modified lvalue conditional safety dependencies remain conditional");

    if (llvm::all_of(Deps, [](const auto &L) { return L->Modified == ModKind::Safe; })) {
      L->Modified = ModKind::Safe;
      return;
    }
    if (llvm::any_of(Deps, [](const auto &L) { return L->Modified == ModKind::Unsafe; }))
    {
      L->Modified = ModKind::Unsafe;
      return;
    }

    assert(llvm::none_of(Deps, [](const auto &L) { return L->Modified == ModKind::Unsafe; }) && "unsafe dependency when only unmodified and safe expected");

    // resolve dependencies to unmodified locs
    for (const auto &DepL : Deps) {
      if (DepL->Modified != ModKind::None) continue;

      // if this loc is not the lvalue of value decl then
      // it's either some rvalue or an lvalue we do not reason about
      // and thus cannot determine it's safe
      if (DepL->Of.empty()) {
        L->Modified = ModKind::Unsafe;
        return;
      }

      for (const auto *VD : DepL->Of) {
        dump_color(*VD, "resolving unmodified conditional safety dependency");
        // FIXME add other decl cases
        if (const auto *VarD = dyn_cast<VarDecl>(VD)) {
          auto LV = State->getLValue(VarD, LCtx);
          dump_line(LV, "lvalue sval");
          auto RV = State->getSVal(LV);
          dump_line(RV, "rvalue sval");
          if (RV.isUndef()) continue; // probably a decl local to the loop; FIXME add special handling for these
          assert(isa<ento::Loc>(RV) && "conditional safety depdendency is not a loc");
          // if any unmodified dependency is not null-safe at this point treat the loc modification as unsafe
          if (getNullConstraint(RV, State) != NullConstraint::IsNotNull) {
            L->Modified = ModKind::Unsafe;
            return;
          }
        }
      }
    }
    // at this point all deps are either safe or they are unmodified and constrained to non-null
    L->Modified = ModKind::Safe;

    // L->Modified = std::accumulate(Deps.begin(), Deps.end(), ModKind::Safe, [](ModKind MK, const LocRef &L) {
    //   if (L->Modified == MK) return MK;
    //   // None is weakest
    //   if (MK == ModKind::None) return L->Modified;
    //   if (L->Modified == ModKind::None) return MK;
    //   // Unsafe is strongest
    //   if (L->Modified == ModKind::Unsafe) return L->Modified;
    //   if (MK == ModKind::Unsafe) return MK;

    //   llvm_unreachable("unresolved ModKind combination");
    // });

    // assert(L->Modified != ModKind::ConditionalSafe && "modified lvalue conditional safety is unresolved");
  }

  void VisitStmt(const Stmt *S) {
    bool Clear = false;
    if (!Modifying) {
      Modifying = Scope && Scope == S;
      Clear = true;
    }

    for (const auto *C : S->children())
      if (C)
        Visit(C);

    if (Clear) {
      Modifying = false;

      // resolve Locs with ModKind::ConditionalSafe
      assert(State && "cannot resolve conditional safety without sybmolic state");
      assert(LCtx && "cannot resolve conditional safety without a location context");
      for (auto &[L, _] : LocEnv) {
        LocGroup Ls;
        resolveConditionalSafety(L, Ls);
      }
    }
  }

  void VisitSwitchStmt(const SwitchStmt *S) {
    if (S->hasVarStorage()) VisitDeclStmt(S->getConditionVariableDeclStmt());
    Visit(S->getCond());

    const auto *Case = S->getSwitchCaseList();
    auto InitLocEnv = LocEnv;
    auto CurrLocEnv = InitLocEnv;
    while (Case) {
      LocEnv = InitLocEnv;
      Visit(Case->getSubStmt());
      for (auto &LV : LocEnv)
        CurrLocEnv[LV.first].append(LV.second);

      Case = Case->getNextSwitchCase();
    }

    LocEnv = CurrLocEnv;
  }

  void VisitCXXTryStmt(const CXXTryStmt *S) {
    auto InitLocEnv = LocEnv;
    auto CurrLocEnv = InitLocEnv;

    Visit(S->getTryBlock());
    for (auto &LV : LocEnv)
      CurrLocEnv[LV.first].append(LV.second);

    for (unsigned int I = 0; I < S->getNumHandlers(); ++I) {
      LocEnv = InitLocEnv;
      Visit(S->getHandler(I));

      for (auto &LV : LocEnv)
        CurrLocEnv[LV.first].append(LV.second);
    }

    LocEnv = CurrLocEnv;
  }

  void VisitCXXThrowExpr(const CXXThrowExpr *E) {
    // TODO
  }
  void VisitBreakStmt(const BreakStmt *S) {
    // TODO
  }
  void VisitContinueStmt(const ContinueStmt *S) {
    // TODO
  }
};

struct FreeAssignmentVisitor : ConstStmtVisitor<FreeAssignmentVisitor, bool> {
  bool VisitDeclRefExpr(const DeclRefExpr *E) {
    return E->getDecl() == Target;
  }
  bool VisitMemberExpr(const MemberExpr *E) {
    return E->getMemberDecl() == Target;
  }
  bool VisitBinAssign(const BinaryOperator *O) {
    return Visit(O->getLHS());
  }
  bool VisitCompoundAssignOperator(const CompoundAssignOperator *O) {
    return Visit(O->getLHS());
  }
  bool VisitComma(const BinaryOperator *O) {
    return Visit(O->getRHS());
  }
  bool VisitConditionalOperator(const ConditionalOperator *O) {
    return Visit(O->getLHS()) || Visit(O->getRHS());
  }
  bool VisitUnaryOperator(const UnaryOperator *O) {
    switch (O->getOpcode()) {
    case UO_PostInc:
    case UO_PostDec:
    case UO_PreInc:
    case UO_PreDec:
      return Visit(O->getSubExpr());
    default:
      return false;
    }
  }
  bool VisitCallExpr(const CallExpr *E) {
    return false;
  }
  bool VisitCXXOperatorCallExpr(const CXXOperatorCallExpr *O) {
    if (!O->isAssignmentOp()) return false;
    return Visit(O->getArg(0));
  }
  bool VisitStmt(const Stmt *S) {
    for (const auto *Child : S->children()) {
      if (Visit(Child)) return true;
    }
    return false;
  }

  const ValueDecl *Target;
};

struct FreeCallEscapeVisitor : ConstStmtVisitor<FreeCallEscapeVisitor, bool> {
  bool VisitCallExpr(const CallExpr *E) {
    for (unsigned int I = 0; I < E->getNumArgs(); ++I) {
      auto ParmTy = E->getDirectCallee()->getParamDecl(I)->getType();
      if (ParmTy.isConstQualified()) continue;

    }
    return false;
  }
  bool VisitStmt(const Stmt *S) {
    for (const auto *Child : S->children()) {
      if (Visit(Child)) return true;
    }
    return false;
  }

  bool isTargetCompatible(QualType T) const {
    if (T.isConstQualified()) return false;
    if (T->isLValueReferenceType()) return true;

    return false;
  }

  const ValueDecl *Target;
};

bool isVarDeclModified(const VarDecl *D, Stmt *S, ASTContext &C) {
  using namespace clang::ast_matchers;

  const auto DeclMatcher = declRefExpr(to(varDecl(equalsNode(D))));
  // NOTE! What about when the operatnd is a comma expr,  conditional expr?
  const auto AssignmentMatcher = binaryOperation(isAssignmentOperator(), hasLHS(ignoringImplicit(DeclMatcher)));
  const auto UnaryOpMatcher = unaryOperator(hasAnyOperatorName("++", "--"), hasUnaryOperand(ignoringImplicit(DeclMatcher)));
  const auto CXXUnaryOpMatcher = cxxOperatorCallExpr(hasAnyOverloadedOperatorName("++", "--"), hasUnaryOperand(ignoringImplicit(DeclMatcher)));

  return false;
}

using ModifiedTy = llvm::PointerUnion<const VarDecl*, const FieldDecl*, const IndirectFieldDecl*>;

llvm::SmallSetVector<ModifiedTy, 5> getModifiedDecls(const Stmt *S, ASTContext &C) {
  using namespace clang::ast_matchers;

  const auto VarDeclMatcher = declRefExpr(to(varDecl().bind("varDecl")));
  const auto FieldDeclRefExprMatcher = memberExpr(
    member(valueDecl(anyOf(fieldDecl().bind("fieldDecl"), indirectFieldDecl().bind("indirectFieldDecl")))));

  const auto DeclMatcher = expr(anyOf(VarDeclMatcher, FieldDeclRefExprMatcher));

  const auto AssignmentMatcher = binaryOperation(isAssignmentOperator(), hasLHS(ignoringImplicit(DeclMatcher)));
  const auto UnaryOpMatcher = unaryOperator(hasAnyOperatorName("++", "--"), hasUnaryOperand(ignoringImplicit(DeclMatcher)));
  const auto CXXUnaryOpMatcher = cxxOperatorCallExpr(hasAnyOverloadedOperatorName("++", "--"), hasUnaryOperand(ignoringImplicit(DeclMatcher)));
  const auto CallMatcher = callExpr(forEachArgumentWithParamType(ignoringImplicit(anyOf(DeclMatcher, unaryOperator(hasOperatorName("&"), hasUnaryOperand(DeclMatcher)))),
                                                                 qualType(hasCanonicalType(anyOf(qualType(lValueReferenceType(), references(qualType(unless(isConstQualified())))),
                                                                                             qualType(pointerType(), pointsTo(qualType(unless(isConstQualified())))))))));
  const auto MethodMatcher = cxxMemberCallExpr(on(DeclMatcher), unless(callee(cxxMethodDecl(isConst()))));

  const auto ModifiedMatcher = stmt(forEachDescendant(expr(anyOf(MethodMatcher, CallMatcher, CXXUnaryOpMatcher, UnaryOpMatcher, AssignmentMatcher))));

  llvm::SmallSetVector<ModifiedTy, 5> Modified;
  const auto Matches = match(ModifiedMatcher, *S, C);

  for (const auto &M : Matches) {
    ModifiedTy Mod;
    if (const auto *Var = M.getNodeAs<VarDecl>("varDecl")) {
      if (!isDeclContained(S, Var, C))
        Mod = Var;
    }
    if (const auto *Field = M.getNodeAs<FieldDecl>("fieldDecl"))
      Mod = Field;
    if (const auto *IndirectField = M.getNodeAs<IndirectFieldDecl>("indirectFieldDecl"))
      Mod = IndirectField;

    Modified.insert(Mod);
  }

  return Modified;
}

bool isPointerToPointer(QualType T) {
  if (T->isPointerType())
    return T->getPointeeType()->isPointerType();

  if (T->isMemberPointerType())
    return T->getPointeeType()->isPointerType();

  return false;
}

llvm::SmallSetVector<ModifiedTy, 5> modifiedClosure(const llvm::SmallSetVector<ModifiedTy, 5> &Modified, ProgramStateRef State, const LocationContext *LCtx) {
  llvm::SmallSetVector<ModifiedTy, 5> Modified_ = Modified;

  for (const auto &M : Modified) {
    if (const auto *Var = M.dyn_cast<const VarDecl*>()) {
      auto Ty = Var->getType();
      auto PointeeTy = Ty->getPointeeType();
      while (isPointerToPointer(Ty) && !PointeeTy.isConstQualified()) {
        auto LV = State->getLValue(Var, LCtx);
        dump_line(LV, "while loop modified pointe-to-pointer lval");
        auto V = State->getSVal(LV.castAs<Loc>());
        dump_line(V, "while loop modified pointe-to-pointer lval");


        Ty = PointeeTy;
        PointeeTy = Ty->getPointeeType();
      }
    }
  }

  return Modified_;
}

struct StmtCompareVisitor : ConstStmtVisitor<StmtCompareVisitor, bool, const Stmt *> {
  bool VisitDeclRefExpr(const DeclRefExpr *S, const Stmt *T) {
    if (T->getStmtClass() != Stmt::DeclRefExprClass) return false;

    return S->getDecl() == cast<DeclRefExpr>(T)->getDecl();
  }

  bool VisitDeclStmt(const DeclStmt *S, const Stmt *T) {
    if (T->getStmtClass() != Stmt::DeclStmtClass) return false;
    if (S == T) return true;
    return false;
  }

  bool VisitStmt(const Stmt *S, const Stmt *T) {
    if (S == T) return true;
    if (S->getStmtClass() != T->getStmtClass()) return false;
    if (llvm::range_size(S->children()) != llvm::range_size(T->children())) return false;

    auto IS = S->child_begin();
    auto IT = T->child_begin();
    auto ES = S->child_end();
    for (; IS != ES; ++IS, ++IT) {
      Visit(*IS, *IT);
    }

    return true;
  }
};

struct LoopGuardVisitor : ConstStmtVisitor<LoopGuardVisitor, bool> {
  ModExprs_t Modified;
  ModExprs_t ConditionModified;
  ModExprs_t ConditionUnModified;

  bool VisitWhileStmt(const WhileStmt *S) {
    if (VisitDeclStmt(S->getConditionVariableDeclStmt()))
      return true;

    const auto *Cond = S->getCond();
    assert(Cond->getStmtClass() == Stmt::BinaryOperatorClass && "while loop missing loop guard");
    Cond = cast<BinaryOperator>(S->getCond())->getRHS();

    return Visit(Cond);
  }

  bool VisitDeclStmt(const DeclStmt *DS) {
    if (!DS) return false;

    for (const auto *D : DS->decls()) {
      if (D->getKind() != Decl::Var) continue;
      const auto *VD = cast<VarDecl>(D);
      if (VD->hasInit() && !Visit(VD->getInit())) return false;
    }
    return true;
  }

  bool VisitBinComma(const BinaryOperator *E) {
    return Visit(E->getRHS());
  }

  bool VisitCallExpr(const CallExpr *E) {
    return false;
    // for (const auto *Arg : E->arguments())
    //   Visit(Arg);
  }

  bool VisitExpr(const Expr *E) {
    if (llvm::is_contained(Modified, E)) {
      ConditionModified.push_back(E);
      return false;
    } else if (E->isLValue()) ConditionUnModified.push_back(E);
    return VisitStmt(E);
  }

  bool VisitStmt(const Stmt *S) {
    for (const auto *C : S->children()) {
      if (!C) continue;
      if (!Visit(C)) return false;
    }

    return true;
  }
};

struct UnmodifiedLocExprVisitor : ConstStmtVisitor<UnmodifiedLocExprVisitor> {
  ModExprs_t Modified;
  ModExprs_t UnModified;

  void VisitExpr(const Expr *E) {
    if (!llvm::is_contained(Modified, E) && E->isGLValue()) UnModified.push_back(E);
    return VisitStmt(E);
  }

  void VisitStmt(const Stmt *S) {
    for (const auto *C : S->children())
      Visit(C);
  }
};

llvm::SmallSetVector<const DeclRefExpr *, 3> getModifiedDeclRefsNonNested(const Stmt *S, ASTContext &C) {
  // mods(S)
  // - DeclRefExprs appearing on lhs of built-in operators: =, +=, -=, *=, /=, %=, ^=, &=, |=, ++, --
  // - DeclRefExprs appearing on rhs of built-in operators: ++, --
  // - DeclRefExprs passed as lvalue refs to calls
  // - DeclRefExprs of objects that have non-const method calls
  using namespace clang::ast_matchers;

  const auto DeclRefExprMatcher = declRefExpr(to(anyOf(varDecl(), fieldDecl()))).bind("declref");
  const auto OperatorMatcher = binaryOperator(isAssignmentOperator(), hasLHS(ignoringImplicit(DeclRefExprMatcher)));

  const auto LoopMatcher = stmt(anyOf(whileStmt(), doStmt(), forStmt(), cxxForRangeStmt())).bind("loop");
  const auto NestedLoopResults = match(stmt(forEachDescendant(LoopMatcher)), *S, C);
  llvm::SmallSetVector<const Stmt*, 3> NestedLoops;
  for (const auto &R: NestedLoopResults) {
    NestedLoops.insert(R.getNodeAs<Stmt>("loop"));
  }

  llvm::SmallSetVector<const DeclRefExpr *, 3> Mods;
  const auto Results = match(stmt(forEachDescendant(stmt(OperatorMatcher, unless(hasAncestor(equalsAnyNode(NestedLoops)))))), *S, C);
  for (const auto &R : Results) {
    const auto *E = R.getNodeAs<DeclRefExpr>("declref");
    Mods.insert(E);
  }

  return Mods;
}

llvm::SmallSetVector<const DeclRefExpr *, 3> getFreeDeclRefs(const Stmt *S, ASTContext &C) {
  using namespace clang::ast_matchers;

  // Technically, we are not really capturing the free vars here;
  // we are currently including any bindings
  llvm::SmallSetVector<const DeclRefExpr *, 3> Free;
  const auto DeclRefExprMatcher = declRefExpr(to(anyOf(varDecl(), fieldDecl()))).bind("declref");
  const auto Results = match(stmt(forEachDescendant(DeclRefExprMatcher)), *S, C);
  for (const auto &R: Results) {
    const auto *E = R.getNodeAs<DeclRefExpr>("declref");
    Free.insert(E);
  }

  return Free;
}

llvm::SmallSetVector<const DeclRefExpr*, 3> getBranchedModifiedDeclRefs(const Stmt *S, const ExplodedNode *N, ASTContext &C) {
  llvm::SmallSetVector<const DeclRefExpr*, 3> BranchMods;
  const auto Mods = getModifiedDeclRefs(S, C);

  const CFGBlock *Start = N->getCFGBlock();
  SmallVector<const CFGBlock*, 4> Pending { Start };
  llvm::SmallSetVector<const CFGBlock*, 4> Visited;
  assert(Start && "Starting CFG block must exist");
  assert(Start->getTerminatorCondition() && "Starting CFG block must have a terminator condition");

  const CFGBlock *Current = nullptr;
  do {
    Current = Pending.pop_back_val();
    Visited.insert(Current);

    const auto ConditionFree = getFreeDeclRefs(Current->getTerminatorCondition(), C);
    for (const auto *FreeModified : llvm::set_intersection(Mods, ConditionFree))
      BranchMods.insert(FreeModified);

    for (const auto Succ : Current->succs()) {
      if (!Succ.isReachable()) continue;
      const auto *Next = Succ.getReachableBlock();
      if (!Next) continue;
      if (Visited.contains(Next)) continue;
      if (Next->empty()) {

      }
      auto Elem = Next->begin();
      while (Elem != Next->end() && (Elem->getKind() < CFGElement::STMT_BEGIN || Elem->getKind() > CFGElement::STMT_END)) {
        Elem++;
      }
      if (Elem == Next->end()) {

      }
    }

  } while (!Pending.empty());

  return BranchMods;
}

// bool LoopState::isInvariant(ProgramStateRef State, const Stmt *S, CheckerContext &C) const {
//   // we weaken the state at the end of an iteration by weaking the loop condition
//   // FIXME this assumes the condition before the loop itself is weak
//   // which may not be the case, it could be constrained to true way before the loop evaluation
//   // we need a proper way to weaken an SVal
//   // perhaps by conjuring?
//   auto ConditionWeakenedState = State->BindExpr(S, C.getLocationContext(), LoopCondition);
//   // return State->getConstraintManager().haveEqualConstraints(State, Node->getState());

//   return areStatesEqual(Node->getState(), ConditionWeakenedState, C);
// }

// ProgramStateRef LoopState::push(ProgramStateRef State, const ExplodedNode *N, SVal LoopCondition) {
ProgramStateRef LoopState::push(ProgramStateRef State, const Stmt *Loop) {
  // auto &F = State->get_context<LoopConstraintSet>();
  // return State->set<LoopConstraintMap>(N, F.getEmptySet())->add<LoopStack>(N);
  return State->add<LoopStack>(LoopState { Loop });
}

ProgramStateRef LoopState::pop(ProgramStateRef State) {
  const auto LS = State->get<LoopStack>();
  if (LS.isEmpty()) return State;

  return State->set<LoopStack>(LS.getTail());
}

std::optional<LoopState> LoopState::top(ProgramStateRef State) {
  const auto LS = State->get<LoopStack>();
  if (LS.isEmpty()) return std::nullopt;

  return LS.getHead();
  // return LoopState { LS.getHead(), *State->get<LoopConstraintMap>(LS.getHead()) };
}

namespace dno {

ProgramStateRef createConstraint(ProgramStateRef State, NullableObjectStateKey K, NullConstraint C) {
  return State->set<DNOConstraintMap>(K, C);
}

ProgramStateRef setAliasInternal(ProgramStateRef State, NullableObjectStateKey LK, NullableObjectStateKey RK, bool Break) {
  auto Aliases = State->get<DNOAliasMap>(LK);
  if (!Aliases && Break) return State;

  DNOAliasSet::Factory &F = State->get_context<DNOAliasSet>();

  if (!Aliases && !Break) {
    State = State->set<DNOAliasMap>(LK, F.add(F.getEmptySet(), RK));
    Aliases = State->get<DNOAliasMap>(LK);
    assert(Aliases && "alias map for key not existing after creation");
  }
  llvm::SmallVector<NullableObjectStateKey, 6> Pending { Aliases->begin(), Aliases->end() };
  llvm::SmallSet<NullableObjectStateKey, 6> Visited;

  if (Break && !Aliases)
    State = State->set<DNOAliasMap>(LK, F.remove(*Aliases, RK));

  auto I = Pending.begin();
  auto E = Pending.end();
  for (; I != E; ++I) {
    if (*I == LK) continue;
    if (Visited.contains(*I)) continue;

    auto Aliases = State->get<DNOAliasMap>(*I);
    if (Break && !Aliases) continue;

    auto UpdatedAliases = [Break, &Aliases, &LK, &F]() {
      if (Break) return F.remove(*Aliases, LK);
      return !Aliases ? F.add(F.getEmptySet(), LK) : F.add(*Aliases, LK);
    }();

    State = State->set<DNOAliasMap>(*I, UpdatedAliases);
    Visited.insert(*I);

    if (Aliases) Pending.append(Aliases->begin(), Aliases->end());
  }

  return State;
}

ProgramStateRef addAlias(ProgramStateRef State, NullableObjectStateKey LK, NullableObjectStateKey RK) {
  State = setAliasInternal(State, LK, RK, false);
  return setAliasInternal(State, RK, LK, false);
}

ProgramStateRef breakAlias(ProgramStateRef State, NullableObjectStateKey LK, NullableObjectStateKey RK) {
  DUMP(llvm::outs() << "breaking alias bettwen ");
  DUMP(LK.dump());
  DUMP(llvm::outs() << " and ");
  DUMP(RK.dump());
  DUMP(llvm::outs() << "\n");

  State = setAliasInternal(State, LK, RK, true);
  return setAliasInternal(State, RK, LK, true);
}

ProgramStateRef breakAliases(ProgramStateRef State, NullableObjectStateKey LK) {
  const auto Aliases = State->get<DNOAliasMap>(LK);
  if (!Aliases) return State;

  for (const auto AliasKey : *Aliases) {
    State = breakAlias(State, LK, AliasKey);
  }

  return State;
}

ProgramStateRef alias(ProgramStateRef State, NullableObjectStateKey K, NullableObjectStateKey AliasK) {
  const auto AC = State->get<DNOConstraintMap>(AliasK);
  if (!AC) return State;

  State = createConstraint(State, K, *AC);

  return addAlias(State, K, AliasK);
}

ProgramStateRef alias(ProgramStateRef State, SymbolRef Sym, NullableObjectStateKey K) {
  if (!Sym || !K) return State;

  dump_line(*Sym, "creating alias from sym");
  dump_line(K, "to nullable object");

  DNOAliasSet::Factory &F = State->get_context<DNOAliasSet>();

  const auto Aliases = State->get<PtrToDNOAliasMap>(Sym);
  const auto UpdatedAliases = F.add(!Aliases ? F.getEmptySet() : *Aliases, K);

  State = State->set<PtrToDNOAliasMap>(Sym, UpdatedAliases);

  llvm::SmallVector<NullableObjectStateKey, 6> Pending { UpdatedAliases.begin(), UpdatedAliases.end() };
  llvm::SmallSet<NullableObjectStateKey, 6> Visited;

  SymAliasSet::Factory &SF = State->get_context<SymAliasSet>();

  auto I = Pending.begin();
  auto E = Pending.end();
  for (; I != E; ++I) {
    if (Visited.contains(*I)) continue;
    Visited.insert(*I);

    DUMP(llvm::outs() << "aliasing ");
    DUMP(I->dump());
    DUMP(llvm::outs() << " to: ");
    DUMP(Sym->dump());
    DUMP(llvm::outs() << "\n");

    const auto SymAliases = State->get<DNOToPtrAliasMap>(*I);
    const auto UpdatedAliases = SF.add(!SymAliases ? SF.getEmptySet() : *SymAliases, Sym);

    State = State->set<DNOToPtrAliasMap>(*I, UpdatedAliases);

    if (const auto Aliases = State->get<DNOAliasMap>(*I))
      Pending.append(Aliases->begin(), Aliases->end());
  }

  return State;
}

ProgramStateRef updateConstraint(ProgramStateRef State, llvm::ImmutableSet<NullableObjectStateKey> Aliases, NullConstraint C) {
  llvm::SmallVector<NullableObjectStateKey, 6> Pending { Aliases.begin(), Aliases.end() };
  auto I = Pending.begin();
  auto E = Pending.end();
  for (; I != E; ++I) {
    State = State->set<DNOConstraintMap>(*I, C);

    const auto Aliases = State->get<DNOAliasMap>(*I);
    if (Aliases) Pending.append(Aliases->begin(), Aliases->end());
  }

  return State;
}

ProgramStateRef updateConstraint(ProgramStateRef State, llvm::ImmutableSet<SymbolRef> Aliases, NullConstraint C) {
  State = State->set<AliasGuard>(true);
  for (const auto *Sym : Aliases) {
    dump_line(*Sym, "updating symbol alias");
    auto SymV = State->getStateManager().getSValBuilder().makeSymbolVal(Sym);
    State = State->assume(SymV, C == NullConstraint::IsNotNull);
  }
  State = State->set<AliasGuard>(false);
  return State;
}

ProgramStateRef updateConstraint(ProgramStateRef State, NullableObjectStateKey K, NullConstraint C) {
  dump_line(K, "updating constraint for nullable object");
  State = State->set<DNOConstraintMap>(K, C);

  const auto Aliases = State->get<DNOAliasMap>(K);
  if (Aliases) State = updateConstraint(State, *Aliases, C);

  const auto SymAliases = State->get<DNOToPtrAliasMap>(K);
  if (SymAliases) State = updateConstraint(State, *SymAliases, C);

  return State;
}

ProgramStateRef updateConstraint(ProgramStateRef State, SymbolRef Sym, NullConstraint C) {
  if (!Sym) return State;
  dump_line(*Sym, "updating constraint for sym");

  const auto Aliases = State->get<PtrToDNOAliasMap>(Sym);
  if (!Aliases) return State;

  llvm::SmallVector<NullableObjectStateKey, 6> Pending { Aliases->begin(), Aliases->end() };
  llvm::SmallSet<SymbolRef, 6> VisitedSyms;
  llvm::SmallSet<NullableObjectStateKey, 6> VisitedNullObjs;
  VisitedSyms.insert(Sym);

  auto I = Pending.begin();
  auto E = Pending.end();
  for (; I != E; ++I) {
    if (VisitedNullObjs.contains(*I)) continue;

    State = State->set<DNOConstraintMap>(*I, C);
    VisitedNullObjs.insert(*I);

    const auto Aliases = State->get<DNOAliasMap>(*I);
    if (Aliases) Pending.append(Aliases->begin(), Aliases->end());

    const auto SymAliases = State->get<DNOToPtrAliasMap>(*I);
    if (!SymAliases) continue;
    for (const auto *SymAlias : *SymAliases) {
      if (VisitedSyms.contains(SymAlias)) continue;
      State = State->set<AliasGuard>(true);
      State = State->assume(State->getStateManager().getSValBuilder().makeSymbolVal(SymAlias), C == NullConstraint::IsNotNull);
      State = State->set<AliasGuard>(false);
      VisitedSyms.insert(SymAlias);

      const auto Aliases = State->get<PtrToDNOAliasMap>(SymAlias);
      if (Aliases) Pending.append(Aliases->begin(), Aliases->end());
    }
  }

  return State;
}

}


// This takes into account nonloc::LazyCompoundVal
const MemRegion *getRegion(SVal V) {
  if (const auto LCV = V.getAs<nonloc::LazyCompoundVal>())
    return LCV->getRegion();

  return V.getAsRegion();
}

NullableObjectStateKey getNullableObjectStateKey(SVal V) {
  if (const auto *Region = getRegion(V))
    return { Region };

  if (auto Sym = V.getAsSymbol())
    return { Sym };

  return {};
}

PathDiagnosticPieceRef NullablePointersChecker::NullableFixItVisitor::VisitNode(const ExplodedNode *N, BugReporterContext &BRC, PathSensitiveBugReport &BR) {
  if (IsConstrained) return {};
  ProgramStateRef State = N->getState();
  auto Val = State->getSVal(PointerLoc);
  if (Val.isUnknownOrUndef()) return {};

  IsConstrained = getNullConstraint(Val.castAs<DefinedOrUnknownSVal>(), State) != NullConstraint::Unknown;

  return {};
}

void NullablePointersChecker::NullableFixItVisitor::finalizeVisitor(BugReporterContext &BRC,
                                                                    const ExplodedNode *EndPathNode,
                                                                    PathSensitiveBugReport &BR) {
  if (IsConstrained) return;
  if (DerefExpr->getStmtClass() != Stmt::DeclRefExprClass) return;

  const auto *E = llvm::dyn_cast<DeclRefExpr>(DerefExpr);
  const auto *D = E->getDecl();
  if (D->getKind() != Decl::ParmVar) return;
  const auto *Param = llvm::dyn_cast<ParmVarDecl>(D);

  auto Pos = PathDiagnosticLocation { Param, BRC.getSourceManager() };
  BR.addNote("function assumes parameter is not nullptr, change type non-nullable", Pos);
  BR.addFixItHint(FixItHint::CreateInsertion(Param->getTypeSpecStartLoc(), "gsl::not_null<"));
  BR.addFixItHint(FixItHint::CreateInsertion(Param->getTypeSpecEndLoc(), ">"));
}

const Stmt *getNextStmtForDiagnostics(const ExplodedNode *Prev) {
  for (const ExplodedNode *N = Prev->getFirstSucc(); N; N = N->getFirstSucc()) {
    if (const Stmt *S = N->getStmtForDiagnostics()) {
      dump_color(*S, "considering stmt for diagnostics");
      // Check if the statement is '?' or '&&'/'||'.  These are "merges",
      // not actual statement points.
      switch (S->getStmtClass()) {
        case Stmt::ChooseExprClass:
        case Stmt::BinaryConditionalOperatorClass:
        case Stmt::ConditionalOperatorClass:
          continue;
        case Stmt::BinaryOperatorClass: {
          const auto *BO = cast<BinaryOperator>(S);
          BinaryOperatorKind Op = BO->getOpcode();
          if (Op == BO_LAnd || Op == BO_LOr)
            continue;
          break;
        }
        default:
          break;
      }
      if (S->getBeginLoc().isInvalid()) continue;
      // We found the statement, so return it.
      return S;
    }
  }

  return nullptr;
}

PathDiagnosticPieceRef NullablePointersChecker::NullableProofVisitor::VisitNode(const ExplodedNode *N, BugReporterContext &BRC, PathSensitiveBugReport &BR) {
  if (!N) return nullptr;

  auto State = N->getState();
  if (const auto *PrevNode = N->getFirstPred()) {
    auto PrevState = PrevNode->getState();

    const auto *PLocRegion = PointerLoc.getAsRegion();

    auto CurrEscapedNullableState = State->get<EscapedNullable>(PLocRegion);
    auto PrevEscapedNullableState = PrevState->get<EscapedNullable>(PLocRegion);

    bool IsEscaped = static_cast<bool>(CurrEscapedNullableState);
    bool WasEscaped = static_cast<bool>(PrevEscapedNullableState);

    bool IsInvalidated = IsEscaped && CurrEscapedNullableState->Validity == ConstraintValidity::Invalid;
    bool WasProofEstablished = WasEscaped && PrevEscapedNullableState->Constraint == NullConstraint::IsNotNull && PrevEscapedNullableState->Validity != ConstraintValidity::Invalid;

    if (const auto *DiagStmt = N->getStmtForDiagnostics()) {
      PathDiagnosticLocation Pos(DiagStmt, BRC.getSourceManager(), N->getLocationContext());

      // detect nullable pointer escape
      if (IsEscaped && !WasEscaped) {
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "pointer escapes function scope", true);
      }

      // detect nullable-safety proof invalidation
      if (IsInvalidated && WasProofEstablished) {
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "nullable-safety proof invalidated", true);
      }
    }

  }

  // detecting nullable-safety proof on predecessor node here to
  // delay diagnostic after the "taking true/false" branch diagnostic
  auto PrevNode = N->getFirstPred();
  auto P = PrevNode->getLocation();
  if (auto BE = P.getAs<BlockEdge>()) {
    const CFGBlock *Src = BE->getSrc();
    const CFGBlock *Dst = BE->getDst();
    const Stmt *T = Src->getTerminatorStmt();
    if (!T) return nullptr;

    switch (T->getStmtClass()) {
    case Stmt::IfStmtClass: {
      bool IsFalse = *(Src->succ_begin() + 1) == Dst;
      DUMP(llvm::outs() << "isTrue? " << !IsFalse << "\n");

      auto PLoc = PointerLoc.castAs<Loc>();
      dump_line(PLoc, "pointer loc");

      auto CurrentState = PrevNode->getState();
      auto PreviousState = PrevNode->getFirstPred()->getState();

      auto PValMaybe = CurrentState->getSVal(PLoc);
      if (PValMaybe.isUndef())
        return nullptr;
      auto PVal = PValMaybe.castAs<DefinedOrUnknownSVal>();
      dump_line(PVal, "pointer val");

      auto CurrentConstraint = getNullConstraint(PVal, CurrentState);
      DUMP(llvm::outs() << "pointer null constraint: " << nullConstraintToString(CurrentConstraint) << "\n");

      auto PreviousPVal = PreviousState->getSVal(PLoc).castAs<DefinedOrUnknownSVal>();
      dump_line(PreviousPVal, "previous pointer val");
      auto PreviousConstraint = getNullConstraint(PreviousPVal, PreviousState);
      DUMP(llvm::outs() << "previous pointer null constraint: " << nullConstraintToString(PreviousConstraint) << "\n");

      if (CurrentConstraint == NullConstraint::IsNotNull && PreviousConstraint == NullConstraint::Unknown) {
        auto DiagLoc = [PrevNode,T]() {
          if (const auto *Next = getNextStmtForDiagnostics(PrevNode)) return Next->getBeginLoc();
          const auto *If = dyn_cast<IfStmt>(T);
          const auto *Then = If->getThen();
          if (Then) return Then->getBeginLoc();
          return If->getRParenLoc();
        }();

        PathDiagnosticLocation Pos(DiagLoc, BRC.getSourceManager());
        return std::make_shared<PathDiagnosticEventPiece>(Pos, "nullable-safety proof established from this point");
      }

      break;
    }
    default:
      return nullptr;
    }
  }

  return nullptr;
}

ProgramStateRef recordPointerEscapes(DefinedOrUnknownSVal V, QualType T, ProgramStateRef State) {
  while ((T->isLValueReferenceType() || T->isPointerType()) && T->getPointeeType()->isPointerType()) {
    auto L = V.castAs<Loc>();
    V = State->getSVal(L).castAs<DefinedOrUnknownSVal>();
    if (isLValLoc(L)) {
      auto Constraint = getNullConstraint(V, State);
      DUMP(llvm::outs() << "sval null constraint: " << nullConstraintToString(Constraint) << "\n");

      DUMP(llvm::outs() << "ptr escaping scope; recording constraint state\n");
      auto Validity = T->getPointeeType().isConstQualified() ? ConstraintValidity::AlwaysValid : ConstraintValidity::Valid;
      State = State->set<EscapedNullable>(L.getAsRegion(), {Constraint, Validity});
    }
    T = T->getPointeeType();
    dump_type(T, "next");
    dump_line(V, "next sval");
  }
  return State;
}

ProgramStateRef checkPointerEscapeViaCompoundVal(SVal V, ProgramStateRef State, const ASTContext &AC) {
  auto CV = V.getAs<nonloc::CompoundVal>();
  if (!CV) return State;

  DUMP(llvm::outs() << "V is compound\n");

  auto CT = CV->getType(AC);
  dump_type(CT, "compound");
  const auto* CR = CT->getAsRecordDecl();
  if (!CR) return State;
  dump_color(*CR, "compound record");

  auto FDI = CR->field_begin();
  auto FDE = CR->field_end();
  auto FVI = CV->begin();
  auto FVE = CV->end();
  for (; FDI != FDE && FVI != FVE; ++FDI, FVI = ++FVI) {
    auto FieldVal = *FVI;
    auto F = *FDI;
    dump_line(FieldVal, "field sval");
    dump_color(*F, "field");
    dump_type(F->getType(), "field");
    auto FT = F->getType();

    auto FV = FieldVal.getAs<DefinedOrUnknownSVal>();
    if (!FV) continue; //TODO! if it's undefined we should report the bug

    if (isa<nonloc::CompoundVal>(*FV)) {
      State = checkPointerEscapeViaCompoundVal(*FV, State, AC);
    }

    if (isNonNullableType(FT)) {
      if (const auto *FD = getNonNullableField(FT)) {
        if (const auto LV = FV->getAs<nonloc::LazyCompoundVal>()) {
          const auto *R = LV->getRegion();
          dump_line(*R, "non-nullable object region");
          auto NNVL = State->getLValue(FD, loc::MemRegionVal(R));
          dump_line(NNVL, "non-nullable ptr_ lval");
          // auto NNV = State->getSVal(NNVL.castAs<Loc>());
          auto NNV = State->getStateManager().getStoreManager().getBinding(LV->getStore(), NNVL.castAs<Loc>());
          dump_line(NNV, "non-nullable ptr_ sval");
          FV = NNV.getAs<DefinedOrUnknownSVal>();
          FT = FD->getType();
        }
      }
    }

    // if (!isLValLoc(*FV)) continue; // we're only interested in nullable LVal's

    State = recordPointerEscapes(*FV, FT, State);

  }

  return State;
}

std::optional<NullableKind> getNullabilityState(ProgramStateRef State, DefinedOrUnknownSVal Val) {
  // if (!isLValLoc(Val)) return {};
  if (!isa<Loc>(Val)) return {};
  const auto *M = State->get<NullabilityMap>(Val.castAs<Loc>().getAsRegion());
  if (!M) return {};
  return *M;
}

}

void NullablePointersChecker::checkPointer(SVal V, const Stmt *S, ProgramStateRef State, CheckerContext &C) const {
  DUMP(llvm::outs() << "checking pointer safety\n");
  dump_line(V, "sval");

  if (V.isUndef()) {
    reportBug(BugUndefinedNullable, State, S, C);
    return;
  }

  // auto Val = [&V, &State]() {
  //   if (isLValLoc(V.castAs<DefinedOrUnknownSVal>())) return State->getSVal(V.castAs<Loc>());
  //   return V;
  // }();
  auto Val = V;
  dump_line(Val, "pointer sval");

  if (Val.isUndef()) {
    reportBug(BugUndefinedNullable, State, S, C);
    return;
  }

  auto PV = Val.castAs<DefinedOrUnknownSVal>();
  auto PVT = PV.getType(C.getASTContext());
  dump_type(PVT, "pointer sval");

  if (const auto *Cast = dyn_cast<ImplicitCastExpr>(S)) {
    if (Cast->getCastKind() == CK_ArrayToPointerDecay) {
      DUMP(llvm::outs() << "array to poitner decay yields not_null\n");
      return;
    }
  }

  const auto NullabilityState = [S, State, &C]() -> std::optional<NullableKind> {
    const auto *DerefExpr = bugreporter::getDerefExpr(S);
    if (!DerefExpr) return std::nullopt;
    dump_color(*DerefExpr, "dereference expr");

    auto L = State->getSVal(DerefExpr, C.getLocationContext()).getAs<DefinedOrUnknownSVal>();
    if (!L) return std::nullopt;
    dump_line(*L, "dereference expr L");

    return getNullabilityState(State, *L);
  }();

  if (NullabilityState)
        DUMP(llvm::outs() << "nullability for pointer: " << nullabilityToString(*NullabilityState) << "\n");
  const auto Constraint = getNullConstraint(PV, State);
  DUMP(llvm::outs() << "null constraint for sub-expr: " << nullConstraintToString(Constraint) << "\n");
  if (NullabilityState && *NullabilityState == NullableKind::VolatileNullable) {
    reportBug(BugNullablePointerDereference, State, S, C);
    return;
  }
  if (NullabilityState && *NullabilityState == NullableKind::NonNullable) {
    DUMP(llvm::outs() << "safe pointer dereference\n");
    return;
  }
  if (Constraint == NullConstraint::IsNotNull) {
    DUMP(llvm::outs() << "safe pointer dereference\n");
    return;
  }
  if (Constraint == NullConstraint::IsNull) {
    reportBug(BugNullalbeNullptrDeference, State, S, C);
    return;
  }
  reportBug(BugNullablePointerDereference, State, S, C);
}

void
NullablePointersChecker::AddDerefSource(raw_ostream &os,
                                   SmallVectorImpl<SourceRange> &Ranges,
                                   const Expr *Ex,
                                   const ProgramState *state,
                                   const LocationContext *LCtx,
                                   bool loadedFrom) {
  Ex = Ex->IgnoreParenLValueCasts();
  switch (Ex->getStmtClass()) {
    default:
      break;
    case Stmt::DeclRefExprClass: {
      const DeclRefExpr *DR = cast<DeclRefExpr>(Ex);
      if (const VarDecl *VD = dyn_cast<VarDecl>(DR->getDecl())) {
        os << " (" << (loadedFrom ? "loaded from" : "from")
           << " variable '" <<  VD->getName() << "')";
        Ranges.push_back(DR->getSourceRange());
      }
      break;
    }
    case Stmt::MemberExprClass: {
      const MemberExpr *ME = cast<MemberExpr>(Ex);
      os << " (" << (loadedFrom ? "loaded from" : "via")
         << " field '" << ME->getMemberNameInfo() << "')";
      SourceLocation L = ME->getMemberLoc();
      Ranges.push_back(SourceRange(L, L));
      break;
    }
  }
}

void NullablePointersChecker::reportNonNullableFromNullable(ProgramStateRef State, const CallEvent &Call, CheckerContext &C) const {
  using namespace clang::ast_matchers;

  ExplodedNode *N = C.generateErrorNode(State);
  if (!N) return;

  const auto Range = [&Call, &C]() {
    const auto *CallExpr = Call.getOriginExpr();
    if (!CallExpr) return Call.getSourceRange();

    const auto VarMatcher = expr(hasAncestor(varDecl(hasInitializer(ignoringImplicit(expr(equalsNode(CallExpr))))).bind("not_null_var")));
    const auto *Var = selectFirst<VarDecl>("not_null_var", match(VarMatcher, *CallExpr, C.getASTContext()));

    return Var ? Var->getSourceRange() : Call.getSourceRange();
  }();

  auto Report = std::make_unique<PathSensitiveBugReport>(BugNullableToNonNullable, BugNullableToNonNullable.getDescription(), N);
  Report->addRange(Range);

  auto NullableArg = Call.getArgSVal(0);
  dump_line(NullableArg, "nullable arg sval");
  const auto *R = NullableArg.getAsRegion();
  const auto *DerefExpr = bugreporter::getDerefExpr(Call.getArgExpr(0));
  dump_color(*DerefExpr, "dereference expr");

  Report->markInteresting(R);
  Report->addVisitor<NullableSourceVisitor>(DerefExpr);

  C.emitReport(std::move(Report));
}

void NullablePointersChecker::reportBug(const BugType &BT, ProgramStateRef State, const Stmt *S, CheckerContext &C) const {
  StringRef DerefStr = " results in a nullable pointer dereference";

  ExplodedNode *N = C.generateErrorNode(State);
  if (!N) {
    DUMP(llvm::outs() << "unable to make error node\n");
    return;
  }

  SmallString<100> buf;
  llvm::raw_svector_ostream os(buf);

  SmallVector<SourceRange, 2> Ranges;

  switch (S->getStmtClass()) {
  case Stmt::ArraySubscriptExprClass: {
    os << "Array access";
    const ArraySubscriptExpr *AE = cast<ArraySubscriptExpr>(S);
    AddDerefSource(os, Ranges, AE->getBase()->IgnoreParenCasts(),
                   State.get(), N->getLocationContext());
    os << DerefStr;
    break;
  }
  case Stmt::UnaryOperatorClass: {
    os << BT.getDescription();
    const UnaryOperator *U = cast<UnaryOperator>(S);
    AddDerefSource(os, Ranges, U->getSubExpr()->IgnoreParens(),
                   State.get(), N->getLocationContext(), true);
    break;
  }
  case Stmt::MemberExprClass: {
    const MemberExpr *M = cast<MemberExpr>(S);
    if (M->isArrow() || isDeclRefExprToReference(M->getBase())) {
      os << "Access to field '" << M->getMemberNameInfo() << "'" << DerefStr;
      AddDerefSource(os, Ranges, M->getBase()->IgnoreParenCasts(),
                     State.get(), N->getLocationContext(), true);
    }
    break;
  }
  default:
    break;
  }

  auto report = std::make_unique<PathSensitiveBugReport>(
      BT, buf.empty() ? BT.getDescription() : buf.str(), N);

  if (const auto *DerefExpr = bugreporter::getDerefExpr(S)) {
    dump_color(*DerefExpr, "dereferencing expr");
    auto L = State->getSVal(DerefExpr, C.getLocationContext());
    dump_line(L, "deref L");
    report->markInteresting(L);
    if (isa<Loc>(L)) // TODO! Improve obtaining a Loc form DerefExpr/S
      report->addVisitor<NullableProofVisitor>(L);
    // if (isa<Loc>(L))
    //   report->addVisitor<NullableFixItVisitor>(L.castAs<Loc>(), DerefExpr);
    report->addVisitor<NullableSourceVisitor>(DerefExpr);
    bugreporter::trackExpressionValue(N, DerefExpr, *report);
  }

  for (SmallVectorImpl<SourceRange>::iterator
       I = Ranges.begin(), E = Ranges.end(); I!=E; ++I)
    report->addRange(*I);

  DUMP(llvm::outs() << "emmiting\n");

  C.emitReport(std::move(report));
}

void NullablePointersChecker::checkLocation(SVal L, bool IsLoad, const Stmt *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkLocation\n");
  dump_color(*S, "Stmt");
  dump_line(L, "L");

  ProgramStateRef State = C.getState();

  if (!IsLoad) {
    DUMP(llvm::outs() << "Not a load\n");

    // auto SV = State->getSVal(S, C.getLocationContext());
    // dump_line(SV, "sval for stmt");

    if (State->contains<WeakenMap>(S)) {
      DUMP(llvm::outs() << "store on loc marked for weakening; discarding weakening mark\n");

      State = State->remove<WeakenMap>(S);

      if (auto Syns = State->get<WeakenSynonyms>(S))
        for (const auto *WS : *Syns)
          State = State->remove<WeakenMap>(WS);
      State = State->remove<WeakenSynonyms>(S);

      C.addTransition(State);
    }

    CHECK_RETURN;
  }


  auto LType = L.getType(C.getASTContext());
  dump_type(LType, "L");

  if (L.isUndef()) {
    CHECK_RETURN_LOG("undefined L");
    return;
  }

  const auto *LValExpr = S;
  if (const auto *Cast = dyn_cast<ImplicitCastExpr>(S)) {
    if (Cast->getCastKind() == CK_LValueToRValue)
      LValExpr = Cast->getSubExpr();
  }

  bool IsValWeakened = false;
  if (const auto *W = State->get<WeakenMap>(LValExpr)) {
    IsValWeakened = true;
    DUMP(llvm::outs() << "weakening due to loop modification\n");
    DUMP(llvm::outs() << "weakening kind: " << *W << "\n");

    auto V = State->getSVal(L.castAs<Loc>());
    dump_line(V, "original sval for loc");

    State = State->invalidateRegions({ L }, cast<Expr>(S), C.blockCount(), C.getLocationContext(), true);
    // State = State->killBinding(LLoc);
    // auto NewVal = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), V.getType(C.getASTContext()), C.blockCount());
    // dump_line(NewVal, "replacement rval");
    // State = State->bindLoc(LLoc, NewVal, C.getLocationContext());
    // V = NewVal;

    State = State->remove<WeakenMap>(LValExpr);

    if (auto Syns = State->get<WeakenSynonyms>(LValExpr))
      for (const auto *WS : *Syns)
        State = State->remove<WeakenMap>(WS);
    State = State->remove<WeakenSynonyms>(LValExpr);

    if (*W == Weakening::Invariant) {
      auto WV = State->getSVal(L.castAs<Loc>()).castAs<DefinedOrUnknownSVal>();
      dump_line(WV, "weakened sval for loc");

      switch (getNullConstraint(V, State)) {
      case NullConstraint::Unknown:
        // do nothing; weakend sval remain invariant
        break;
      case NullConstraint::IsNull:
        DUMP(llvm::outs() << "invariant weakening to: null\n");
        State = State->assume(WV, false);
        break;
      case NullConstraint::IsNotNull:
        DUMP(llvm::outs() << "invariant weakening to: not-null\n");
        State = State->assume(WV, true);
      }
    }
  }

  if (LType.isNull()) {
    CHECK_RETURN_LOG("L has no typedef");
  }

  auto LLoc = L.castAs<Loc>();
  // if (LType->isLValueReferenceType()) {
  //   auto PointeeVal = State->getSVal(LLoc);
  //   dump_line(PointeeVal, "pointee sval");
  // }

  const auto *GLValRegion = getGLValLocRegion(LLoc);
  if (GLValRegion) DUMP(llvm::outs() << "is glval region\n");
  const auto *Region = GLValRegion ? GLValRegion : LLoc.getAsRegion();
  if (Region)
    dump_line(*Region, "loc region");

  auto LNullability = State->get<NullabilityMap>(Region);
  if (LNullability) {
    DUMP(llvm::outs() << "L nullability: " << nullabilityToString(*LNullability) << "\n");
  } else {
    DUMP(llvm::outs() << "L has no nullability state\n");
  }

  // const bool IsRValExpr = [S]() {
  //   if (const auto *E = dyn_cast<Expr>(S))
  //     return E->isPRValue() || E->isXValue();
  //   return false;
  // }();

  auto MaybeV = State->getSVal(LLoc).getAs<DefinedOrUnknownSVal>();
  // auto MaybeV = State->getSVal(Region).getAs<DefinedOrUnknownSVal>();
  // when checkLocation runs on an rvalue of a pointer (e.g. a DeclRefExr directly without an lvalue to rvalue cast)
  // then the location SVal L does yields an undefined value.
  if (!MaybeV) {
    // if (IsRValExpr && Region && Region->getAs<TypedValueRegion>() && isNakedRawPointerType(Region->getAs<TypedValueRegion>()->getValueType()))
    if (Region && Region->getAs<TypedValueRegion>() && isNakedRawPointerType(Region->getAs<TypedValueRegion>()->getValueType()))
      reportBug(LNullability && *LNullability == NullableKind::NonNullable ? BugUndefinedNonNullable : BugUndefinedNullable, State, S, C);

    // if (!IsRValExpr) {
    //   auto LV = State->getSVal(S, C.getLocationContext());
    //   dump_line(LV, "lval expr sval");
    //   auto RV = State->getSVal(LV.castAs<Loc>());
    //   dump_line(RV, "rval expr sval");

    //   if (const auto MR = L.getAs<loc::MemRegionVal>()) {
    //     if (const auto *PR = MR->getRegion()->castAs<VarRegion>()) {
    //       auto LV = State->getLValue(dyn_cast<ParmVarDecl>(PR->getDecl()), C.getLocationContext());
    //       dump_line(LV, "lval expr sval");
    //       auto RV = State->getSVal(LV);
    //       dump_line(RV, "rval expr sval");
    //     }
    //   }
    //   if (const auto *FD = C.getLocationContext()->getDecl()->getAsFunction()) {
    //     for (const auto *Param : FD->parameters()) {
    //       auto L = State->getLValue(Param, C.getLocationContext());
    //       dump_line(L, "param loc val");

    //       auto V = State->getSVal(L);
    //       dump_line(V, "param val");
    //     }
    //   }
    // }

    CHECK_RETURN_LOG("undefined lval");
  }

  auto V = *MaybeV;
  dump_line(V, "V");

  if (V.isPureUnknown() && Region) {
    if (const auto *TR = Region->getAs<TypedValueRegion>()) {
      auto ReplacementVal = C.getSValBuilder().conjureSymbolVal(nullptr, C.getLocationContext(), TR->getValueType(), C.blockCount());
      dump_line(ReplacementVal, "replacement val");
      State = State->killBinding(LLoc)->bindLoc(LLoc, ReplacementVal, C.getLocationContext());
      V = ReplacementVal;
    }

    // State = State->killBinding(LLoc);

    // if (const auto *TR = Region->getAs<TypedValueRegion>()) {
    //   if (TR->getValueType()->isMemberPointerType()) {
    //     auto ReplacementVal = C.getSValBuilder().getUnknownMemberPointer();
    //     dump_line(ReplacementVal, "replacement sym region val");

    //     State = State->killBinding(LLoc)->bindLoc(LLoc, ReplacementVal, C.getLocationContext());
    //     // State = State->BindExpr(S, C.getLocationContext(), ReplacementVal);
    //     V = ReplacementVal;
    //   } }

    // V = UnknownNullableSVal(NullConstraint::Unknown);
    // dump_line(V, "replaced V");
    // State = State->bindLoc(LLoc, V, C.getLocationContext());
  }

  auto VT = V.getType(C.getASTContext());
  dump_type(VT, "V");
  if (VT.isNull()) {
    CHECK_RETURN_LOG("V type is null");
  }


  NullConstraint RhsNullness = getNullConstraint(V, State);
  DUMP(llvm::outs() << "V null constraint: " << nullConstraintToString(RhsNullness) << "\n");

  // llvm::outs() << "is lval escaped?: " << State->contains<EscapedNullable>(V.getAsRegion()) << "\n";
  DUMP(llvm::outs() << "is lval escaped?: " << State->contains<EscapedNullable>(L.getAsRegion()) << "\n");
  // if (const auto *EscapedPtrConstraintValidity = State->get<EscapedNullable>(V.getAsRegion())) {
  if (const auto *EscapedPtrConstraintValidity = State->get<EscapedNullable>(L.getAsRegion())) {
    if (EscapedPtrConstraintValidity->Validity == ConstraintValidity::Invalid && RhsNullness != NullConstraint::Unknown) {
      DUMP(llvm::outs() << "escaped ptr constraints are invalidated\n");
      // auto RVal = State->getSVal(LLoc);
      // dump_line(RVal, "escaped ptr rval");

      State = State->killBinding(LLoc);
      auto NewVal = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), V.getType(C.getASTContext()), C.blockCount());
      dump_line(NewVal, "replacement rval");
      State = State->bindLoc(LLoc, NewVal, C.getLocationContext());
      State = State->set<EscapedNullable>(L.getAsRegion(), {NullConstraint::Unknown, ConstraintValidity::Valid});
    }
    if (EscapedPtrConstraintValidity->Validity == ConstraintValidity::AlwaysInvalid) {
      State = State->killBinding(LLoc);
      auto NewVal = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), V.getType(C.getASTContext()), C.blockCount());
      dump_line(NewVal, "replacement rval");
      State = State->bindLoc(LLoc, NewVal, C.getLocationContext());
    }
  }

  // auto NullabilityState = NullableKind::Nullable;
  // const auto *PotentialNullabilityState = State->get<NullabilityMap>(V.getAsLocSymbol());
  // if (PotentialNullabilityState) {
  //   NullabilityState = *PotentialNullabilityState;
  // } else {
  //   llvm::outs() << "no nullability state for a V region; defaulting to Nullable\n";
  // }
  // llvm::outs() << "V nullability: " << nullabilityToString(NullabilityState) << "\n";

  // if (NullabilityState == NullableKind::NonNullable && RhsNullness == NullConstraint::IsNull) {
  //   llvm::outs() << "impossible non-nullable with null constraint detected\n";
  // }

  if (isNonNullableGLValLoc(LLoc)) {
    DUMP(llvm::outs() << "L is for not_null::ptr_\n");
    // model not_null<T*>::ptr_ access
    // marking the LVal as NonNullable and adding constraint to the RVal
    if (LNullability && *LNullability != NullableKind::NonNullable) {
      DUMP(llvm::outs() << "setting nullablilty to NonNullable\n");
      State = State->set<NullabilityMap>(Region, NullableKind::NonNullable);
    }

    if (RhsNullness != NullConstraint::IsNotNull) {
      DUMP(llvm::outs() << "setting null constraint to not-null\n");
      auto &SVB = C.getSValBuilder();
      auto Zero = SVB.makeZeroVal(L.getType(C.getASTContext())->getPointeeType());
      if (V.isPureUnknown()) {
        auto T = L.getType(C.getASTContext())->getPointeeType();
        if (T->isMemberPointerType()) {
          auto MPT = T->getAs<MemberPointerType>();
          auto *R = MPT->getMostRecentCXXRecordDecl();
          for (const auto *F : R->fields()) {
            if (F->getType() == MPT->getPointeeType()) {
              DUMP(llvm::outs() << "found field for member pointer\n");
              auto VV = UnknownNullableSVal(NullConstraint::IsNotNull);
              dump_line(VV, "replacement pointer to member sval");
              State = State->killBinding(L.castAs<Loc>())->bindLoc(L, VV, C.getLocationContext());
              V = VV;
              break;
            }
          }
        }
      }
      auto Constraint = SVB.evalBinOp(State, BO_NE, V, Zero, C.getASTContext().BoolTy).castAs<DefinedOrUnknownSVal>();
      DUMP(llvm::outs() << "V is a loc? " << isa<Loc>(V) << "\n");
      DUMP(llvm::outs() << "Zero is a loc? " << isa<Loc>(Zero) << "\n");
      dump_line(Constraint, "constraint");
      State = State->assume(Constraint, true);
    }
  } else if (LNullability && *LNullability == NullableKind::VolatileNullable) {
    auto NewVal = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), VT, C.blockCount());
    dump_line(NewVal, "new volatile val");
    State = State->killBinding(LLoc)->bindLoc(LLoc, NewVal, C.getLocationContext());
  } else if (LType->isLValueReferenceType() && LType->getPointeeType().isVolatileQualified()) {
    auto Nullability = NullableKind::VolatileNullable;
    DUMP(llvm::outs() << "setting L nullablilty to " << nullabilityToString(Nullability) << "\n");
    State = State->set<NullabilityMap>(Region, Nullability);
    State = State->killBinding(LLoc);
    // member pointers are Unknown SVals and we do not have a type for these
    if (!V.isPureUnknown()) {
      auto NewVal = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), VT, C.blockCount());
      dump_line(NewVal, "new volatile val");
      State = State->bindLoc(LLoc, NewVal, C.getLocationContext());
      V = NewVal;
    } else {
      V = UnknownNullableSVal(NullConstraint::Unknown);
      State = State->bindLoc(LLoc, V, C.getLocationContext());
    }
  // TODO! This condition does NOT include member pointers
  } else if (LType->getPointeeType().isNull()) {
    CHECK_RETURN_LOG("L has no pointee type");
  } else if (LType->getPointeeType()->isPointerType() && LType->getPointeeType()->getPointeeType().isVolatileQualified()) {
    auto Nullability = NullableKind::VolatileNullable;
    DUMP(llvm::outs() << "setting V nullablilty to " << nullabilityToString(Nullability) << "\n");
    const auto *VRegion = V.getAsRegion();
    if (!VRegion) {
      DUMP(llvm::outs() << "no region for V\n");
      C.addTransition(State);
      CHECK_RETURN;
      return;
    }
    State = State->set<NullabilityMap>(VRegion, Nullability);

    auto VLoc = V.castAs<Loc>();
    auto VV = State->getSVal(VLoc);
    dump_line(VV, "V deref sval");

    auto NewVal = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), VT->getPointeeType(), C.blockCount());
    dump_line(NewVal, "new volatile val");

    State = State->killBinding(VLoc)->bindLoc(VLoc, NewVal, C.getLocationContext());
  } else if (isGLValLoc(LLoc) && !LNullability) {
    // auto Nullability = [&V,&C]() {
    //   auto T = V.getType(C.getASTContext());
    //   if (T.isVolatileQualified() && T->isLValueReferenceType()) return NullableKind::VolatileNullable;
    //   return NullableKind::Nullable;
    // }();
    auto Nullability = NullableKind::Nullable;
    DUMP(llvm::outs() << "setting nullablilty to " << nullabilityToString(Nullability) << "\n");
    State = State->set<NullabilityMap>(Region, Nullability);
  } //TODO! check for contradtiction of being NonNullable and !isNonNullableGLValLoc

  if (isGlobalRegion(Region) && !VT.getCanonicalType().isConstQualified() && !IsValWeakened) {
    DUMP(llvm::outs() << "recording non-local pointer\n");
    State = State->add<NonLocalNullable>(Region);
  }

  C.addTransition(State);

  CHECK_RETURN;
}


void NullablePointersChecker::checkBind(SVal L, SVal V, const Stmt *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkBind\n");
  dump_color(*S, "stmt");
  dump_line(L, "L");
  dump_line(V, "V");

  const auto *LRegion = L.getAsRegion();
  if (!LRegion)
    return;

  ProgramStateRef State = C.getState();

  auto LType = L.getType(C.getASTContext());
  dump_type(LType, "L");

  auto VType = V.getType(C.getASTContext());
  dump_type(VType, "V");

  // loc rank > 1 so that it's a ref to pointer or pointer to pointer
  if (isGlobalRegion(LRegion) && getLocTypeRank(LType) > 1) {
    dump_line(*LRegion, "is a global region");

    auto VDeref = State->getSVal(V.castAs<Loc>());
    auto Constraint = getNullConstraint(VDeref.castAs<DefinedOrUnknownSVal>(), State);
    if (Constraint != NullConstraint::IsNotNull) {
      DUMP(llvm::outs() << "invalidating all non-local nullable");
      for (const auto* NLR : State->get<NonLocalNullable>()) {
        if (!NLR) continue;
        dump_line(*NLR, "invalidating non-local");
        State = State->set<EscapedNullable>(NLR, { NullConstraint::Unknown, ConstraintValidity::Invalid });
      }
    }
  }

  State = checkPointerEscapeViaCompoundVal(V, State, C.getASTContext());

  dump_line(*LRegion, "L region");

  if (isa<Loc>(L)) {
    if (auto State_ = checkPointerAddressEscapes(State, L.castAs<Loc>(), V, C.getASTContext())) {
      State = State_;
    } else {
      reportBug(BugVolatilePointerEscape, State, S, C);
      CHECK_RETURN;
    }
  }

  // const auto *VR = V.getAsRegion();
  // if (const auto PAE = getPointerAddressEscapeKind(LRegion, VR)) {
  //   auto EV = VR->getKind() == MemRegion::SymbolicRegionKind ? V : State->getSVal(VR);
  //   dump_line(EV, "escaped ptr sval");
  //   auto Constraint = getNullConstraint(EV.castAs<DefinedOrUnknownSVal>(), State);
  //   llvm::outs() << "escape pointer null constraint: " << nullConstraintToString(Constraint) << "\n";
  //   State = State->set<EscapedNullable>(EV.getAsRegion(), EscapedNullableState { Constraint, *PAE == PointerAddressEscapeKind::NonVolatile ? ConstraintValidity::Unknown : ConstraintValidity::Invalidated });
  // }

  const bool IsLRefPP = LType->isLValueReferenceType() && !LType->getPointeeType().isConstQualified() && LType->getPointeeType()->isPointerType() && LType->getPointeeType()->getPointeeType()->isPointerType();
  if (State->contains<NonLocalNullable>(LRegion) && IsLRefPP) {
    auto VDeref = State->getSVal(V.castAs<Loc>());
    dump_line(VDeref, "esacpe pointer val");

    auto Constraint = getNullConstraint(VDeref.castAs<DefinedOrUnknownSVal>(), State);
    DUMP(llvm::outs() << "escape pointer null constraint: " << nullConstraintToString(Constraint) << "\n");

    State = State->set<EscapedNullable>(V.getAsRegion(), EscapedNullableState { Constraint, ConstraintValidity::Valid });
    C.addTransition(State);
    CHECK_RETURN;
  }

  if (isa<Loc>(L) && isNullableObjectMemberFieldLoc(L.castAs<Loc>())) {
    const auto R = L.castAs<loc::MemRegionVal>().getRegionAs<FieldRegion>()->getBaseRegion()->getAs<TypedValueRegion>();
    if (R) {
      dump_line(*R, "nullable object field base region");
      auto RV = State->getSVal(R);
      dump_line(RV, "nullable object sval");

      const auto LHSKey = getNullableObjectStateKey(RV);
      assert(LHSKey && "lhs must have a nullable object state key");
      auto LHState = State->get<DNOConstraintMap>(LHSKey);
      if (LHState) {
        DUMP(llvm::outs() << "breaking nulable object aliases\n");
        State = dno::breakAliases(State, LHSKey);
      }
      State = dno::createConstraint(State, LHSKey, NullConstraint::Unknown);
      C.addTransition(State);
      CHECK_RETURN;
    }
  }

  QualType LocType = LType->getPointeeType();
  if (!LocType->isPointerType() && !LocType->isMemberPointerType()) {
    if (const auto *Record = getCXXRecordFromType(LocType)) {
      if (isNullableObject(Record)) {
        DUMP(llvm::outs() << "a nullable object\n");

        const auto LHSKey = getNullableObjectStateKey(L);
        assert(LHSKey && "lhs should be loc that has a region to use as a nullable object state key");

        if (const auto RHSKey = getNullableObjectStateKey(V)) {
          auto RHState = State->get<DNOConstraintMap>(RHSKey);
          if (!RHState) {
            DUMP(llvm::outs() << "no state recorded for rhs nullable object\n");
            State = dno::createConstraint(State, RHSKey, NullConstraint::Unknown);
          }
          DUMP(llvm::outs() << "aliasing lhs and rhs\n");
          State = dno::breakAliases(State, LHSKey);
          State = dno::alias(State, LHSKey, RHSKey);
        }
      }
    }

    C.addTransition(State);
    CHECK_RETURN_LOG("not a pointer");
  }

  // auto LNullability = State->get<NullabilityMap>(L.getAsLocSymbol());
  auto LNullability = State->get<NullabilityMap>(LRegion);
  if (LNullability) {
    DUMP(llvm::outs() << "L nullability: " << nullabilityToString(*LNullability) << "\n");
  } else {
    DUMP(llvm::outs() << "L has no nullability state\n");
  }

  auto ValDefOrUnknown = V.getAs<DefinedOrUnknownSVal>();
  if (!ValDefOrUnknown) {
    C.addTransition(State);
    CHECK_RETURN_LOG("not a good SVal");
  }
  dump_line(*ValDefOrUnknown, "rhs sval");
  dump_type(ValDefOrUnknown->getType(C.getASTContext()), "rhs sval");
  NullConstraint RhsNullness = getNullConstraint(*ValDefOrUnknown, State);
  DUMP(llvm::outs() << "rhs null constraint: " << nullConstraintToString(RhsNullness) << "\n");

  // auto RhsNullability = [S, State, &C]() -> std::optional<NullableKind> {
  //   const auto *AssignDeclStmt = dyn_cast<DeclStmt>(S);
  //   if (!AssignDeclStmt) return {};
  //   const auto *AssignVar = AssignDeclStmt->getSingleDecl();
  //   if (!AssignVar) return {};
  //   const auto *AssignVarDecl = dyn_cast<VarDecl>(AssignVar);
  //   if (!AssignVarDecl) return {};

  //   // const auto *DerefExpr = bugreporter::getDerefExpr(AssignVarDecl->getInit());
  //   const auto *DerefExpr = AssignVarDecl->getInit();
  //   if (!DerefExpr) return {};
  //   dump_color(*DerefExpr, "rhs dereference expr");

  //   auto RhsLoc = State->getSVal(DerefExpr, C.getLocationContext()).getAs<DefinedOrUnknownSVal>();
  //   if (!RhsLoc) return {};
  //   dump_line(*RhsLoc, "rhs L");

  //   return getNullabilityState(State, *RhsLoc);
  // }();
  // if (RhsNullability)
  //   llvm::outs() << "rhs nullability state: " << nullabilityToString(*RhsNullability) << "\n";

  auto RegionSVal = ValDefOrUnknown->getAs<loc::MemRegionVal>();
  if (!RegionSVal) {
    C.addTransition(State);
    CHECK_RETURN_LOG("no region sval");
  }

  const MemRegion *Region = ValDefOrUnknown->getAsRegion();
  dump_line(*Region, "rhs region");

  auto NullabilityState = NullableKind::Nullable;
  if (LNullability) {
    switch (*LNullability) {
    case NullableKind::NonNullable:
      if (RhsNullness == NullConstraint::IsNotNull) break;

      if (ValDefOrUnknown->isZeroConstant())
        reportBug(BugNullptrToNonNullable, State, S, C);
      else
        reportBug(BugNullableToNonNullable, State, S, C);

      NullabilityState = NullableKind::NonNullable;
      break;
    case NullableKind::Nullable:
    case NullableKind::VolatileNullable:
      break;
    }
  } else if (!LNullability && isRegionNotNullPtrField(LRegion)) {
    // we are binding to not_null::ptr
    NullabilityState = NullableKind::NonNullable;
  }
  DUMP(llvm::outs() << "setting nullability state for L: " << nullabilityToString(NullabilityState) << "\n");

  // State = State->set<NullabilityMap>(L.getAsSymbol(), NullabilityState);
  State = State->set<NullabilityMap>(LRegion, NullabilityState);
  C.addTransition(State);

  CHECK_RETURN;
}

void NullablePointersChecker::checkPostCall(const CallEvent &Call, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostCall\n");

  const auto *E = Call.getOriginExpr();
  if (!E) {
    CHECK_RETURN_LOG("no call expr");
  }
  dump_color(*E, "call expr");
  dump_type(Call.getResultType(), "return");

  ProgramStateRef State = C.getState();
  auto &SVB = C.getSValBuilder();

  auto ReturnSVal = Call.getReturnValue();
  dump_line(ReturnSVal, "return sval");

  if (const auto *New = dyn_cast<CXXNewExpr>(E)) {
    if (const auto *NewOp = New->getOperatorNew()) {
      auto ExcSpec = NewOp->getExceptionSpecType();
      if (ExcSpec != EST_BasicNoexcept && ExcSpec != EST_NoexceptTrue) {
        auto Zero = SVB.makeZeroVal(Call.getResultType());
        auto Constraint = SVB.evalBinOp(State, BO_EQ, ReturnSVal, Zero, C.getASTContext().BoolTy);
        State = State->assume(Constraint.castAs<DefinedOrUnknownSVal>(), false);
        DUMP(llvm::outs() << "assumming noexcept new returns non null\n");
      }
    }
  }

  if (ReturnSVal.isUnknown()) {
    // This seems to happen on CXXConstructExprs that are not evalCalled or checkBinded by the ExprEngine
    // seems to happen when the CXXRecord has no member fields
    // e.g. when creating copies for call arguments, NoOp casts

    ReturnSVal = C.getSValBuilder().conjureSymbolVal(E, C.getLocationContext(), Call.getResultType(), C.blockCount());
    dump_line(ReturnSVal, "recovered return sval");
    if (!ReturnSVal.isUnknown()) {
      State = State->BindExpr(E, C.getLocationContext(), ReturnSVal);
      if (const auto *CC = dyn_cast<CXXConstructExpr>(E)) {
        if (const auto *Ctor = CC->getConstructor()) {
          if (isNullableObject(Ctor->getParent())) {
            if (Ctor->isCopyConstructor()) {
              auto OrigVal = State->getSVal(CC->getArg(0), C.getLocationContext());
              dump_line(OrigVal, "nullable object origin sval");

              auto CopyKey = getNullableObjectStateKey(ReturnSVal);
              auto OrigKey = getNullableObjectStateKey(OrigVal);
              assert(CopyKey && OrigKey && "both keys must be obtainable");

              auto OrigConstraint = State->get<DNOConstraintMap>(OrigKey);
              if (!OrigConstraint) {
                DUMP(llvm::outs() << "nullable object origin sval: not-recorded; creating unknown\n");
                State = dno::createConstraint(State, OrigKey, NullConstraint::Unknown);
              }

              DUMP(llvm::outs() << "creating aliased constraint to origin sval\n");
              State = dno::alias(State, CopyKey, OrigKey);

              // if (const auto CopyKey = getNullableObjectStateKey(ReturnSVal)) {
              //   llvm::outs() << "aliasing return sval with origin\n";
              //   // State = State->set<NullableObjectState>(CopyKey, { NullConstraint::Unknown, { {OrigVal.getAsRegion()} } });
              //   // State = NullableObjectConstraint::create(State, CopyKey, NullConstraint::Unknown, {{ getRegion(OrigVal) }});
              // }

              // if (const auto OrigKey = getNullableObjectStateKey(OrigVal)) {
              //   if (auto OrigState = State->get<NullableObjectState>(OrigKey)) {
              //     if (const auto CopyKey = getNullableObjectStateKey(ReturnSVal)) {
              //       llvm::outs() << "copying nullable object state\n";
              //       State = State->set<NullableObjectState>(CopyKey, *OrigState);
              //     }
              //   } else {
              //     llvm::outs() << "creating origin nullable object state\n";
              //     // State = State->set<NullableObjectState>(OrigKey, NullConstraint::Unknown);
              //     State = NullableObjectConstraint::create(State, OrigKey, NullConstraint::Unknown);
              //   }
              // }
            }
          }
        }
      }
    }
  }

  const bool IgnoreEscapeAnalysis = ignoreEscapeAnalysis(Call);
  if (!IgnoreEscapeAnalysis) {
    const auto &NonLocals = State->get<NonLocalNullable>();
    for (const auto *R : NonLocals) {
      auto NonLocalLoc = C.getSValBuilder().makeLoc(R);
      dump_line(NonLocalLoc, "invalidating non-local loc");

      State = State->invalidateRegions({ NonLocalLoc }, E, C.blockCount(), C.getLocationContext(), true);
      // State = State->killBinding(NonLocalLoc);
      // auto NewVal = C.getSValBuilder().conjureSymbolVal(nullptr, C.getLocationContext(), NonLocalLoc.getType(C.getASTContext()), C.blockCount());
      // dump_line(NewVal, "replacement val");
      // State = State->bindLoc(NonLocalLoc, NewVal, C.getLocationContext());
    }

    auto ReturnType = Call.getResultType();
    if (ReturnType->isLValueReferenceType() && ReturnType->getPointeeType()->isPointerType()) {
      DUMP(llvm::outs() << "recording return as a non-local nullable\n");
      State = State->add<NonLocalNullable>(ReturnSVal.getAsRegion());
    }

    if (C.getStackFrame()) {
      if (const auto *D = C.getStackFrame()->getDecl()) {
        if (const auto *F = D->getAsFunction()) {
          for (const auto *Param : F->parameters()) {
            if (!Param->getType()->isLValueReferenceType()) continue;
            if (!Param->getType()->getPointeeType()->isPointerType()) continue;
            dump_color(*Param, "lval reference parameter constraint invalidation");

            auto ParamLoc = State->getLValue(Param, C.getLocationContext());
            dump_line(ParamLoc, "param lval");

            auto ParamPtrLoc = State->getSVal(ParamLoc).castAs<Loc>();
            dump_line(ParamPtrLoc, "param ptr sval");

            auto ParamVal = State->getSVal(ParamPtrLoc);
            dump_line(ParamVal, "param sval");

            State = State->killBinding(ParamPtrLoc);
            // auto NewVal = C.getSValBuilder().makeLoc(ParamVal.getAsRegion());
            auto NewVal = C.getSValBuilder().conjureSymbolVal(nullptr, C.getLocationContext(), Param->getType()->getPointeeType(), C.blockCount());
            dump_line(NewVal, "replacement param sval");
            State = State->bindLoc(ParamPtrLoc, NewVal, C.getLocationContext());
          }
        }
      }
    }
    // Any constraints we may have on escaped pointers right now
    // need to be invalidated.
    // We do not know if the callee has any writable access to it.
    const auto &Escaped = State->get<EscapedNullable>();
    for (const auto &EscapeRecord: Escaped) {
      const auto *LValRegion = EscapeRecord.first;
      auto Constraint = EscapeRecord.second.Constraint;
      auto Validity = EscapeRecord.second.Validity;

      auto Val = C.getSValBuilder().makeLoc(LValRegion);
      dump_line(Val, "escaped ptr lval");
      dump_type(getRegionValueType(LValRegion), "lval region value");

      // auto RVal = State->getSVal(Val);
      // dump_line(RVal, "escaped ptr rval");

      // auto Zero = C.getSValBuilder().makeZeroVal(getRegionValueType(LValRegion));
      // if (Validity == ConstraintValidity::AlwaysValid && Constraint == NullConstraint::IsNotNull) {
      //   llvm::outs() << "constrained to not null and constraint is valid\n";

      //   State = State->killBinding(Val);

      //   auto NewVal = C.getSValBuilder().makeLoc(Val.getAsRegion());
      //   dump_line(NewVal, "replacement rval");

      //   State = State->bindLoc(Val, NewVal, C.getLocationContext());
      //   auto Condition = C.getSValBuilder().evalBinOp(State, BO_NE, NewVal, Zero, C.getASTContext().BoolTy);
      //   State = State->assume(Condition.castAs<DefinedOrUnknownSVal>(), true);
      // }

      // if (Validity == ConstraintValidity::AlwaysValid && Constraint != NullConstraint::Unknown) {
      // if (Validity != ConstraintValidity::AlwaysValid) {
      if (Validity == ConstraintValidity::Valid && Constraint != NullConstraint::Unknown) {
        DUMP(llvm::outs() << "invalidating constraints\n");
        // State = State->killBinding(LVal);
        State = State->set<EscapedNullable>(LValRegion, {EscapeRecord.second.Constraint, ConstraintValidity::Invalid});
        // State = State->invalidateRegions({LValRegion}, Call.getOriginExpr(), C.blockCount(), C.getLocationContext(), true);
      }
    }
  }

  for (unsigned int I = 0; I < Call.getNumArgs(); ++I) {
    const auto *ArgExpr = Call.getArgExpr(I);
    dump_color(*ArgExpr, "arg expr");
    if (!ArgExpr) continue;
    DUMP(llvm::outs() << "arg is lval: " << ArgExpr->isLValue() << "\n");

    // TODO! This is not exactly right, we should check the SVal's MemRegion structure for this
    if (!ArgExpr->isLValue()) continue;

    auto LVal = Call.getArgSVal(I).castAs<Loc>();
    dump_line(LVal, "arg lval");
    const auto *LValRegion = LVal.getAsRegion();
    if (LValRegion) dump_line(*LValRegion, "arg lval region");

    auto RVal = State->getSVal(LVal);
    dump_line(RVal, "arg rval");

    const auto *Param = Call.parameters()[I];
    dump_color(*Param, "param");

    auto ParamType = Param->getType();
    dump_type(ParamType, "param");
  }

  if (ReturnSVal.isUndef()) {
    C.addTransition(State);
    CHECK_RETURN_LOG("return sval is undefined");
  }
  DUMP((llvm::outs() << "return sval null constraint: " << nullConstraintToString(getNullConstraint(ReturnSVal.castAs<DefinedOrUnknownSVal>(), State)) << "\n"));

  // if (auto *NewExpr = dyn_cast<CXXNewExpr>(Call.getOriginExpr())) {
  //   if (!NewExpr->getOperatorNew()->isStdNamespace()) {
  //     auto ReplacementReturnSVal = C.getSValBuilder().conjureSymbolVal(Call.getOriginExpr(), C.getLocationContext(), Call.getResultType(), C.blockCount());
  //     dump_line(ReplacementReturnSVal, "conjuring new return sval");
  //     State = State->BindExpr(Call.getOriginExpr(), Call.getLocationContext(), ReplacementReturnSVal);
  //   }
  // }

  if (const auto *MC = dyn_cast<CXXMemberCallExpr>(E)) {
    if (const auto *D = MC->getCalleeDecl()) {
      if (const auto *F = D->getAsFunction()) {
        if (!F->getFunctionType()->isConst()) {
          // non const member call exprs; invalidate constraints
          const auto *Target = MC->getImplicitObjectArgument();
          if (isNullableObjectType(Target->getType())) {
            auto TargetSVal = State->getSVal(Target, C.getLocationContext());
            dump_line(TargetSVal, "nullable object sval");

            if (const auto TargetKey = getNullableObjectStateKey(TargetSVal)) {
              DUMP(llvm::outs() << "non-const member function called on nullable object\n");

              State = dno::breakAliases(State, TargetKey);
              State = dno::createConstraint(State, TargetKey, NullConstraint::Unknown);
            }
          }
        }
      }
    }
  }
  if (const auto *MC = dyn_cast<CXXOperatorCallExpr>(E)) {
    if (const auto *D = MC->getCalleeDecl()) {
      if (const auto *F = D->getAsFunction()) {
        if (!F->getFunctionType()->isConst()) {
          // non const member call exprs; invalidate constraints
          const auto *Target = MC->getArg(0);
          if (isNullableObjectType(Target->getType())) {
            auto TargetSVal = State->getSVal(Target, C.getLocationContext());
            dump_line(TargetSVal, "nullable object sval");

            if (const auto TargetKey = getNullableObjectStateKey(TargetSVal)) {
              DUMP(llvm::outs() << "non-const member function called on nullable object\n");

              State = dno::breakAliases(State, TargetKey);
              State = dno::createConstraint(State, TargetKey, NullConstraint::Unknown);
            }
          }
        }
      }
    }
  }

  if (isStdAddressOf(Call)) {
    // std::addressof will never return a nullptr
    DUMP(llvm::outs() << "modelling std::addressof\n");

    State = State->assume(ReturnSVal.castAs<DefinedOrUnknownSVal>(), true);
  }
  // if (!isInterProcedural(Call)) {
  //   llvm::outs() << "breaking inter-procedural analysis\n";
  //   //conjure a new return value
  //   auto ReplacementReturnSVal = C.getSValBuilder().conjureSymbolVal(Call.getOriginExpr(), C.getLocationContext(), Call.getResultType(), 0);
  //   dump_line(ReplacementReturnSVal, "conjuring new return sval");
  //   State = State->BindExpr(Call.getOriginExpr(), Call.getLocationContext(), ReplacementReturnSVal);
  // }

  C.addTransition(State);

  CHECK_RETURN;
}


void NullablePointersChecker::checkBeginFunction(CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkBeginFunction\n");

  const LocationContext *LocContext = C.getLocationContext();

  const Decl *D = LocContext->getDecl();
  auto FD = D->getAsFunction();
  assert(FD);
  dump_color(*FD, "function decl");

  // if (!C.inTopFrame()) {
  //   llvm::outs() << "inlined frame\n";
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return;
  // }

  if (FD->isImplicit()) {
    C.addSink();
    CHECK_RETURN_LOG("implicit; sinking analysis");
  }

  ProgramStateRef State = C.getState();

  if (C.inTopFrame()) {
    DUMP(llvm::outs() << "top frame\n");

    for (const auto *Param : FD->parameters()) {
      auto L = State->getLValue(Param, C.getLocationContext());
      dump_line(L, "param loc val");

      auto V = State->getSVal(L);
      dump_line(V, "param val");
    }

    // if (const auto *CM = dyn_cast<CXXMethodDecl>(C.getLocationContext()->getDecl())) {
    //   if (CM->getOverloadedOperator() == OO_Call) {
    //     if (CM->getParent()->isLambda()) {
    //       for (const auto& LC : CM->getParent()->captures()) {
    //         if (LC.getCaptureKind() != LCK_ByRef) continue;

    //         State = State->add<LambdaRefCaptures>(LC.getCapturedVar());
    //       }
    //     }
    //   }
    // }

    // C.addTransition(State);

    CHECK_RETURN;
  }

  if (isInterProcedural(FD)) {
    CHECK_RETURN_LOG("ignoring function; allow inter-procedural analysis");
  }

  DUMP(llvm::outs() << "breaking inter-procedural analysis\n");


  // we need to break the binding for every argument lvalue
  // except for not_null
  for (const ParmVarDecl *Param: FD->parameters()) {
    DUMP(llvm::outs() << "Param name: " << Param->getName() << "\n");

    if (isNonNullableType(Param->getType()))
      continue;

    auto LVal = State->getLValue(Param, LocContext);
    dump_line(LVal, "param lval");

    State = State->killBinding(LVal);
  }

  C.addTransition(State);

  CHECK_RETURN;

  // we do not need the following
  // we can model each not_null access at the time it occurs in checkLocation
  // and raw pointer lvals can default to Nullable when nullabiltiy is absent
  for (const ParmVarDecl *Param : FD->parameters()) {
    DUMP(llvm::outs() << "Param name: " << Param->getName() << "\n");
    const auto *Rec = Param->getType()->getAsCXXRecordDecl();

    Loc ParamLVal = State->getLValue(Param, LocContext);
    dump_line(ParamLVal, "ParamLVal");

    auto ParamSVal = State->getSVal(ParamLVal);
    dump_line(ParamSVal, "ParamSVal");

    // set nullability value for a not_null<T*>::ptr_ r-value;
    if (Rec && Rec->getQualifiedNameAsString() == NotNullPtrQualifiedName) {
      DUMP(llvm::outs() << "param is a not_null<T*>\n");

      for (const FieldDecl *FD : Rec->fields()) {
        if (FD->getName() != "ptr_") continue;

        auto NotNullPtrLVal = State->getLValue(FD, ParamLVal).getAs<Loc>();
        if (!NotNullPtrLVal) {
          DUMP(llvm::outs() << "not_null::ptr_ LVal not a Loc\n");
          continue;
        }
        dump_line(*NotNullPtrLVal, "not_null::ptr_ LVal");

        auto NotNullPtrSVal = State->getSVal(*NotNullPtrLVal).castAs<DefinedOrUnknownSVal>();
        dump_line(NotNullPtrSVal, "not_null::ptr_ SVal");

        const auto* NotNullPtrSValRegion = NotNullPtrSVal.getAsRegion();
        if (!NotNullPtrSValRegion) {
          DUMP(llvm::outs() << "not_null::ptr_ SVal has not region\n");
          continue;
        }

        // llvm::outs() << "marking not_null::ptr_ SVal as NonNullable\n";
        // State = State->assume(NotNullPtrSVal, true);
        // State = State->set<NullabilityMap>(NotNullPtrSVal.getAsLocSymbol(), NullableKind::NonNullable);

        // llvm::outs() << "marking not_null::ptr_ LVal as NonNullable\n";
        // State = State->set<NullabilityMap>(NotNullPtrLVal->getAsLocSymbol(), NullableKind::NonNullable);
      }

      continue;
    }

    if (Param->getType()->isPointerType()) {
      DUMP(llvm::outs() << "param is a raw pointer\n");

      const auto* ParamSValRegion = ParamSVal.getAsRegion();
      if (!ParamSValRegion) {
        DUMP(llvm::outs() << "param SVal has no region\n");
        continue;
      }

      // llvm::outs() << "marking raw pointer SVal as Nullable\n";
      // State = State->set<NullabilityMap>(ParamSVal.getAsLocSymbol(), NullableKind::Nullable);

      // llvm::outs() << "marking raw pointer LVal as NonNullable\n";
      // State = State->set<NullabilityMap>(ParamLVal.getAsLocSymbol(), NullableKind::Nullable);
    }
  }

  C.addTransition(State);

  CHECK_RETURN;
}

// void NullablePointersChecker::checkEndFunction(CheckerContext &C) const {
//   ProgramStateRef State = C.getState();
//   auto &F = State->get_context<LambdaRefCaptures>();
//   State = State->set<LambdaRefCaptures>(F.getEmptySet());
//   C.addTransition(State);
// }

void NullablePointersChecker::checkPreStmt(const UnaryOperator *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPreStmt<UnaryOperator>\n");
  dump_color(*S, "UnaryOperator");

  if (const auto *Ctx = dyn_cast_or_null<CXXMethodDecl>(C.getLocationContext()->getDecl())) {
    if (Ctx->getParent()->getQualifiedNameAsString() == NotNullPtrQualifiedName) {
      CHECK_RETURN_LOG("self is not_null");
    }
  }

  ProgramStateRef State = C.getState();

  if (const auto *FD = C.getLocationContext()->getDecl()->getAsFunction()) {
    for (const auto *Param : FD->parameters()) {
      auto L = State->getLValue(Param, C.getLocationContext());
      dump_line(L, "param loc val");

      auto V = State->getSVal(L);
      dump_line(V, "param val");
    }
  }

  switch (S->getOpcode()) {
    case UnaryOperatorKind::UO_Deref: {
      if (!S->getSubExpr()->getType()->isPointerType()) {
        CHECK_RETURN_LOG("not a pointer type");
      }

      auto SubExprSVal = State->getSVal(S->getSubExpr(), C.getLocationContext());
      dump_line(SubExprSVal, "sub-expr sval");

      checkPointer(SubExprSVal, S->getSubExpr(), State, C);
      // if (SubExprSVal.isUndef()) {
      //   reportBug(BugUndefinedNullable, State, S, C);
      //   break;
      // }
      // auto Val = SubExprSVal.castAs<DefinedOrUnknownSVal>();
      // auto RVal = [&Val, &State]() {
      //   if (isLValLoc(Val)) return State->getSVal(Val.castAs<Loc>()).castAs<DefinedOrUnknownSVal>();
      //   return Val;
      // }();
      // dump_type(RVal.getType(C.getASTContext()), "rval");

      // // NOTE: ideally we need the lval nullability state
      // const auto NullabilityState = getNullabilityState(State, Val);
      // if (NullabilityState)
      //   llvm::outs() << "nullability for sub-expr: " << nullabilityToString(*NullabilityState) << "\n";
      // // NOTE: ideally we need the rval null constraint
      // const auto Constraint = getNullConstraint(RVal, State);
      // llvm::outs() << "null constraint for sub-expr: " << nullConstraintToString(Constraint) << "\n";
      // if ((NullabilityState && *NullabilityState == NullableKind::NonNullable) || Constraint == NullConstraint::IsNotNull) {
      //   llvm::outs() << "safe pointer dereference\n";
      //   break;
      // }
      // if (Constraint == NullConstraint::IsNull) {
      //   reportBug(BugNullalbeNullptrDeference, State, S, C);
      //   break;
      // }
      // reportBug(BugNullablePointerDereference, State, S, C);

      break;
    }
    case UnaryOperatorKind::UO_PostInc:
    case UnaryOperatorKind::UO_PostDec:
    case UnaryOperatorKind::UO_PreInc:
    case UnaryOperatorKind::UO_PreDec: {
      if (!S->getType()->isPointerType()) {
        CHECK_RETURN_LOG("not a pointer type");
      }
      // we know the pointer is nullable here
      // non-nullable pointers cannot be incremented/decremented
      // post/pre incr/decr can only occur on lvalue expressions
      // this lvalue is undefined post-statement
      // auto LVal = State->getSVal(S->getSubExpr(), C.getLocationContext()).castAs<Loc>();
      // dump_line(LVal, "sub-expr lval");

      // // replace lvalue binding with an undefined value
      // State = State->killBinding(LVal)->bindLoc(LVal, UndefinedVal(), C.getLocationContext());
      // C.addTransition(State);

      break;
    }
    default:
      DUMP(llvm::outs() << "ignoring opcode\n");
      break;
  }

  CHECK_RETURN;
}

void NullablePointersChecker::checkPreStmt(const BinaryOperator *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPreStmt<BinaryOperator>\n");
  dump_color(*S, "BinaryOperator");

  ProgramStateRef State = C.getState();
  const auto *LCtx = C.getLocationContext();

  if (S->getOpcode() == BO_Assign) {
    const auto *LHS = S->getLHS();
    if (const auto *LUO = dyn_cast<UnaryOperator>(LHS)) {
      if (LUO->getOpcode() == UO_Deref) {
        DUMP(llvm::outs() << "Assignment LHS is a dereference\n");

        const auto V = State->getSVal(LHS, LCtx);
        dump_line(V, "lhs sval");

        if (isa<Loc>(V)) {
          const auto L = V.castAs<Loc>();
          dump_line(L, "lhs loc sval");

          const auto LV = State->getSVal(L);
          dump_line(LV, "lhs loc value sval");

          if (isa<Loc>(LV)) {
            DUMP(llvm::outs()  << "lhs loc value is a loc\n");
          }
          State = State->BindExpr(LHS, LCtx, LV);

          C.addTransition(State);
          CHECK_RETURN;
        }
      }
    }
  }

  if ((S->getOpcode() != BO_PtrMemI && S->getOpcode() != BO_PtrMemD) || !S->getLHS() || !S->getRHS()) {
    CHECK_RETURN;
  }

  if (S->getOpcode() == BO_PtrMemI) {
    auto VL = State->getSVal(S->getLHS(), LCtx).castAs<DefinedOrUnknownSVal>();
    dump_line(VL, "left sval");

    checkPointer(VL, S->getLHS(), State, C);
  }
  auto VR = State->getSVal(S->getRHS(), LCtx).castAs<DefinedOrUnknownSVal>();
  dump_line(VR, "right sval");

  checkPointer(VR, S->getRHS(), State, C);

  CHECK_RETURN;
}

void NullablePointersChecker::checkPostStmt(const UnaryOperator *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostStmt<UnaryOperator>\n");
  dump_color(*S, "UnaryOperator");

  // if (S->getOpcode() != UO_AddrOf) {
  //   llvm::outs() << "ignoring opcode\n";
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return;
  // }

  ProgramStateRef State = C.getState();

  auto SVal = State->getSVal(S, C.getLocationContext());
  dump_line(SVal, "expr sval");

  auto Constraint = getNullConstraint(SVal, State);
  DUMP(llvm::outs() << "expr sval null constraint: " << nullConstraintToString(Constraint) << "\n");

  CHECK_RETURN;
}

void NullablePointersChecker::checkPreStmt(const MemberExpr *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPreStmt<MemberExpr>\n");
  dump_color(*S, "MemberExpr");
  dump_color(*S->getBase(), "base");
  dump_type(S->getBase()->getType(), "base");

  if (isa<CXXThisExpr>(S->getBase())) {
    CHECK_RETURN_LOG("sub-expr is this");
  }

  if (!isNakedRawPointerType(S->getBase()->getType()) && !isNullableObjectType(S->getBase()->getType())) {
    CHECK_RETURN_LOG("sub-expr is not a pointer");
  }

  ProgramStateRef State = C.getState();

  auto SVal = State->getSVal(S->getBase(), C.getLocationContext());
  auto SValType = SVal.getType(C.getASTContext());
  dump_type(SValType, "sub-expr sval");
  dump_line(SVal, "sub-expr sval");

  if (SVal.isUndef()) {
    reportBug(BugUndefinedNullable, State, S, C);
    CHECK_RETURN;
  }

  auto SubExprSVal = SVal.castAs<DefinedOrUnknownSVal>();

  if (isNakedRawPointerType(S->getBase()->getType())) {
    if (const auto *D = S->getBase()->getType()->getPointeeType()->getAsCXXRecordDecl()) {
      if (D->getQualifiedNameAsString() == NotNullPtrQualifiedName) {
        CHECK_RETURN_LOG("receiver is gsl::not_null");
      }
    }
  }

  if (SubExprSVal.isZeroConstant()) {
    DUMP(llvm::outs() << "reporting bug\n");
    reportBug(BugNullablePointerDereference, State, S, C);

    CHECK_RETURN_LOG("sub-expr is nullptr");
  }

  if (const auto *Cast = dyn_cast<ImplicitCastExpr>(S->getBase())) {
    if (Cast->getCastKind() == CK_ArrayToPointerDecay) {
      CHECK_RETURN_LOG("array to pointer decay yields not_null");
    }
  }

  // const auto *SubExprSValRegion = SubExprSVal.getAsRegion();
  // if (!SubExprSValRegion) {
  //   llvm::outs() << "sub-expr SVal has no region\n";
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return;
  // }

  // llvm::outs() << "is receiver an lval loc? " << isLValLoc(SubExprSVal) << "\n";
  // auto RVal = [&SubExprSVal, &State]() {
  //   if (isLValLoc(SubExprSVal)) return State->getSVal(SubExprSVal.castAs<Loc>()).castAs<DefinedOrUnknownSVal>();
  //   return SubExprSVal;
  // }();

  auto Constraint = getNullConstraint(SubExprSVal, State);
  DUMP(llvm::outs() << "sub-expr rval null constraint: " << nullConstraintToString(Constraint) << "\n");

  const auto NullabilityState = getNullabilityState(State, SubExprSVal);
  if (NullabilityState) {
    DUMP(llvm::outs() << "nullability for sub-expr SVal: " << nullabilityToString(*NullabilityState) << "\n");
  }

  if ((NullabilityState && *NullabilityState == NullableKind::NonNullable) || Constraint == NullConstraint::IsNotNull) {
    CHECK_RETURN;
  }

  reportBug(BugNullablePointerDereference, State, S, C);

  CHECK_RETURN;
}

void NullablePointersChecker::checkPostStmt(const MemberExpr *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostStmt<MemberExpr>\n");
  dump_color(*S, "MemberExpr");

  auto T = S->getType();
  dump_type(T, "expr");
  if (!isNakedRawPointerType(T) && !isNullableObjectType(T)) {
    CHECK_RETURN_LOG("not of pointer or lval ref to pointer type");
  }

  ProgramStateRef State = C.getState();

  // pointers returned by member exprs, i.e. member fields of nullable pointer type
  // are non-local; any exit from current function scope invalidtes a nullable-safety proof

  auto Val = State->getSVal(S, C.getLocationContext());
  dump_line(Val, "pointer sval");

  if (Val.isPureUnknown()) {
    CHECK_RETURN_LOG("unknown sval");
  }
  if (isNonNullableGLValLoc(Val.castAs<DefinedOrUnknownSVal>())) {
    DUMP(llvm::outs() << "pointer within not_null\n");
    if (isNullableObjectType(T)) {
      if (const auto K = getNullableObjectStateKey(Val)) {
        State = dno::createConstraint(State, K, NullConstraint::IsNotNull);

        DUMP(llvm::outs() << "recording nullable object constraint to not_null\n");
        C.addTransition(State);
      }
    }

    CHECK_RETURN;
  }

  const auto *ValReg = Val.getAsRegion();
  if (!isLocalRegion(ValReg, C.getLocationContext())) {
    DUMP(llvm::outs() << "recording pointer as non-local\n");

    State = State->add<NonLocalNullable>(ValReg);
  }

  C.addTransition(State);

  CHECK_RETURN;
}

void NullablePointersChecker::checkPostStmt(const MaterializeTemporaryExpr *S, CheckerContext &C) const {
  if (!S) return;
  if (!S->getSubExpr()) return;

  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostCall\n");
  dump_color(*S, "materialize temporary expr");

  const auto *Record = S->getType()->getAsCXXRecordDecl();
  if (!Record || !isNullableObject(Record)) {
    CHECK_RETURN_LOG("not a nullable object");
  }

  ProgramStateRef State = C.getState();

  auto MaterializedSVal = State->getSVal(S, C.getLocationContext());
  dump_line(MaterializedSVal, "materialized sval");

  auto TemporarySVal = State->getSVal(S->getSubExpr(), C.getLocationContext());
  dump_line(TemporarySVal, "temporary sval");

  const auto MaterialKey = getNullableObjectStateKey(MaterializedSVal);
  const auto TemporaryKey = getNullableObjectStateKey(TemporarySVal);
  if (!MaterialKey || !TemporaryKey) {
    CHECK_RETURN_LOG("no keys for material or temporary");
  }

  auto TempState = State->get<DNOConstraintMap>(TemporaryKey);
  if (!TempState) {
    DUMP(llvm::outs() << "no state recorded for temporary nullable object\n");
    State = dno::createConstraint(State, TemporaryKey, NullConstraint::Unknown);
    DUMP(llvm::outs() << "recorded unknown null constraint on temporary nullable object\n");
  }
  assert(!State->contains<DNOConstraintMap>(MaterialKey) && "symbolic state should not contain a materialized temporary nullable object sval null constraint at this point");

  State = dno::alias(State, MaterialKey, TemporaryKey);
  DUMP(llvm::outs() << "recorded temporary's null constraint on materialized nullable object\n");

  C.addTransition(State);

  CHECK_RETURN;
}

void NullablePointersChecker::checkPostStmt(const DeclStmt *S, CheckerContext &C) const {
  if (!S) return;

  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostStmt\n");
  dump_color(*S, "decl stmt");

  // Same as in ExprEngine
  // Assumption: The CFG has one DeclStmt per Decl.
  const VarDecl *VD = dyn_cast_or_null<VarDecl>(*S->decl_begin());
  if (!VD) {
    CHECK_RETURN;
  }

  ProgramStateRef State = C.getState();

  auto VarSVal = State->getLValue(VD, C.getLocationContext());
  dump_line(VarSVal, "var sval");

  if (!VD->getInit()) {
    CHECK_RETURN;
  }

  auto InitSVal = State->getSVal(VD->getInit(), C.getLocationContext());
  dump_line(InitSVal, "init sval");

  if (!isNullableObject(getCXXRecordFromType(VD->getType())) && !isNullableObject(getCXXRecordFromType(VD->getInit()->getType()))) {
    CHECK_RETURN;
  }

  DUMP(llvm::outs() << "var decl of nullable object\n");

  State = dno::alias(State, getNullableObjectStateKey(VarSVal), getNullableObjectStateKey(InitSVal));

  C.addTransition(State);

  CHECK_RETURN;
}

void NullablePointersChecker::checkPostStmt(const ImplicitCastExpr *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostStmt\n");
  dump_color(*S, "Stmt");

  if (S->getCastKind() == CK_ArrayToPointerDecay) {
    DUMP(llvm::outs() << "array-to-pointer decay\n");

    ProgramStateRef State = C.getState();

    const auto V = State->getSVal(S, C.getLocationContext());
    dump_line(V, "cast sval");

    DUMP((llvm::outs() << "null constraint: " << nullConstraintToString(getNullConstraint(V, State)) << "\n"));

    DUMP(llvm::outs() << "constraining to not-null\n");
    auto T = V.getType(C.getASTContext());
    dump_type(T, "val");
    auto L = V.castAs<Loc>();
    auto &SVB = C.getSValBuilder();
    auto Zero = SVB.makeZeroVal(T).castAs<Loc>();

    const auto *SV = C.getSymbolManager().getRegionValueSymbol(V.getAsRegion()->getAs<TypedValueRegion>());
    dump_line(*SV, "symbol for sval region");
    dump_type(SV->getType(), "symbol");

    const auto SVL = SVB.makeLoc(SV);
    dump_line(SVL, "sval region symbol loc sval");

    auto Constraint = SVB.evalBinOp(State, BO_EQ, L, Zero, C.getASTContext().BoolTy);
    State = State->assume(Constraint.castAs<DefinedSVal>(), false);

    C.addTransition(State);
  }

  CHECK_RETURN;
}


// void NullablePointersChecker::checkPostStmt(const InitListExpr *S, CheckerContext &C) const {
//   llvm::outs() << "NULLABLE POINTERS :: checkPreCall\n";
//   dump_color(*S, "int list expr");

//   ProgramStateRef State = C.getState();
//   const auto *LCtx = C.getLocationContext();

//   for (const Expr *Init : S->inits()) {
//     dump_color(*Init, "init expr");

//     auto InitSVal = State->getSVal(Init, LCtx);
//     dump_line(InitSVal, "init sval");

//     auto Type = InitSVal.getType(C.getASTContext());
//     auto Val = InitSVal;
//     while ((Type->isLValueReferenceType() || Type->isPointerType()) && Type->getPointeeType()->isPointerType()) {
//         auto L = Val.castAs<Loc>();
//         auto V = State->getSVal(L).castAs<DefinedOrUnknownSVal>();
//         if (isLValLoc(L)) {
//           auto Constraint = getNullConstraint(V, State);
//           llvm::outs() << "sval null constraint: " << nullConstraintToString(Constraint) << "\n";

//           llvm::outs() << "ptr escaping scope; recording constraint state\n";
//           auto Validity = Type->getPointeeType().isConstQualified() ? ConstraintValidty::AlwaysValid : ConstraintValidty::Unknown;
//           State = State->set<EscapedNullable>(L.getAsRegion(), {Constraint, Validity});
//         }

//         Type = Type->getPointeeType();
//         Val = V;
//         dump_type(Type, "next type");
//         dump_line(Val, "next sval");
//     }
//   }

//   C.addTransition(State);

//   llvm::outs() << "NULLABLE POINTERS\n\n";
// }

bool NullablePointersChecker::checkCalleeExpr(const Expr *E, ProgramStateRef State, CheckerContext &C) const {
  if (!E) return false;
  if (isa<CXXMemberCallExpr>(E)) return false;
  if (const auto *OperatorCall = dyn_cast<CXXOperatorCallExpr>(E)) {
    if (OperatorCall->getOperator() != OO_Star && OperatorCall->getOperator() != OO_Arrow && OperatorCall->getOperator() != OO_ArrowStar) return false;
    DUMP(llvm::outs() << "checking operator callee expr\n");

    const auto *D = OperatorCall->getCalleeDecl();
    if (!D) return false;

    const auto *Callee = D->getAsFunction();
    if (!Callee) return false;

    const auto *Target = [Callee]() -> const CXXRecordDecl * {
      if (const auto *M = dyn_cast<CXXMethodDecl>(Callee)) {
        return M->getParent();
      }
      assert(!Callee->param_empty() && "no parameters for operator overload function");
      auto TargetType = Callee->parameters()[0]->getType()->getCanonicalTypeUnqualified().getNonReferenceType();
      return TargetType->getAsCXXRecordDecl();
    }();

    if (!Target) return false;

    if (!isNullableObject(Target)) return false;

    auto TargetSVal = State->getSVal(OperatorCall->getArg(0), C.getLocationContext());
    dump_line(TargetSVal, "nullable-object deref target sval");

    if (TargetSVal.isUnknownOrUndef()) {
      reportBug(BugUndefinedNullable, State, E, C);
      return true;
    }

    if (isNonNullableGLValLoc(TargetSVal.castAs<DefinedOrUnknownSVal>())) {
      DUMP(llvm::outs() << "target is not_null::ptr_\n");
      return false;
    }

    // const auto *TargetRegion = TargetSVal.getAsRegion();
    // if (!TargetRegion) return false;
    // dump_line(*TargetRegion, "nullable-object region");
    // if (!isa<DefinedSVal>(TargetSVal)) return false;
    const auto TargetKey = getNullableObjectStateKey(TargetSVal);

    auto TargetState = State->get<DNOConstraintMap>(TargetKey);
    if (!TargetState || *TargetState != NullConstraint::IsNotNull) {
      if (TargetState) {
        DUMP(llvm::outs() << "nullable object constraint: " << nullConstraintToString(*TargetState) << "\n");
      }
      reportBug(BugNullablePointerDereference, State, E, C);
      return true;
    }
    DUMP(llvm::outs() << "nullable object state: not-null\n");
    return false;
  }
  const auto *Call = dyn_cast<CallExpr>(E);
  if (!Call) return false;

  const auto *Callee = Call->getCallee();
  if (!Callee) return false;

  const auto *CastExpr = dyn_cast<ImplicitCastExpr>(Callee);
  if (!CastExpr) {
    // this expression now must be an rvalue
    reportBug(BugNullablePointerDereference, State, E, C);
    return true;
  }

  switch (CastExpr->getCastKind()) {
  default:
    break;
  case CK_FunctionToPointerDecay:
  case CK_BuiltinFnToFnPtr:
    return false;
  }

  if (CastExpr->getCastKind() == CK_FunctionToPointerDecay) return false;

  // a this point the implicit cast is an lvalue-to-rvalue

  const auto CalleeSVal = State->getSValAsScalarOrLoc(CastExpr, C.getLocationContext());
  dump_line(CalleeSVal, "callee sval");

  const auto Constraint = getNullConstraint(CalleeSVal.castAs<DefinedOrUnknownSVal>(), State);
  if (Constraint == NullConstraint::IsNotNull) return false;

  reportBug(BugNullablePointerDereference, State, E, C);
  return true;
}

void NullablePointersChecker::checkPreCall(const CallEvent &CallEvent, CheckerContext &C) const {
  if (!CallEvent.getOriginExpr()) return;

  const Expr* E = CallEvent.getOriginExpr();

  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPreCall\n");
  dump_color(*CallEvent.getOriginExpr(), "call expr");

  ProgramStateRef State = C.getState();

  if (checkCalleeExpr(E, State, C)) {
    CHECK_RETURN;
  }

  const bool IgnoreEscapeAnalysis = ignoreEscapeAnalysis(CallEvent);

  if (!IgnoreEscapeAnalysis) {
    const auto Escaped = State->get<EscapedNullable>();
    for (const auto &EscapeRecord: Escaped) {
      auto LValRegion = EscapeRecord.first;
      auto Validity = EscapeRecord.second.Validity;

      auto LVal = C.getSValBuilder().makeLoc(LValRegion);
      dump_line(LVal, "escaped ptr loc sval");

      // auto RVal = State->getSVal(LVal).castAs<DefinedOrUnknownSVal>();
      // dump_line(RVal, "escaped ptr rval");

      if (Validity != ConstraintValidity::Invalid) {
        auto CurrentConstraint = getNullConstraint(LVal, State);
        DUMP(llvm::outs() << "updating rval constraint to: " << nullConstraintToString(CurrentConstraint) << "\n");

        State = State->set<EscapedNullable>(LValRegion, {CurrentConstraint, Validity});
      }
    }
  }

  bool IsNotNullConstruction = [E,&CallEvent]() {
    if (const auto *ConstructExpr = dyn_cast<CXXConstructExpr>(E)) {
      const auto *ConstructDecl = ConstructExpr->getConstructor();
      return ConstructDecl->getParent()->getQualifiedNameAsString() == NotNullPtrQualifiedName && !(ConstructDecl->isCopyOrMoveConstructor() || isExprANotNullPtr(CallEvent.getArgExpr(0)));
    }
    if (const auto *CallExpr = dyn_cast<::clang::CallExpr>(E)) {
      const auto *CalleeDecl = CallExpr->getCalleeDecl();
      if (!CalleeDecl) return false;
      if (const auto *F = CalleeDecl->getAsFunction()) return F->getQualifiedNameAsString() == MakeNotNullQualifiedName;
    }

    return false;
  }();

  if (IsNotNullConstruction) {
    DUMP(llvm::outs() << "construct a gsl::not_null\n");

    const auto ArgSVal = CallEvent.getArgSVal(0).castAs<DefinedOrUnknownSVal>();
    dump_line(ArgSVal, "arg sval");

    if (const auto *Cast = dyn_cast<ImplicitCastExpr>(CallEvent.getArgExpr(0))) {
      if (Cast->getCastKind() == CK_ArrayToPointerDecay) {
        CHECK_RETURN_LOG("array to pointer decay yields not_null");
      }
    }

    NullConstraint Constraint = getNullConstraint(ArgSVal, State);
    DUMP(llvm::outs() << "arg sval null constraint: " << nullConstraintToString(Constraint) << "\n");
    if (Constraint != NullConstraint::IsNotNull) {
      DUMP(llvm::outs() << "constructing not_null from nullable\n");
      reportNonNullableFromNullable(State, CallEvent, C);
      // reportBug(BugNullableToNonNullable, State, E, C);
    }

    CHECK_RETURN;
  }

  if (!IgnoreEscapeAnalysis) {
    for (unsigned int I = 0; I < CallEvent.getNumArgs(); ++I) {
      const auto *ArgExpr = CallEvent.getArgExpr(I);
      dump_color(*ArgExpr, "arg expr");
      // llvm::outs() << "arg is lval: " << ArgExpr->isLValue() << "\n";

      auto ArgSVal = CallEvent.getArgSVal(I);
      dump_line(ArgSVal, "arg sval");

      // if (!ArgExpr->isLValue()) continue;
      const auto *Param = CallEvent.parameters()[I];
      dump_color(*Param, "param");

      auto ParamType = Param->getType();
      dump_type(ParamType, "param");

      auto Type = ParamType;
      auto Val = ArgSVal;
      if (isNonNullableType(Type)) {
        if (const auto *FD = getNonNullableField(Type)) {
          if (const auto LV = Val.getAs<nonloc::LazyCompoundVal>()) {
            const auto *R = LV->getRegion();
            dump_line(*R, "non-nullable object region");
            auto NNVL = State->getLValue(FD, loc::MemRegionVal(R));
            dump_line(NNVL, "non-nullable ptr_ lval");
            dump_type(NNVL.getType(C.getASTContext()), "non-nullable ptr_ lval");
            auto NNV = State->getSVal(NNVL.castAs<Loc>());
            dump_line(NNV, "non-nullable ptr_ sval");
            Val = NNV;
            Type = FD->getType();
          }
        }
      }
      while ((Type->isLValueReferenceType() || Type->isPointerType()) && Type->getPointeeType()->isPointerType()) {
        auto L = Val.castAs<Loc>();
        auto V = State->getSVal(L).castAs<DefinedOrUnknownSVal>();
        if (isLValLoc(L)) {
          auto Constraint = getNullConstraint(V, State);
          DUMP(llvm::outs() << "sval null constraint: " << nullConstraintToString(Constraint) << "\n");

          DUMP(llvm::outs() << "ptr escaping scope; recording constraint state\n");
          if (!Type->getPointeeType().isConstQualified() && Type->getPointeeType().isVolatileQualified()) {
            reportBug(BugVolatilePointerEscape, State, E, C);
            CHECK_RETURN;
          }
          // auto Validity = Type->getPointeeType().isConstQualified() ? ConstraintValidity::AlwaysValid : (Type->getPointeeType().isVolatileQualified() ? ConstraintValidity::AlwaysInvalid : ConstraintValidity::Valid);
          auto Validity = Type->getPointeeType().isConstQualified() ? ConstraintValidity::AlwaysValid : ConstraintValidity::Valid;
          // State = State->set<EscapedNullable>(V.getAsRegion(), {Constraint, Validity});
          State = State->set<EscapedNullable>(L.getAsRegion(), {Constraint, Validity});
        }

        Type = Type->getPointeeType();
        Val = V;
        dump_type(Type, "next");
        dump_line(Val, "next sval");
      }
    }
  }

  C.addTransition(State);

  CHECK_RETURN;
}

const Expr *getBranchLValueExpr(const Stmt *S) {
  switch (S->getStmtClass()) {
  case Stmt::ImplicitCastExprClass: {
    const auto *E = dyn_cast<ImplicitCastExpr>(S)->getSubExprAsWritten();
    if (!E->isLValue()) return nullptr;
    return E;
  }
  default:
    return nullptr;
  }
}

const Expr *getBranchRValueExpr(const Stmt *S) {
  switch (S->getStmtClass()) {
  case Stmt::ImplicitCastExprClass:
    return dyn_cast<ImplicitCastExpr>(S)->getSubExpr();
    break;
  default:
    return nullptr;
  }
}

namespace {

struct ReEvaluator : ConstStmtVisitor<ReEvaluator> {
  ReEvaluator(CheckerContext &C, ExprEngine &Engine, ProgramStateRef State, const LocationContext *LCtx) : C(C), Engine(Engine), LCtx(LCtx) {
    Preds.Add(C.addTransition(State));
  }

  void VisitChildren(const Stmt *S) {
    for (const auto *Child : S->children()) {
      if (Child) Visit(Child);
    }
  }

  void VisitDeclRefExpr(const DeclRefExpr */*E*/) {}

  void VisitStmt(const Stmt *S) {
    removeBinding(S);
    VisitChildren(S);
    reEval(S);
  }

  void removeBinding(const Stmt *S) {
    ExplodedNodeSet NewPreds;
    for (auto *Pred : Preds) {
      auto State = Pred->getState()->UnbindExpr(S, LCtx);
      NewPreds.Add(C.addTransition(State, Pred));
    }
    Preds = NewPreds;
  }

  void reEval(const Stmt *S) {
    ExplodedNodeSet NewPreds;
    for (auto *Pred : Preds) {
      ExplodedNodeSet TempDst;

      dump_color(*S, "re-evaluating");

      Engine.Visit(S, Pred, TempDst);

      for (const auto *N : TempDst) {
        DUMP(llvm::outs() << "new state node...\n");
        auto V = N->getState()->getSVal(S, LCtx);
        dump_line(V, "sval after re-eval");

        NewPreds.Add(C.addTransition(N->getState(), Pred));
      }
    }
    Preds.clear();
    Preds.insert(NewPreds);
  }

  CheckerContext &C;
  ExprEngine &Engine;
  const LocationContext *LCtx;
  ExplodedNodeSet Preds;
  SmallVector<ProgramStateRef, 4> States;
};

}

void NullablePointersChecker::checkBranchCondition(const Stmt *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkBranchCondition\n");
  dump_color(*S, "Stmt");

  ProgramStateRef State = C.getState();

  auto Condition = State->getSVal(S, C.getLocationContext());
  dump_line(Condition, "condition sval");

  DUMP(llvm::outs() << "block count: " << C.blockCount() << "\n");

  C.addTransition(State);
  CHECK_RETURN_LOG("early exit; skipping condition re-eval");

  const auto LS = LoopState::top(State);
  if (LS) {
    // branch condition within a loop
    // weaken the branch condition to force execution of every branch
    const auto Mods = getModifiedDeclRefs(LS->Loop, C.getASTContext());
    llvm::SmallSetVector<const DeclRefExpr *, 3> ModSet { Mods.begin(), Mods.end() };

    const auto CondFree = getFreeDeclRefs(S, C.getASTContext());
    SmallVector<SVal, 3> Values;
    for (const auto *FreeVar : CondFree) {
      if (ModSet.contains(FreeVar)) continue;
      dump_color(*FreeVar, "free");
      if (const auto *Var = dyn_cast<VarDecl>(FreeVar->getFoundDecl())) {
        auto FreeVal = State->getLValue(Var, C.getLocationContext());
        dump_line(FreeVal, "free val");
        Values.push_back(FreeVal);
      }
    }

    if (Values.empty()) CHECK_RETURN_LOG("no values to weaken");

    State = State->invalidateRegions(Values, dyn_cast<Expr>(S), C.blockCount(), C.getLocationContext(), true);

    DUMP(llvm::outs() << "re-evaluating condition with ExprEngine...\n");

    ReEvaluator ReEval(C, C.getStateManager().getOwningEngine(), State, C.getLocationContext());
    ReEval.Visit(S);

    for (auto *N : ReEval.Preds) {
      DUMP(llvm::outs() << "new node with re-eval result...\n");
      State = N->getState();

      auto NewCond = State->getSVal(S, C.getLocationContext());
      dump_line(NewCond, "condition after free invalidation");
      if (NewCond.isUnknownOrUndef()) {
        CHECK_RETURN_LOG("unknown or undef condition after free invalidation");
      }
    }

    CHECK_RETURN_LOG("continuing with weakened branch condition");
  }

  CHECK_RETURN;
}

void NullablePointersChecker::checkLoopCondition(const Stmt *L, const Stmt *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkLoopCondition\n");
  dump_color(*L, "loop");
  dump_color(*S, "condition");

  ProgramStateRef State = C.getState();

  auto Cond = State->getSVal(S, C.getLocationContext());
  if (Cond.isUnknownOrUndef()) {
    CHECK_RETURN_LOG("loop condition is unknown or undefined");
  }
  auto Condition = Cond.castAs<DefinedSVal>();
  dump_line(Condition, "condition sval");

  DUMP(llvm::outs() << "block count: " << C.blockCount() << "\n");

  dump_line(*C.getPredecessor()->getCFGBlock(), "CFG block");

  if (C.blockCount() == 1) {
    // experimenting with weakening at this point
    // NOT NEEDED
    // for (const auto *WExpr : State->get<WeakenSet>()) {
    //   dump_color(*WExpr, "expr marked for weakening");
    //   auto WVal = State->getSVal(WExpr, C.getLocationContext());
    //   dump_line(WVal, "sval of expr");

    //   auto *W = const_cast<Expr*>(cast<Expr>(WExpr));
    //   const auto *Cast = ImplicitCastExpr::Create(C.getASTContext(), W->getType(), CK_LValueToRValue, W, nullptr, VK_PRValue, {});

    //   auto V = State->getSVal(Cast, C.getLocationContext());
    //   dump_line(V, "lval-to-rval sval");

    //   if (const auto *DR = dyn_cast<DeclRefExpr>(WExpr)) {
    //     if (const auto *PV = dyn_cast<VarDecl>(DR->getFoundDecl())) {
    //       auto DV = State->getLValue(PV, C.getLocationContext());
    //       dump_line(DV, "lvalue sval");

    //       auto DVRV = State->getSVal(DV);
    //       dump_line(DVRV, "rvalue sval");
    //     }
    //   }

    //   // if (WVal.isUnknown()) {
    //   //   auto T = cast<Expr>(WExpr)->getType();
    //   //   auto V = C.getSValBuilder().conjureSymbolVal(S, C.getLocationContext(), T, C.blockCount());
    //   //   dump_line(V, "unknown replacement");
    //   //   State = State->BindExpr(S, C.getLocationContext(), V);
    //   // }
    // }
    if (!Condition.isConstant()) {
      ProgramStateRef StTrue, StFalse;
      std::tie(StTrue, StFalse) = State->assume(Condition);
      if (StTrue) {
        C.addTransition(StTrue);
      }
      if (StFalse) {
        auto &F = StFalse->get_context<WeakenMap>();
        StFalse = StFalse->set<WeakenMap>(F.getEmptyMap());
        auto &FS = StFalse->get_context<WeakenSynonyms>();
        StFalse = StFalse->set<WeakenSynonyms>(FS.getEmptyMap());
        C.addTransition(StFalse);
      }

      CHECK_RETURN;
    }

    if (Condition.isZeroConstant()) {
        auto &F = State->get_context<WeakenMap>();
        State = State->set<WeakenMap>(F.getEmptyMap());
        auto &FS = State->get_context<WeakenSynonyms>();
        State = State->set<WeakenSynonyms>(FS.getEmptyMap());
        C.addTransition(State);
    }

    CHECK_RETURN;
  }

  if (C.blockCount() == 2) {
    if (!Condition.isConstant()) {
      ProgramStateRef StTrue, StFalse;
      std::tie(StTrue, StFalse) = State->assume(Condition);
      if (StFalse) {
        auto &F = StFalse->get_context<WeakenMap>();
        StFalse = StFalse->set<WeakenMap>(F.getEmptyMap());
        auto &FS = StFalse->get_context<WeakenSynonyms>();
        StFalse = StFalse->set<WeakenSynonyms>(FS.getEmptyMap());
        C.addTransition(StFalse);
      }

      CHECK_RETURN;
    }

    if (!Condition.isZeroConstant()) {
      C.addSink();
      CHECK_RETURN_LOG("sinking true branch");
    }
    auto &F = State->get_context<WeakenMap>();
    State = State->set<WeakenMap>(F.getEmptyMap());
    auto &FS = State->get_context<WeakenSynonyms>();
    State = State->set<WeakenSynonyms>(FS.getEmptyMap());
    C.addTransition(State);
  }

  CHECK_RETURN;

  if (C.blockCount() == 1) {
    // on the false branch just pass through
    if (!Condition.isConstant()) {
      ProgramStateRef StTrue, StFalse;
      std::tie(StTrue, StFalse) = State->assume(Condition);
      C.addTransition(StFalse); // directly transition to the state of the false branch
      State = StTrue; // continue with the state of the true branch
    } else {
      if (Condition.isZeroConstant()) {
        // C.addSink();
        CHECK_RETURN_LOG("continuing with loop exit path");
      }
    }

    // on the true branch
    // weaken any modified lvalue
    // const auto ConditionFree = getFreeDeclRefs(S, C.getASTContext());
    const auto Mods = getModifiedDeclRefsNonNested(L, C.getASTContext());
    // const auto NonConditionMods = [&ConditionFree,&Mods]() {
    //   const auto ConditionFreeVars = llvm::map_range(ConditionFree, [](const DeclRefExpr *Free) { return Free->getFoundDecl(); });
    //   const auto ConditionFreeSet = llvm::SmallSetVector<const NamedDecl*, 3>(ConditionFreeVars.begin(), ConditionFreeVars.end());
    //   const auto R = llvm::make_filter_range(Mods, [&ConditionFreeSet](const DeclRefExpr *Mod) { return !ConditionFreeSet.contains(Mod->getFoundDecl()); });
    //   return llvm::SmallVector<const DeclRefExpr*, 3>(R.begin(), R.end());
    // }();
    // for (const auto *Mod : NonConditionMods) {
    for (const auto *Mod : Mods) {
      dump_color(*Mod, "mod");

      auto ModVal = State->getLValue(dyn_cast<VarDecl>(Mod->getDecl()), C.getLocationContext());
      dump_line(ModVal, "mod val");
      SmallVector<SVal, 1> ModValues = {ModVal};

      // auto WeakVal = C.getSValBuilder().conjureSymbolVal(Mod, C.getLocationContext(), Mod->getType(), C.blockCount());
      // dump_line(WeakVal, "weakened val");

      // State = State->bindLoc(ModVal.castAs<Loc>(), WeakVal, C.getLocationContext());
      State = State->invalidateRegions(ModValues, Mod, C.blockCount(), C.getLocationContext(), true);
    }

    // State = LoopState::push(State, C.getPredecessor(), Condition);
    State = LoopState::push(State, L);

    C.addTransition(State);
    CHECK_RETURN_LOG("continuing in true branch");
  }

  if (C.blockCount() == 2) {
    // end of first iteration, weaken everything modified within the body
    // const auto Mods = getModifiedDeclRefs(L, C.getASTContext());
    // for (const auto *Mod : Mods) {
    //   dump_color(*Mod, "mod");

    //   auto ModVal = State->getLValue(dyn_cast<VarDecl>(Mod->getDecl()), C.getLocationContext());
    //   dump_line(ModVal, "mod val");

    //   auto WeakVal = C.getSValBuilder().conjureSymbolVal(Mod, C.getLocationContext(), Mod->getType(), C.blockCount());
    //   dump_line(WeakVal, "weakened val");

    //   State = State->bindLoc(ModVal.castAs<Loc>(), WeakVal, C.getLocationContext());
    // }
    State = LoopState::pop(State);

    const auto CondFree = getFreeDeclRefs(S, C.getASTContext());
    SmallVector<SVal, 3> Values;
    for (const auto *FreeVar : CondFree) {
      dump_color(*FreeVar, "free");
      if (const auto *Var = dyn_cast<VarDecl>(FreeVar->getFoundDecl())) {
        auto FreeVal = State->getLValue(Var, C.getLocationContext());
        dump_line(FreeVal, "free val");
        Values.push_back(FreeVal);
      }
    }

    State = State->invalidateRegions(Values, dyn_cast<Expr>(S), C.blockCount(), C.getLocationContext(), true);

    CHECK_RETURN_LOG("TESTING EARLY EXIT");
    DUMP(llvm::outs() << "re-evaluating condition with ExprEngine...\n");

    ReEvaluator ReEval(C, C.getStateManager().getOwningEngine(), State, C.getLocationContext());
    ReEval.Visit(S);

    for (auto *N : ReEval.Preds) {
      DUMP(llvm::outs() << "new node with re-eval result...\n");
      State = N->getState();

      auto NewCond = State->getSVal(S, C.getLocationContext());
      dump_line(NewCond, "condition after free invalidation");
      if (NewCond.isUnknownOrUndef()) {
        CHECK_RETURN_LOG("unknown or undef condition after free invalidation");
      }
      ProgramStateRef StTrue, StFalse;
      std::tie(StTrue, StFalse) = State->assume(NewCond.castAs<DefinedOrUnknownSVal>());
      if (!StTrue && !StFalse) {
        CHECK_RETURN_LOG("inconsistent state for loop condition");
      }
      if (StTrue) {
        DUMP(llvm::outs() << "Not transitioning to true branch\n");
        C.generateSink(StTrue, N);
      }
      if (StFalse) {
        DUMP(llvm::outs() << "Transitioning to false branch\n");
        C.addTransition(StFalse);
      }
      // if (!NewCond.isZeroConstant()) {
      //   C.generateSink(State, N);
      // }
      // if (NewCond.isZeroConstant()) {
      //   C.addTransition(State);
      // } else {
      //   C.addSink();
      // }

      // ProgramStateRef StTrue, StFalse;
      // std::tie(StTrue, StFalse) = State->assume(NewCond.castAs<DefinedOrUnknownSVal>());
      // if (StTrue) {
      //   llvm::outs() << "sinking true state\n";
      //   // C.generateSink(StTrue, N);
      //   C.generateSink(StTrue, C.getPredecessor());
      // }
      // if (StFalse) {
      //   llvm::outs() << "keeping false state\n";
      //   // C.addTransition(StFalse, N);
      //   C.addTransition(StFalse);
      // }
      // if (!StFalse && !StTrue) {
      //   CHECK_RETURN_LOG("incosistent state for loop condition");
      // }
    }

    CHECK_RETURN_LOG("loop widened");
  }

  CHECK_RETURN;
}


void NullablePointersChecker::checkPostStmt(const BreakStmt *S, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPostStmt<BreakStmt>\n");
  dump_color(*S, "break");

  ProgramStateRef State = C.getState();
  auto &F = State->get_context<WeakenMap>();
  auto &FS = State->get_context<WeakenSynonyms>();
  State = State->set<WeakenSynonyms>(FS.getEmptyMap());
  C.addTransition(State->set<WeakenMap>(F.getEmptyMap()));

  CHECK_RETURN;
}

ProgramStateRef NullablePointersChecker::checkPointerEscape(
    ProgramStateRef State, const InvalidatedSymbols &Escaped,
    const CallEvent *Call, PointerEscapeKind Kind) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: checkPointerEscape\n");
  if (Call) {
    dump_color(*Call->getOriginExpr(), "call expr");
  }

  for (const auto *Sym : Escaped) {
    auto EscapedPtrSymbol = SymbolRef { Sym };
    dump_line(*EscapedPtrSymbol, "escaped symbol");

    const auto *R = EscapedPtrSymbol->getOriginRegion();
    if (R) dump_line(*R, "escaped region");

    dump_type(EscapedPtrSymbol->getType(), "escaped");
  }

  DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
  return State;
}

ProgramStateRef NullablePointersChecker::checkPointerAddressEscapes(ProgramStateRef State, Loc L, SVal V, const ASTContext &C) const {
  DUMP(llvm::outs() << "checking pointer escapes\n");
  dump_line(L, "to loc");

  if (!isGlobalSVal(L)) return State;
  DUMP(llvm::outs() << "L is global sval");

  if (isNonNullableType(V.getType(C))) {
    if (const auto *FD = getNonNullableField(V.getType(C))) {
      if (const auto LV = V.getAs<nonloc::LazyCompoundVal>()) {
        const auto *R = LV->getRegion();
        dump_line(*R, "non-nullable object region");
        auto NNVL = State->getLValue(FD, loc::MemRegionVal(R));
        dump_line(NNVL, "non-nullable ptr_ lval");
        dump_type(NNVL.getType(C), "non-nullable ptr_ lval");
        auto NNV = State->getSVal(NNVL.castAs<Loc>());
        dump_line(NNV, "non-nullable ptr_ sval");
        V = NNV;
      }
    }
  }

  auto T = getRegionValueType(L.getAsRegion());
  if (const auto *FD = getNonNullableField(T)) {
    T = FD->getType();
  }

  while (isRawPointerType(T) && !T->getPointeeType().isConstQualified() && isRawPointerType(T->getPointeeType())) {
    dump_line(V, "from sval");

    const auto *VR = V.getAsRegion();
    if (!VR) break;
    dump_line(*VR, "from sval region");
    auto EV = VR->getKind() == MemRegion::SymbolicRegionKind ? V : State->getSVal(VR);
    // if (!isGlobalRegion(VR)) {
    dump_line(EV, "escaped ptr sval");
    auto Constraint = getNullConstraint(EV.castAs<DefinedOrUnknownSVal>(), State);
    DUMP(llvm::outs() << "escape pointer null constraint: " << nullConstraintToString(Constraint) << "\n");
    // State = State->set<EscapedNullable>(EV.getAsRegion(), EscapedNullableState { Constraint, T->getPointeeType().isVolatileQualified() ? ConstraintValidity::Invalidated : ConstraintValidity::Unknown });
    if (T->getPointeeType().isVolatileQualified()) {
      return nullptr;
    }
    State = State->set<EscapedNullable>(VR, EscapedNullableState { Constraint, ConstraintValidity::Valid });
    // }

    T = T->getPointeeType();
    V = State->getSVal(V.castAs<Loc>());
    dump_type(T, "next");
  }

  return State;
}

ProgramStateRef NullablePointersChecker::evalAssume(ProgramStateRef State, SVal Cond, bool Assumption) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: evalAssume\n");
  dump_line(Cond, "condition sval");
  DUMP(llvm::outs() << "assumption: " << Assumption << "\n");

  if (Cond.isUndef()) {
    DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
    return State;
  }

  // if (const auto LS = LoopState::top(State)) {
  //   if (const auto *Sym = Cond.getAsSymbol())
  //     // we are in a loop context; record the constraint
  //     State = LS->record(State, LoopState::Constraint { Sym, Assumption });
  // }

  if (State->get<AliasGuard>()) {
    DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
    return State;
  }

  auto &CMgr = State->getConstraintManager();
  auto PtrToDNOAliases = State->get<PtrToDNOAliasMap>();
  for (const auto &PtrAliases : PtrToDNOAliases) {
    const auto *PtrSym = PtrAliases.first;
    const auto Aliases = PtrAliases.second;

    if (State->contains<PtrGuards>(PtrSym)) continue;
    State = State->add<PtrGuards>(PtrSym);

    dump_line(*PtrSym, "updating aliases for symbol");

    auto IsNullConstraint = CMgr.isNull(State, PtrSym);
    if (IsNullConstraint.isConstrainedTrue()) {
      DUMP(llvm::outs() << "symbol constrainted to: null\n");
      State = dno::updateConstraint(State, PtrSym, NullConstraint::IsNull);
    }
    if (IsNullConstraint.isConstrainedFalse()) {
      DUMP(llvm::outs() << "symbol constrainted to: not-null\n");
      State = dno::updateConstraint(State, PtrSym, NullConstraint::IsNotNull);
    }
  }

  PtrGuardsTy::Factory &F = State->get_context<PtrGuards>();
  State = State->set<PtrGuards>(F.getEmptySet());

  DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
  return State;
}

bool NullablePointersChecker::evalCall(const CallEvent &CE, CheckerContext &C) const {
  DUMP(llvm::outs() << "NULLABLE POINTERS :: evalCall\n");
  if (!CE.getOriginExpr()) {
    DUMP(llvm::outs() << "no call expression\n");
    DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
    return false;
  }
  dump_color(*CE.getOriginExpr(), "CallExpr");

  if (isInterProcedural(CE)) {
    DUMP(llvm::outs() << "inter-procedural call\n");
    DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
    return false;
  }

  auto ReturnType = CE.getResultType();
  if (ReturnType == C.getASTContext().VoidTy) {
    DUMP(llvm::outs() << "returns void\n");
    DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
    return true;
  }

  ProgramStateRef State = C.getState();
  const auto *LCtx = CE.getLocationContext();
  auto &SVB = C.getSValBuilder();

  if (const auto *L = getGuardedLoop(CE.getOriginExpr(), C.getASTContext())) {
    if (const auto *FD = LCtx->getDecl()->getAsFunction()) {
      dump_color(*L, "guarded loop");

      DUMP(llvm::outs() << "block count: " << C.blockCount() << "\n");

      ModsVisitor MV(State, LCtx);
      MV.Scope = L;
      MV.Visit(FD->getBody());

      LoopGuardVisitor LGV;
      LGV.Modified = MV.ModifiedLocExprs();
      const bool GuardNeeded = LGV.Visit(L);
      DUMP(llvm::outs() << "GuardedNeeded = " << GuardNeeded << "\n");

      auto TrueVal = SVB.makeBoolVal(CXXBoolLiteralExpr::Create(C.getASTContext(), true, C.getASTContext().BoolTy, {}));
      auto FalseVal = SVB.makeBoolVal(CXXBoolLiteralExpr::Create(C.getASTContext(), false, C.getASTContext().BoolTy, {}));

      const bool GuardActiveFirstCount = false;

      if (C.blockCount() == 1) {
        if (GuardNeeded && GuardActiveFirstCount) {
          DUMP(llvm::outs() << "generating loop guard false branch transition\n");
          C.addTransition(State->BindExpr(CE.getOriginExpr(), LCtx, FalseVal));
        }

        for (const auto [M, W] : MV.WeakenLocExprs()) {
          dump_color(*M, "recording modified in loop for weakening");
          DUMP(llvm::outs() << "weakening variance: " << W << "\n");
          State = State->set<WeakenMap>(M, W);

          auto &FS = State->get_context<SynonymList>();
          auto Syns = FS.getEmptyList();
          for (const auto *S : MV.ModifiedSynonyms(M))
            Syns = FS.add(S, Syns);

          State = State->set<WeakenSynonyms>(M, Syns);
        }
        DUMP(llvm::outs() << "generating loop guard true branch transition\n");
        C.addTransition(State->BindExpr(CE.getOriginExpr(), LCtx, TrueVal));
      } else {
        auto &F = State->get_context<WeakenMap>();
        State = State->set<WeakenMap>(F.getEmptyMap());
        auto &FS = State->get_context<WeakenSynonyms>();
        State = State->set<WeakenSynonyms>(FS.getEmptyMap());

        if (GuardNeeded) {
          DUMP(llvm::outs() << "generating false branch transition only\n");
          C.addTransition(State->BindExpr(CE.getOriginExpr(), LCtx, FalseVal));
          DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
          return true;
        }

        const auto WeakenExprs = MV.WeakenLocExprs();

        for (const auto *M : LGV.ConditionModified) {
          dump_color(*M, "recording condition modified for weakening");
          const auto W = llvm::find_if(WeakenExprs, [M](const auto &EM) { return EM.first == M; });
          assert(W != WeakenExprs.end() && "condition modified expr is not detected for weakening");
          State = State->set<WeakenMap>(M, W->second);

          auto &FS = State->get_context<SynonymList>();
          auto Syns = FS.getEmptyList();
          for (const auto *S : MV.ModifiedSynonyms(M))
            Syns = FS.add(S, Syns);

          State = State->set<WeakenSynonyms>(M, Syns);
        }

        DUMP(llvm::outs() << "generating true branch transition\n");
        C.addTransition(State->BindExpr(CE.getOriginExpr(), LCtx, TrueVal));
      }

      DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
      return true;
    }
  }

  if (const auto *CC = dyn_cast<CXXConstructExpr>(CE.getOriginExpr())) {
    if (const auto *Ctor = CC->getConstructor()) {
      // check for nullable object construction from nullptr
      if (isNullableObject(Ctor->getParent()) && Ctor->isConvertingConstructor(true) && getNakedType(CC->getArg(0)->getType())->isNullPtrType()) {
        auto NullptrObjectSVal = SVB.conjureSymbolVal(CE.getOriginExpr(), LCtx, ReturnType, C.blockCount());
        dump_line(NullptrObjectSVal, "conjured nullptr nullable object sval");

        State = State->BindExpr(CE.getOriginExpr(), LCtx, NullptrObjectSVal);
        if (const auto K = getNullableObjectStateKey(NullptrObjectSVal)) {
          State = dno::createConstraint(State, K, NullConstraint::IsNull);
        }
        C.addTransition(State);

        DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
        return true;
      }

      if (isNullableObject(Ctor->getParent()) && Ctor->isConvertingConstructor(true) && isNakedRawPointerType(CC->getArg(0)->getType())) {
        auto NullableObjectSVal = SVB.conjureSymbolVal(CE.getOriginExpr(), LCtx, ReturnType, C.blockCount());
        dump_line(NullableObjectSVal, "conjured nullable object from nullable pointer sval");

        State = State->BindExpr(CE.getOriginExpr(), LCtx, NullableObjectSVal);

        if (const auto K = getNullableObjectStateKey(NullableObjectSVal)) {
          auto PtrSVal = State->getSVal(CC->getArg(0), LCtx);
          dump_line(PtrSVal, "nullable poitner sval");

          auto Constraint = getNullConstraint(PtrSVal, State);
          DUMP(llvm::outs() << "nullable pointer sval constraint: " << nullConstraintToString(Constraint) << "\n");

          State = dno::createConstraint(State, K, Constraint);
          if (const auto Sym = PtrSVal.getAsSymbol())
            State = dno::alias(State, Sym, K);
        }

        C.addTransition(State);

        DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
        return true;
      }
    }
  }

  if (const auto *OC = dyn_cast<CXXOperatorCallExpr>(CE.getOriginExpr())) {
    // check for nullable object assignment

    if (OC->getOperator() == OO_Arrow) {
      const auto *LHS = OC->getArg(0);
      if (LHS) {
        const auto *LHSRec = getCXXRecordFromType(LHS->getType());
        if (LHSRec && isNullableObject(LHSRec)) {
          DUMP(llvm::outs() << "-> on nullable object\n");

          auto LHSVal = State->getSVal(LHS, LCtx);
          dump_line(LHSVal, "nullable object -> receiver sval");

          if (const auto LHSKey = getNullableObjectStateKey(LHSVal)) {
            auto LHState = State->get<DNOConstraintMap>(LHSKey);
            if (LHState) {
              if (isNakedRawPointerType(OC->getType())) {
                auto ReturnSVal = SVB.conjureSymbolVal(CE.getOriginExpr(), LCtx, ReturnType, C.blockCount());
                dump_line(ReturnSVal, "return sval");

                auto Zero = SVB.makeZeroVal(OC->getType());
                auto Constraint = SVB.evalBinOp(State, BO_NE, ReturnSVal, Zero, C.getASTContext().BoolTy);
                State = State->assume(Constraint.castAs<DefinedOrUnknownSVal>(), *LHState == NullConstraint::IsNotNull);

                C.addTransition(State);

                DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
                return true;
              }
              if (const auto *TargetRec = getCXXRecordFromType(OC->getType())) {
                if (isNullableObject(TargetRec)) {

                }
              }
            }
          }
        }
      }
    }

    if (OC->getOperator() == OO_Equal) {
      const auto *LHS = OC->getArg(0);
      const auto *RHS = OC->getArg(1);
      if (LHS && RHS) {
        const auto *LHSRec = getCXXRecordFromType(LHS->getType());
        if (LHSRec && isNullableObject(LHSRec)) {
          DUMP(llvm::outs() << "assignment to nullable object\n");

          auto LHSVal = State->getSVal(LHS, LCtx);
          auto RHSVal = State->getSVal(RHS, LCtx);
          dump_line(LHSVal, "nullable object assignment lhs sval");
          dump_line(RHSVal, "nullable object assignment rhs sval");

          if (LHSVal == RHSVal) {
            DUMP(llvm::outs() << "nullable ojects are the same\n");
            DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
            return true;
          }

          const auto LHSKey = getNullableObjectStateKey(LHSVal);
          State = dno::breakAliases(State, LHSKey);

          if (RHSVal.isZeroConstant()) {
            DUMP(llvm::outs() << "rhs is: null\n");

            State = dno::createConstraint(State, LHSKey, NullConstraint::IsNull);
            DUMP(llvm::outs() << "set lhs to: null");

            C.addTransition(State);

            DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
            return true;
          }

          if (isNakedRawPointerType(RHS->getType())) {
            DUMP(llvm::outs() << "rhs is a nullable pointer\n");

            const auto RHSConstraint = getNullConstraint(RHSVal, State);
            DUMP(llvm::outs() << "rhs null constraint: " << nullConstraintToString(RHSConstraint) << "\n");

            State = dno::createConstraint(State, LHSKey, RHSConstraint);
            DUMP(llvm::outs() << "set lhs to rhs constrain\n");

            C.addTransition(State);

            DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
            return true;
          }

          const auto *RHSRec = getCXXRecordFromType(RHS->getType());
          if (RHSRec && isNullableObject(RHSRec)) {
            DUMP(llvm::outs() << "rhs is a nullable object\n");

            State = dno::alias(State, LHSKey, getNullableObjectStateKey(RHSVal));
            DUMP(llvm::outs() << "aliased lhs to rhs constraint\n");

            C.addTransition(State);

            DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
            return true;
          }
        }
      }
    }

    // check for nullable object comparisons
    if (OC->getOperator() == OO_EqualEqual || OC->getOperator() == OO_ExclaimEqual) {
      const auto *LHS = OC->getArg(0);
      const auto *RHS = OC->getArg(1);
      if (LHS && RHS) {
        const auto *LHSRec = getCXXRecordFromType(LHS->getType());
        const auto *RHSRec = getCXXRecordFromType(RHS->getType());
        if ((LHSRec && isNullableObject(LHSRec)) || (RHSRec && isNullableObject(RHSRec))) {
          auto LHSVal = State->getSVal(LHS, LCtx);
          auto RHSVal = State->getSVal(RHS, LCtx);
          dump_line(LHSVal, "nullable object comparison lhs");
          dump_line(RHSVal, "nullable object comparison rhs");

          if (LHSVal == RHSVal) {
            DUMP(llvm::outs() << "nullable ojects are the same\n");

            auto TrueVal = SVB.makeTruthVal(OC->getOperator() == OO_EqualEqual, ReturnType);
            State = State->BindExpr(CE.getOriginExpr(), LCtx, TrueVal);
            C.addTransition(State);

            DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
            return true;
          }

          auto IsTrue = SVB.makeTruthVal(true, ReturnType);
          auto IsFalse = SVB.makeTruthVal(false, ReturnType);

          auto IsTrueState = State->BindExpr(CE.getOriginExpr(), LCtx, IsTrue);
          auto IsFalseState = State->BindExpr(CE.getOriginExpr(), LCtx, IsFalse);

          if (LHSVal.isZeroConstant()) {
            assert(!RHSVal.isZeroConstant() && "both nullable object args to comparison cannot be 0");
            DUMP(llvm::outs() << "lhs is: null\n");

            if (const auto RHSKey = getNullableObjectStateKey(RHSVal)) {
              auto RHState = State->get<DNOConstraintMap>(RHSKey);
              if (!RHState) {
                DUMP(llvm::outs() << "rhs state is not recored; creating unknown\n");
                IsTrueState = dno::createConstraint(IsTrueState, RHSKey, NullConstraint::Unknown);
                IsFalseState = dno::createConstraint(IsFalseState, RHSKey, NullConstraint::Unknown);
                RHState = IsTrueState->get<DNOConstraintMap>(RHSKey);
                assert(RHState && "rhs state does not exist after creation");
              }
              if (*RHState == NullConstraint::Unknown) {
                DUMP(llvm::outs() << "rhs state is unknown\n");

                const auto IsTrueConstraint = OC->getOperator() == OO_EqualEqual ? NullConstraint::IsNull : NullConstraint::IsNotNull;
                const auto IsFalseConstraint = negateNullConstraint(IsTrueConstraint);

                IsTrueState = dno::updateConstraint(IsTrueState, RHSKey, IsTrueConstraint);
                IsFalseState = dno::updateConstraint(IsFalseState, RHSKey, IsFalseConstraint);

                C.addTransition(IsTrueState);
                C.addTransition(IsFalseState);

                DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
                return true;
              }

              if (*RHState == NullConstraint::IsNull) {
                DUMP(llvm::outs() << "rhs state is: null\n");

                C.addTransition(OC->getOperator() == OO_EqualEqual ? IsTrueState : IsFalseState);
              }

              if (*RHState == NullConstraint::IsNotNull) {
                DUMP(llvm::outs() << "rhs state is: not-null\n");

                C.addTransition(OC->getOperator() == OO_EqualEqual ? IsFalseState : IsTrueState);
              }

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }
          }
          // TODO! Merge with above
          if (RHSVal.isZeroConstant()) {
            assert(!LHSVal.isZeroConstant() && "both nullable object args to comparison cannot be 0");
            DUMP(llvm::outs() << "rhs is: null\n");

            if (const auto LHSKey = getNullableObjectStateKey(LHSVal)) {
              auto LHState = State->get<DNOConstraintMap>(LHSKey);
              if (!LHState) {
                DUMP(llvm::outs() << "lhs state is not recored; creating unknown\n");
                IsTrueState = dno::createConstraint(IsTrueState, LHSKey, NullConstraint::Unknown);
                IsFalseState = dno::createConstraint(IsFalseState, LHSKey, NullConstraint::Unknown);
                LHState = IsTrueState->get<DNOConstraintMap>(LHSKey);
                assert(LHState && "lhs state does not exist after creation");
              }
              if (*LHState== NullConstraint::Unknown) {
                DUMP(llvm::outs() << "lhs state is unknown\n");

                const auto IsTrueConstraint = OC->getOperator() == OO_EqualEqual ? NullConstraint::IsNull : NullConstraint::IsNotNull;
                const auto IsFalseConstraint = negateNullConstraint(IsTrueConstraint);

                IsTrueState = dno::updateConstraint(IsTrueState, LHSKey, IsTrueConstraint);
                IsFalseState = dno::updateConstraint(IsFalseState, LHSKey, IsFalseConstraint);

                C.addTransition(IsTrueState);
                C.addTransition(IsFalseState);

                DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
                return true;
              }

              if (*LHState == NullConstraint::IsNull) {
                DUMP(llvm::outs() << "lhs state is: null\n");

                C.addTransition(OC->getOperator() == OO_EqualEqual ? IsTrueState : IsFalseState);
              }

              if (*LHState == NullConstraint::IsNotNull) {
                DUMP(llvm::outs() << "lhs state is: not-null\n");

                C.addTransition(OC->getOperator() == OO_EqualEqual ? IsFalseState : IsTrueState);
              }

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }
          }

          // TODO! Move Keys and key checks to top
          auto LHSKey = getNullableObjectStateKey(LHSVal);
          auto RHSKey = getNullableObjectStateKey(RHSVal);
          if (!LHSKey) DUMP(llvm::outs() << "no key for lhs\n");
          if (!RHSKey) DUMP(llvm::outs() << "no key for rhs\n");

          if (LHSKey && RHSKey) {
            auto LHState = State->get<DNOConstraintMap>(LHSKey);
            auto RHState = State->get<DNOConstraintMap>(RHSKey);

            if (!LHState) {
              DUMP(llvm::outs() << "lhs is not recored; creating unknown\n");
              IsTrueState = dno::createConstraint(IsTrueState, LHSKey, NullConstraint::Unknown);
              IsFalseState = dno::createConstraint(IsFalseState, LHSKey, NullConstraint::Unknown);
              LHState = IsTrueState->get<DNOConstraintMap>(LHSKey);
              assert(LHState && "lhs state does not exist after creation");
            }
            if (!RHState) {
              DUMP(llvm::outs() << "rhs is not recored; creating unknown\n");
              IsTrueState = dno::createConstraint(IsTrueState, RHSKey, NullConstraint::Unknown);
              IsFalseState = dno::createConstraint(IsFalseState, RHSKey, NullConstraint::Unknown);
              RHState = IsTrueState->get<DNOConstraintMap>(RHSKey);
              assert(RHState && "lhs state does not exist after creation");
            }

            if (*LHState == NullConstraint::Unknown && *RHState == NullConstraint::Unknown) {
              DUMP(llvm::outs() << "lhs and rhs are: unknown\n");

              // in case of equality we want them aliased
              // in case of inequality we do not:
              // a != b && b == nulptr !=> a != nullptr
              // (inequality is not transtitive)
              if (OC->getOperator() == OO_EqualEqual) {
                IsTrueState = dno::addAlias(IsTrueState, LHSKey, RHSKey);
              } else {
                IsFalseState = dno::addAlias(IsFalseState, LHSKey, RHSKey);
              }

              C.addTransition(IsTrueState);
              C.addTransition(IsFalseState);

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }

            if (*LHState == NullConstraint::Unknown && *RHState != NullConstraint::Unknown) {
              DUMP(llvm::outs() << "lhs is: unknown, rhs is: constrained\n");

              // we update the lhs constraint to be the same as the rhs in case of equality
              // in case of inequality we update the constraint to the opposite of rhs
              const auto IsTrueConstraint = OC->getOperator() == OO_EqualEqual ? *RHState : negateNullConstraint(*RHState);
              const auto IsFalseConstraint = negateNullConstraint(IsTrueConstraint);

              IsTrueState = dno::updateConstraint(IsTrueState, LHSKey, IsTrueConstraint);
              IsFalseState = dno::updateConstraint(IsFalseState, LHSKey, IsFalseConstraint);

              C.addTransition(IsTrueState);
              C.addTransition(IsFalseState);

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }

            if (*LHState != NullConstraint::Unknown && *RHState == NullConstraint::Unknown) {
              DUMP(llvm::outs() << "lhs is: constrained, rhs is: unknown\n");

              // we update the rhs constraint to be the same as the lhs in case of equality
              // in case of inequality we update the constraint to the opposite of lhs
              const auto IsTrueConstraint = OC->getOperator() == OO_EqualEqual ? *LHState : negateNullConstraint(*LHState);
              const auto IsFalseConstraint = negateNullConstraint(IsTrueConstraint);

              IsTrueState = dno::updateConstraint(IsTrueState, RHSKey, IsTrueConstraint);
              IsFalseState = dno::updateConstraint(IsFalseState, RHSKey, IsFalseConstraint);

              C.addTransition(IsTrueState);
              C.addTransition(IsFalseState);

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }

            if (*LHState != NullConstraint::Unknown && *RHState != NullConstraint::Unknown) {
              DUMP(llvm::outs() << "lhs is: constrained, rhs is: contrained\n");

              // the truth can be fully detemined here and we need not split the state
              // in case of equality the result is true iff lhs and rhs have the same constrained
              // in case of inequality the result is true iff their constraints differ

              const bool IsTrue = OC->getOperator() == OO_EqualEqual ? *LHState == *RHState : *LHState != *RHState;
              const auto Result = SVB.makeTruthVal(IsTrue, ReturnType);

              State = State->BindExpr(CE.getOriginExpr(), LCtx, Result);

              C.addTransition(State);

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }
          }
        }
      }
    }
  }

  if (const auto *MC = dyn_cast<CXXMemberCallExpr>(CE.getOriginExpr())) {
    if (const auto *Callee = MC->getCalleeDecl()) {
      if (Callee->getFunctionType()->isConst()) {
        //FIXME include constexpr and consteval
        if (const auto *Conversion = dyn_cast<CXXConversionDecl>(Callee)) {
          // check for nullable-object conversion to bool
          if (isNullableObject(Conversion->getParent()) && Conversion->getCallResultType()->isBooleanType()) {
            DUMP(llvm::outs() << "conversion of nullable object to boolean\n");

            const auto *Target = MC->getImplicitObjectArgument();
            auto TargetSVal = State->getSVal(Target, LCtx);
            dump_line(TargetSVal, "nullable object sval");

            if (const auto TargetKey = getNullableObjectStateKey(TargetSVal)) {
              DUMP(llvm::outs() << "splitting state on bool conversion\n");

              auto IsNotNull = SVB.makeTruthVal(true, Conversion->getCallResultType());
              auto IsNull = SVB.makeTruthVal(false, Conversion->getCallResultType());

              auto IsNotNullState = State->BindExpr(CE.getOriginExpr(), LCtx, IsNotNull);
              IsNotNullState = dno::updateConstraint(IsNotNullState, TargetKey, NullConstraint::IsNotNull);
              C.addTransition(IsNotNullState);

              auto IsNullState = State->BindExpr(CE.getOriginExpr(), LCtx, IsNull);
              IsNullState = dno::updateConstraint(IsNullState, TargetKey, NullConstraint::IsNull);
              C.addTransition(IsNullState);

              DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
              return true;
            }
          }
          if (isNullableObject(Conversion->getParent()) && isNakedRawPointerType(Conversion->getCallResultType())) {
            DUMP(llvm::outs() << "conversion of nullable object to nullable pointer\n");

            const auto *Target = MC->getImplicitObjectArgument();
            auto TargetSVal = State->getSVal(Target, LCtx);
            dump_line(TargetSVal, "nullable object sval");

            auto ReturnSVal = SVB.conjureSymbolVal(CE.getOriginExpr(), LCtx, ReturnType, C.blockCount());
            State = State->BindExpr(CE.getOriginExpr(), LCtx, ReturnSVal);
            dump_line(ReturnSVal, "conjured return sval");

            if (auto RetSym = ReturnSVal.getAsSymbol()) {
              if (const auto TargetKey = getNullableObjectStateKey(TargetSVal)) {
                // record constraint alias between nullable object and nullable pointer

                State = dno::alias(State, RetSym, TargetKey);

                const auto TargetConstraint = State->get<DNOConstraintMap>(TargetKey);
                if (!TargetConstraint) {
                  State = dno::createConstraint(State, TargetKey, NullConstraint::Unknown);
                } else if (TargetConstraint && *TargetConstraint != NullConstraint::Unknown) {
                  State = State->assume(ReturnSVal, *TargetConstraint == NullConstraint::IsNotNull);
                }
              }
            }

            C.addTransition(State);

            DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
            return true;
          }
        }
      }
    }
  }

  if (const auto *Call = dyn_cast<CallExpr>(CE.getOriginExpr())) {
    if (Call->isCXX11ConstantExpr(C.getASTContext())) {
      DUMP(llvm::outs() << "call expression is constant\n");

      if (CE.getKind() >= CE_BEG_FUNCTION_CALLS && CE.getKind() <= CE_END_FUNCTION_CALLS) {
        const auto K = CallEventKey { State, cast<AnyFunctionCall>(CE) };
        const auto MV = State->get<ConstantCallEvents>(K);
        if (MV) {
          dump_line(**MV, "existing constant expr sval");
          State = State->BindExpr(CE.getOriginExpr(), LCtx, SVB.makeSymbolVal(*MV));
        } else {
          DUMP(llvm::outs() << "recording sval of constant call expression\n");

          auto ReturnSVal = SVB.conjureSymbolVal(CE.getOriginExpr(), LCtx, ReturnType, std::numeric_limits<unsigned int>::max());
          dump_line(ReturnSVal, "return sval");
          State = State->set<ConstantCallEvents>(K, ReturnSVal.getAsSymbol());
          State = State->BindExpr(CE.getOriginExpr(), LCtx, ReturnSVal);
        }

        C.addTransition(State);
        DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
        return true;
      }
    }
  }

  // conjure a return value
  auto ReturnSVal = SVB.conjureSymbolVal(CE.getOriginExpr(), LCtx, ReturnType, C.blockCount());
  dump_line(ReturnSVal, "return sval");
  State = State->BindExpr(CE.getOriginExpr(), LCtx, ReturnSVal);

  C.addTransition(State);

  // const auto *Call= dyn_cast_or_null<CallExpr>(CE.getOriginExpr());
  // if (!Call) {
  //   llvm::outs() << "not a CallExpr\n";
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return false;
  // }

  // llvm::outs() << "callee name: " << C.getCalleeName(Call) << "\n";

  // const auto* StdForwardIdent = &C.getASTContext().Idents.get("forward");
  // if (CE.getCalleeIdentifier() != StdForwardIdent) {
  //   llvm::outs() << "not modeled\n";
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return false;
  // }

  // // modelling std::forward when applied to nullable pointer lvalue/rvalue references
  // const auto *ArgExpr = CE.getArgExpr(0);
  // assert(ArgExpr && "std::forward missing argument expression");
  // const auto ArgExprType = ArgExpr->getType();
  // const bool IsNullableArg = ArgExprType->isPointerType() || (ArgExprType->isReferenceType() && ArgExprType->getPointeeType()->isPointerType());
  // if (!IsNullableArg) {
  //   llvm::outs() << "not modeled for non nullable pointers\n";
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return false;
  // }

  // ProgramStateRef State = C.getState();

  // auto ArgSVal = CE.getArgSVal(0).castAs<DefinedOrUnknownSVal>();
  // dump_line(ArgSVal, "arg sval");
  // auto ArgConstraint = getNullConstraint(ArgSVal, State);
  // llvm::outs() << "arg null constraint: " << nullConstraintToString(ArgConstraint) << "\n";

  // if (ArgConstraint == NullConstraint::Unknown) {
  //   llvm::outs() << "NULLABLE POINTERS\n\n";
  //   return false;
  // }

  // // no matter if std::forward produces an lvalue or rvalue
  // // the return must be the same SVal as the argument itself
  // State = State->BindExpr(Call, C.getLocationContext(), ArgSVal);

  // C.addTransition(State);

  DUMP(llvm::outs() << "NULLABLE POINTERS\n\n");
  return true;
}

void ento::registerNullablePointersChecker(CheckerManager &mgr) {
  mgr.registerChecker<NullablePointersChecker>();
}

bool ento::shouldRegisterNullablePointersChecker(const CheckerManager &mgr) {
  return true;
}
