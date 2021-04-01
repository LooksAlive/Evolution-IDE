/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Struct Utility Declarations                                                *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace mlir {
namespace spirv {

// DictionaryAttr with field(s): 'local_size' (each field having its own constraints)
class EntryPointABIAttr : public ::mlir::DictionaryAttr {
public:
  using ::mlir::DictionaryAttr::DictionaryAttr;
  static bool classof(::mlir::Attribute attr);
  static EntryPointABIAttr get(
      ::mlir::DenseIntElementsAttr local_size,
      ::mlir::MLIRContext* context);

  ::mlir::DenseIntElementsAttr local_size() const;
};

} // namespace mlir
} // namespace spirv
namespace mlir {
namespace spirv {

// DictionaryAttr with field(s): 'max_compute_shared_memory_size', 'max_compute_workgroup_invocations', 'max_compute_workgroup_size', 'subgroup_size' (each field having its own constraints)
class ResourceLimitsAttr : public ::mlir::DictionaryAttr {
public:
  using ::mlir::DictionaryAttr::DictionaryAttr;
  static bool classof(::mlir::Attribute attr);
  static ResourceLimitsAttr get(
      ::mlir::IntegerAttr max_compute_shared_memory_size,
      ::mlir::IntegerAttr max_compute_workgroup_invocations,
      ::mlir::DenseIntElementsAttr max_compute_workgroup_size,
      ::mlir::IntegerAttr subgroup_size,
      ::mlir::MLIRContext* context);

  ::mlir::IntegerAttr max_compute_shared_memory_size() const;
  ::mlir::IntegerAttr max_compute_workgroup_invocations() const;
  ::mlir::DenseIntElementsAttr max_compute_workgroup_size() const;
  ::mlir::IntegerAttr subgroup_size() const;
};

} // namespace mlir
} // namespace spirv
