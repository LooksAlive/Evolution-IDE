
    ArrayAttr MatmulOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr MatmulOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2;
    bindDims(context, d0, d1, d2);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(3, 0, {d0, d2}, context), 
	AffineMap::get(3, 0, {d2, d1}, context), 
	AffineMap::get(3, 0, {d0, d1}, context) });
  }
  void MatmulOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr MatvecOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr MatvecOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1;
    bindDims(context, d0, d1);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(2, 0, {d0, d1}, context), 
	AffineMap::get(2, 0, {d1}, context), 
	AffineMap::get(2, 0, {d0}, context) });
  }
  void MatvecOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr VecmatOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr VecmatOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1;
    bindDims(context, d0, d1);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(2, 0, {d1}, context), 
	AffineMap::get(2, 0, {d1, d0}, context), 
	AffineMap::get(2, 0, {d0}, context) });
  }
  void VecmatOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr DotOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr DotOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0;
    bindDims(context, d0);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(1, 0, {d0}, context), 
	AffineMap::get(1, 0, {d0}, context), 
	AffineMap::get(1, 0, {}, context) });
  }
  void DotOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr BatchMatmulOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr BatchMatmulOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3;
    bindDims(context, d0, d1, d2, d3);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(4, 0, {d0, d1, d3}, context), 
	AffineMap::get(4, 0, {d0, d3, d2}, context), 
	AffineMap::get(4, 0, {d0, d1, d2}, context) });
  }
  void BatchMatmulOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvWOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvWOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1;
    bindDims(context, d0, d1);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(2, 0, {d0 + d1}, context), 
	AffineMap::get(2, 0, {d1}, context), 
	AffineMap::get(2, 0, {d0}, context) });
  }
  void ConvWOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvNWCOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getParallelIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvNWCOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4;
    bindDims(context, d0, d1, d2, d3, d4);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(5, 0, {d0, d1 + d3, d4}, context), 
	AffineMap::get(5, 0, {d2, d3, d4}, context), 
	AffineMap::get(5, 0, {d0, d1, d2}, context) });
  }
  void ConvNWCOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvNCWOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getParallelIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvNCWOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4;
    bindDims(context, d0, d1, d2, d3, d4);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(5, 0, {d0, d4, d2 + d3}, context), 
	AffineMap::get(5, 0, {d1, d4, d3}, context), 
	AffineMap::get(5, 0, {d0, d1, d2}, context) });
  }
  void ConvNCWOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvHWOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvHWOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3;
    bindDims(context, d0, d1, d2, d3);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(4, 0, {d0 + d2, d1 + d3}, context), 
	AffineMap::get(4, 0, {d2, d3}, context), 
	AffineMap::get(4, 0, {d0, d1}, context) });
  }
  void ConvHWOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvNHWCOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getParallelIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvNHWCOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4, d5, d6;
    bindDims(context, d0, d1, d2, d3, d4, d5, d6);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(7, 0, {d0, d1 + d4, d2 + d5, d6}, context), 
	AffineMap::get(7, 0, {d3, d4, d5, d6}, context), 
	AffineMap::get(7, 0, {d0, d1, d2, d3}, context) });
  }
  void ConvNHWCOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvNCHWOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getParallelIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvNCHWOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4, d5, d6;
    bindDims(context, d0, d1, d2, d3, d4, d5, d6);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(7, 0, {d0, d6, d2 + d4, d3 + d5}, context), 
	AffineMap::get(7, 0, {d1, d6, d4, d5}, context), 
	AffineMap::get(7, 0, {d0, d1, d2, d3}, context) });
  }
  void ConvNCHWOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvDHWOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvDHWOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4, d5;
    bindDims(context, d0, d1, d2, d3, d4, d5);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(6, 0, {d0 + d3, d1 + d4, d2 + d5}, context), 
	AffineMap::get(6, 0, {d3, d4, d5}, context), 
	AffineMap::get(6, 0, {d0, d1, d2}, context) });
  }
  void ConvDHWOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvNDHWCOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getParallelIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvNDHWCOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4, d5, d6, d7, d8;
    bindDims(context, d0, d1, d2, d3, d4, d5, d6, d7, d8);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(9, 0, {d0, d1 + d5, d2 + d6, d3 + d7, d8}, context), 
	AffineMap::get(9, 0, {d4, d5, d6, d7, d8}, context), 
	AffineMap::get(9, 0, {d0, d1, d2, d3, d4}, context) });
  }
  void ConvNDHWCOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }

    ArrayAttr ConvNCDHWOp::iterator_types() {
      return Builder(getContext()).getStrArrayAttr(SmallVector<StringRef, 8>{ getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getParallelIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getReductionIteratorTypeName(), getParallelIteratorTypeName() });
    }
  // This is temporary until we transition out of manually specified ops that
  // should be auto-generated with linalg-ods-gen.
  ArrayAttr ConvNCDHWOp::indexing_maps() {
    MLIRContext *context = getContext();
    AffineExpr d0, d1, d2, d3, d4, d5, d6, d7, d8;
    bindDims(context, d0, d1, d2, d3, d4, d5, d6, d7, d8);
    return Builder(context).getAffineMapArrayAttr({ 
	AffineMap::get(9, 0, {d0, d8, d2 + d5, d3 + d6, d4 + d7}, context), 
	AffineMap::get(9, 0, {d1, d8, d5, d6, d7}, context), 
	AffineMap::get(9, 0, {d0, d1, d2, d3, d4}, context) });
  }
  void ConvNCDHWOp::regionBuilder(Block &block) {
    using namespace edsc;
    using namespace intrinsics;
    auto args = block.getArguments();
    Value _0(args[0]), _1(args[1]), _2(args[2]);
    
    Value _4 = std_mulf(_0, _1);
    Value _5 = std_addf(_2, _4);
    (linalg_yield(ValueRange{ _5 }));
  }
