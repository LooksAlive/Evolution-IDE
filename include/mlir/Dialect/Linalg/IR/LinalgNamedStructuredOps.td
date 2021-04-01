  def MatmulOp : LinalgStructuredBase_Op<"matmul", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<MatmulOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<MatmulOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<MatmulOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def MatvecOp : LinalgStructuredBase_Op<"matvec", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<MatvecOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<MatvecOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<MatvecOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def VecmatOp : LinalgStructuredBase_Op<"vecmat", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<VecmatOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<VecmatOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<VecmatOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def DotOp : LinalgStructuredBase_Op<"dot", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<DotOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<DotOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<DotOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def BatchMatmulOp : LinalgStructuredBase_Op<"batch_matmul", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<BatchMatmulOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<BatchMatmulOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<BatchMatmulOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvWOp : LinalgStructuredBase_Op<"conv_1d", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvWOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvNWCOp : LinalgStructuredBase_Op<"conv_1d_nwc", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvNWCOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvNWCOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvNWCOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvNCWOp : LinalgStructuredBase_Op<"conv_1d_ncw", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvNCWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvNCWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvNCWOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvHWOp : LinalgStructuredBase_Op<"conv_2d", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvHWOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvNHWCOp : LinalgStructuredBase_Op<"conv_2d_nhwc", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvNHWCOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvNHWCOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvNHWCOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvNCHWOp : LinalgStructuredBase_Op<"conv_2d_nchw", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvNCHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvNCHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvNCHWOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvDHWOp : LinalgStructuredBase_Op<"conv_3d", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvDHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvDHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvDHWOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvNDHWCOp : LinalgStructuredBase_Op<"conv_3d_ndhwc", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvNDHWCOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvNDHWCOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvNDHWCOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
  def ConvNCDHWOp : LinalgStructuredBase_Op<"conv_3d_ncdhw", [
    NamedStructuredOpTrait,
    AttrSizedOperandSegments,
    SingleBlockImplicitTerminator<"YieldOp">]> {
      let arguments = (ins Variadic<AnyShaped>:$inputs,
                           Variadic<AnyMemRef>:$output_buffers,
                           Variadic<AnyRankedTensor>:$init_tensors);
      let results = (outs Variadic<AnyRankedTensor>:$result_tensors);
      let regions = (region AnyRegion:$region);

      let skipDefaultBuilders = 1;
      let builders = [ OpBuilder<
        "OpBuilder &b, OperationState &result, "
        "ValueRange inputs, ValueRange outputBuffers",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(0)}));
          buildNamedStructuredOpRegionAndAttributes<ConvNCDHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(),
            TypeRange());
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange inputs, ValueRange outputBuffers, ValueRange initTensors",
        [{
          result.addOperands(inputs);
          result.addOperands(outputBuffers);
          result.addOperands(initTensors);
          result.addTypes(resultTensorTypes);
          result.addAttribute(
            "operand_segment_sizes",
            b.getI32VectorAttr({static_cast<int32_t>(inputs.size()),
                                static_cast<int32_t>(outputBuffers.size()),
                                static_cast<int32_t>(initTensors.size())}));
          buildNamedStructuredOpRegionAndAttributes<ConvNCDHWOp>(
            b,
            result,
            TypeRange(inputs),
            TypeRange(outputBuffers),
            TypeRange(initTensors),
            resultTensorTypes);
        }]>, OpBuilder<
        "OpBuilder &b, OperationState &result, TypeRange resultTensorTypes,"
        "ValueRange operands, ArrayRef<NamedAttribute> attributes = {}",
        [{
          result.addOperands(operands);
          result.addAttributes(attributes);
          result.addTypes(resultTensorTypes);
          (void)result.addRegion();
        }]>
      ];
      let printer = [{ return ::printNamedStructuredOp(p, *this); }];
      let parser = [{ return ::parseNamedStructuredOp<ConvNCDHWOp>(parser, result); }];
      let verifier = [{ return ::verifyNamedStructuredOp(*this); }];
      let hasFolder = 1;
      let hasCanonicalizer = 1;

      let extraClassDeclaration = [{
        // Auto-generated.
        ArrayAttr iterator_types();
        ArrayAttr indexing_maps();
        static void regionBuilder(Block &block);

        // Generic methods.
        static unsigned getNumRegionArgs() { return 3; }
        std::string getLibraryCallName() {
          return generateLibraryCallName(getOperation());
        }
      }];
  }
