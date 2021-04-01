/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Struct Utility Definitions                                                 *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace mlir {
namespace spirv {
EntryPointABIAttr EntryPointABIAttr::get(
    ::mlir::DenseIntElementsAttr local_size,
    ::mlir::MLIRContext* context) {
  ::llvm::SmallVector<::mlir::NamedAttribute, 1> fields;

  assert(local_size);
  auto local_size_id = ::mlir::Identifier::get("local_size", context);
  fields.emplace_back(local_size_id, local_size);

  ::mlir::Attribute dict = ::mlir::DictionaryAttr::get(fields, context);
  return dict.dyn_cast<EntryPointABIAttr>();
}

bool EntryPointABIAttr::classof(::mlir::Attribute attr) {
  if (!attr)
    return false;
  auto derived = attr.dyn_cast<::mlir::DictionaryAttr>();
  if (!derived)
    return false;
  int num_absent_attrs = 0;

  auto local_size = derived.get("local_size");
  if (!local_size || !(((local_size.isa<::mlir::DenseIntElementsAttr>())) && ((local_size.cast<::mlir::DenseIntElementsAttr>().getType().getElementType().isSignlessInteger(32)))))
    return false;

  return derived.size() + num_absent_attrs == 1;
}

::mlir::DenseIntElementsAttr EntryPointABIAttr::local_size() const {
  auto derived = this->cast<::mlir::DictionaryAttr>();
  auto local_size = derived.get("local_size");
  assert(local_size && "attribute not found.");
  assert(local_size.isa<::mlir::DenseIntElementsAttr>() && "incorrect Attribute type found.");
  return local_size.cast<::mlir::DenseIntElementsAttr>();
}
} // namespace spirv
} // namespace mlir
namespace mlir {
namespace spirv {
ResourceLimitsAttr ResourceLimitsAttr::get(
    ::mlir::IntegerAttr max_compute_shared_memory_size,
    ::mlir::IntegerAttr max_compute_workgroup_invocations,
    ::mlir::DenseIntElementsAttr max_compute_workgroup_size,
    ::mlir::IntegerAttr subgroup_size,
    ::mlir::MLIRContext* context) {
  ::llvm::SmallVector<::mlir::NamedAttribute, 4> fields;

  if (max_compute_shared_memory_size) {
    auto max_compute_shared_memory_size_id = ::mlir::Identifier::get("max_compute_shared_memory_size", context);
    fields.emplace_back(max_compute_shared_memory_size_id, max_compute_shared_memory_size);
  }

  if (max_compute_workgroup_invocations) {
    auto max_compute_workgroup_invocations_id = ::mlir::Identifier::get("max_compute_workgroup_invocations", context);
    fields.emplace_back(max_compute_workgroup_invocations_id, max_compute_workgroup_invocations);
  }

  if (max_compute_workgroup_size) {
    auto max_compute_workgroup_size_id = ::mlir::Identifier::get("max_compute_workgroup_size", context);
    fields.emplace_back(max_compute_workgroup_size_id, max_compute_workgroup_size);
  }

  if (subgroup_size) {
    auto subgroup_size_id = ::mlir::Identifier::get("subgroup_size", context);
    fields.emplace_back(subgroup_size_id, subgroup_size);
  }

  ::mlir::Attribute dict = ::mlir::DictionaryAttr::get(fields, context);
  return dict.dyn_cast<ResourceLimitsAttr>();
}

bool ResourceLimitsAttr::classof(::mlir::Attribute attr) {
  if (!attr)
    return false;
  auto derived = attr.dyn_cast<::mlir::DictionaryAttr>();
  if (!derived)
    return false;
  int num_absent_attrs = 0;

  auto max_compute_shared_memory_size = derived.get("max_compute_shared_memory_size");
  if (!max_compute_shared_memory_size)
    ++num_absent_attrs;
  else if (!(((max_compute_shared_memory_size.isa<::mlir::IntegerAttr>())) && ((max_compute_shared_memory_size.cast<::mlir::IntegerAttr>().getType().isSignlessInteger(32)))))
    return false;

  auto max_compute_workgroup_invocations = derived.get("max_compute_workgroup_invocations");
  if (!max_compute_workgroup_invocations)
    ++num_absent_attrs;
  else if (!(((max_compute_workgroup_invocations.isa<::mlir::IntegerAttr>())) && ((max_compute_workgroup_invocations.cast<::mlir::IntegerAttr>().getType().isSignlessInteger(32)))))
    return false;

  auto max_compute_workgroup_size = derived.get("max_compute_workgroup_size");
  if (!max_compute_workgroup_size)
    ++num_absent_attrs;
  else if (!(((max_compute_workgroup_size.isa<::mlir::DenseIntElementsAttr>())) && ((max_compute_workgroup_size.cast<::mlir::DenseIntElementsAttr>().getType().getElementType().isSignlessInteger(32)))))
    return false;

  auto subgroup_size = derived.get("subgroup_size");
  if (!subgroup_size)
    ++num_absent_attrs;
  else if (!(((subgroup_size.isa<::mlir::IntegerAttr>())) && ((subgroup_size.cast<::mlir::IntegerAttr>().getType().isSignlessInteger(32)))))
    return false;

  return derived.size() + num_absent_attrs == 4;
}

::mlir::IntegerAttr ResourceLimitsAttr::max_compute_shared_memory_size() const {
  auto derived = this->cast<::mlir::DictionaryAttr>();
  auto max_compute_shared_memory_size = derived.get("max_compute_shared_memory_size");
  if (!max_compute_shared_memory_size) {
    ::mlir::Builder builder(getContext());
    return builder.getIntegerAttr(builder.getIntegerType(32), 16384);
  }
  assert(max_compute_shared_memory_size.isa<::mlir::IntegerAttr>() && "incorrect Attribute type found.");
  return max_compute_shared_memory_size.cast<::mlir::IntegerAttr>();
}

::mlir::IntegerAttr ResourceLimitsAttr::max_compute_workgroup_invocations() const {
  auto derived = this->cast<::mlir::DictionaryAttr>();
  auto max_compute_workgroup_invocations = derived.get("max_compute_workgroup_invocations");
  if (!max_compute_workgroup_invocations) {
    ::mlir::Builder builder(getContext());
    return builder.getIntegerAttr(builder.getIntegerType(32), 128);
  }
  assert(max_compute_workgroup_invocations.isa<::mlir::IntegerAttr>() && "incorrect Attribute type found.");
  return max_compute_workgroup_invocations.cast<::mlir::IntegerAttr>();
}

::mlir::DenseIntElementsAttr ResourceLimitsAttr::max_compute_workgroup_size() const {
  auto derived = this->cast<::mlir::DictionaryAttr>();
  auto max_compute_workgroup_size = derived.get("max_compute_workgroup_size");
  if (!max_compute_workgroup_size) {
    ::mlir::Builder builder(getContext());
    return ::mlir::DenseElementsAttr::get(::mlir::RankedTensorType::get({}, builder.getIntegerType(32)), ::llvm::makeArrayRef({128, 128, 64})).cast<::mlir::DenseIntElementsAttr>();
  }
  assert(max_compute_workgroup_size.isa<::mlir::DenseIntElementsAttr>() && "incorrect Attribute type found.");
  return max_compute_workgroup_size.cast<::mlir::DenseIntElementsAttr>();
}

::mlir::IntegerAttr ResourceLimitsAttr::subgroup_size() const {
  auto derived = this->cast<::mlir::DictionaryAttr>();
  auto subgroup_size = derived.get("subgroup_size");
  if (!subgroup_size) {
    ::mlir::Builder builder(getContext());
    return builder.getIntegerAttr(builder.getIntegerType(32), 0x7FFFFFFF);
  }
  assert(subgroup_size.isa<::mlir::IntegerAttr>() && "incorrect Attribute type found.");
  return subgroup_size.cast<::mlir::IntegerAttr>();
}
} // namespace spirv
} // namespace mlir
