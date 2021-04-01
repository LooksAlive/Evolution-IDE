#ifndef GENERATED_DECISION_FOREST_MODEL_COMPLETIONMODEL_H
#define GENERATED_DECISION_FOREST_MODEL_COMPLETIONMODEL_H
#include <cstdint>
#include "llvm/Support/Compiler.h"

namespace clang {
namespace clangd {
class Example {
public:
  void setFilterLength(float V) { FilterLength = OrderEncode(V); }
  void setIsDeprecated(float V) { IsDeprecated = OrderEncode(V); }
  void setIsReservedName(float V) { IsReservedName = OrderEncode(V); }
  void setIsImplementationDetail(float V) { IsImplementationDetail = OrderEncode(V); }
  void setNumReferences(float V) { NumReferences = OrderEncode(V); }
  void setIsNameInContext(float V) { IsNameInContext = OrderEncode(V); }
  void setIsForbidden(float V) { IsForbidden = OrderEncode(V); }
  void setIsInBaseClass(float V) { IsInBaseClass = OrderEncode(V); }
  void setFileProximityDistance(float V) { FileProximityDistance = OrderEncode(V); }
  void setSemaFileProximityScore(float V) { SemaFileProximityScore = OrderEncode(V); }
  void setSymbolScopeDistance(float V) { SymbolScopeDistance = OrderEncode(V); }
  void setSemaSaysInScope(float V) { SemaSaysInScope = OrderEncode(V); }
  void setIsInstanceMember(float V) { IsInstanceMember = OrderEncode(V); }
  void setHadContextType(float V) { HadContextType = OrderEncode(V); }
  void setHadSymbolType(float V) { HadSymbolType = OrderEncode(V); }
  void setTypeMatchesPreferred(float V) { TypeMatchesPreferred = OrderEncode(V); }
  void setSymbolCategory(unsigned V) { SymbolCategory = 1 << V; }
  void setContextKind(unsigned V) { ContextKind = 1 << V; }
  void setScope(unsigned V) { Scope = 1 << V; }

private:
  uint32_t FilterLength = 0;
  uint32_t IsDeprecated = 0;
  uint32_t IsReservedName = 0;
  uint32_t IsImplementationDetail = 0;
  uint32_t NumReferences = 0;
  uint32_t IsNameInContext = 0;
  uint32_t IsForbidden = 0;
  uint32_t IsInBaseClass = 0;
  uint32_t FileProximityDistance = 0;
  uint32_t SemaFileProximityScore = 0;
  uint32_t SymbolScopeDistance = 0;
  uint32_t SemaSaysInScope = 0;
  uint32_t IsInstanceMember = 0;
  uint32_t HadContextType = 0;
  uint32_t HadSymbolType = 0;
  uint32_t TypeMatchesPreferred = 0;
  uint32_t SymbolCategory = 0;
  uint32_t ContextKind = 0;
  uint32_t Scope = 0;

  // Produces an integer that sorts in the same order as F.
  // That is: a < b <==> orderEncode(a) < orderEncode(b).
  static uint32_t OrderEncode(float F);
  friend float Evaluate(const Example&);
};

// The function may have large number of lines of code. MSAN
// build times out in such case.
LLVM_NO_SANITIZE("memory")
float Evaluate(const Example&);
} // namespace clangd
} // namespace clang
#endif // GENERATED_DECISION_FOREST_MODEL_COMPLETIONMODEL_H
