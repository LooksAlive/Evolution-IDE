/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Interface Declarations                                                     *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace mlir {
namespace detail {
struct CallOpInterfaceInterfaceTraits {
  class Concept {
  public:
    virtual ~Concept() = default;
    virtual CallInterfaceCallable getCallableForCallee(::mlir::Operation *tablegen_opaque_val) const = 0;
    virtual Operation::operand_range getArgOperands(::mlir::Operation *tablegen_opaque_val) const = 0;
    virtual Operation *resolveCallable(::mlir::Operation *tablegen_opaque_val) const = 0;
  };
  template<typename ConcreteOp>
  class Model : public Concept {
  public:
    CallInterfaceCallable getCallableForCallee(::mlir::Operation *tablegen_opaque_val) const final {
      return (llvm::cast<ConcreteOp>(tablegen_opaque_val)).getCallableForCallee();
    }
    Operation::operand_range getArgOperands(::mlir::Operation *tablegen_opaque_val) const final {
      return (llvm::cast<ConcreteOp>(tablegen_opaque_val)).getArgOperands();
    }
    Operation *resolveCallable(::mlir::Operation *tablegen_opaque_val) const final {
      // If the callable isn't a value, lookup the symbol reference.
        CallInterfaceCallable callable = (llvm::cast<ConcreteOp>(tablegen_opaque_val)).getCallableForCallee();
        if (auto symbolRef = callable.dyn_cast<SymbolRefAttr>())
          return SymbolTable::lookupNearestSymbolFrom((llvm::cast<ConcreteOp>(tablegen_opaque_val)), symbolRef);
        return callable.get<Value>().getDefiningOp();
    }
  };
};
} // end namespace detail
class CallOpInterface : public ::mlir::OpInterface<CallOpInterface, detail::CallOpInterfaceInterfaceTraits> {
public:
  using ::mlir::OpInterface<CallOpInterface, detail::CallOpInterfaceInterfaceTraits>::OpInterface;
  template <typename ConcreteOp>
  struct CallOpInterfaceTrait : public ::mlir::OpInterface<CallOpInterface, detail::CallOpInterfaceInterfaceTraits>::Trait<ConcreteOp> {
  };
  template <typename ConcreteOp>
  struct Trait : public CallOpInterfaceTrait<ConcreteOp> {};
  CallInterfaceCallable getCallableForCallee();
  Operation::operand_range getArgOperands();
  Operation *resolveCallable();
};
} // namespace mlir
namespace mlir {
namespace detail {
struct CallableOpInterfaceInterfaceTraits {
  class Concept {
  public:
    virtual ~Concept() = default;
    virtual Region *getCallableRegion(::mlir::Operation *tablegen_opaque_val) const = 0;
    virtual ArrayRef<Type> getCallableResults(::mlir::Operation *tablegen_opaque_val) const = 0;
  };
  template<typename ConcreteOp>
  class Model : public Concept {
  public:
    Region *getCallableRegion(::mlir::Operation *tablegen_opaque_val) const final {
      return (llvm::cast<ConcreteOp>(tablegen_opaque_val)).getCallableRegion();
    }
    ArrayRef<Type> getCallableResults(::mlir::Operation *tablegen_opaque_val) const final {
      return (llvm::cast<ConcreteOp>(tablegen_opaque_val)).getCallableResults();
    }
  };
};
} // end namespace detail
class CallableOpInterface : public ::mlir::OpInterface<CallableOpInterface, detail::CallableOpInterfaceInterfaceTraits> {
public:
  using ::mlir::OpInterface<CallableOpInterface, detail::CallableOpInterfaceInterfaceTraits>::OpInterface;
  template <typename ConcreteOp>
  struct CallableOpInterfaceTrait : public ::mlir::OpInterface<CallableOpInterface, detail::CallableOpInterfaceInterfaceTraits>::Trait<ConcreteOp> {
  };
  template <typename ConcreteOp>
  struct Trait : public CallableOpInterfaceTrait<ConcreteOp> {};
  Region *getCallableRegion();
  ArrayRef<Type> getCallableResults();
};
} // namespace mlir
