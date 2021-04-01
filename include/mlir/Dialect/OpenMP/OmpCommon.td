def ProcBindKindmaster : StrEnumAttrCase<"master">;
def ProcBindKindclose : StrEnumAttrCase<"close">;
def ProcBindKindspread : StrEnumAttrCase<"spread">;
def ProcBindKind: StrEnumAttr<
  "ClauseProcBindKind",
  "ProcBindKind Clause",
  [ProcBindKindmaster,ProcBindKindclose,ProcBindKindspread]> {
    let cppNamespace = "::mlir::omp";
}
