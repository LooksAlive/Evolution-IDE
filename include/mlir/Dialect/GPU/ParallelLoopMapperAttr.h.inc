/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Struct Utility Declarations                                                *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace mlir {
namespace gpu {

// DictionaryAttr with field(s): 'processor', 'map', 'bound' (each field having its own constraints)
class ParallelLoopDimMapping : public ::mlir::DictionaryAttr {
public:
  using ::mlir::DictionaryAttr::DictionaryAttr;
  static bool classof(::mlir::Attribute attr);
  static ParallelLoopDimMapping get(
      ::mlir::IntegerAttr processor,
      AffineMapAttr map,
      AffineMapAttr bound,
      ::mlir::MLIRContext* context);

  ::mlir::IntegerAttr processor() const;
  AffineMapAttr map() const;
  AffineMapAttr bound() const;
};

} // namespace mlir
} // namespace gpu
