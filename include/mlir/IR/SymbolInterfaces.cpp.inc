/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Interface Definitions                                                      *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

StringRef mlir::SymbolOpInterface::getName() {
      return getImpl()->getName(getOperation());
  }
void mlir::SymbolOpInterface::setName(StringRef name) {
      return getImpl()->setName(getOperation(), name);
  }
mlir::SymbolTable::Visibility mlir::SymbolOpInterface::getVisibility() {
      return getImpl()->getVisibility(getOperation());
  }
void mlir::SymbolOpInterface::setVisibility(mlir::SymbolTable::Visibility vis) {
      return getImpl()->setVisibility(getOperation(), vis);
  }
bool mlir::SymbolOpInterface::isNested() {
      return getImpl()->isNested(getOperation());
  }
bool mlir::SymbolOpInterface::isPrivate() {
      return getImpl()->isPrivate(getOperation());
  }
bool mlir::SymbolOpInterface::isPublic() {
      return getImpl()->isPublic(getOperation());
  }
Optional<::mlir::SymbolTable::UseRange> mlir::SymbolOpInterface::getSymbolUses(Operation * from) {
      return getImpl()->getSymbolUses(getOperation(), from);
  }
bool mlir::SymbolOpInterface::symbolKnownUseEmpty(Operation * from) {
      return getImpl()->symbolKnownUseEmpty(getOperation(), from);
  }
LogicalResult mlir::SymbolOpInterface::replaceAllSymbolUses(StringRef newSymbol, Operation * from) {
      return getImpl()->replaceAllSymbolUses(getOperation(), newSymbol, from);
  }
bool mlir::SymbolOpInterface::isOptionalSymbol() {
      return getImpl()->isOptionalSymbol(getOperation());
  }
bool mlir::SymbolOpInterface::canDiscardOnUseEmpty() {
      return getImpl()->canDiscardOnUseEmpty(getOperation());
  }
