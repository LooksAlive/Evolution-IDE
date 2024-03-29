if (auto op = dyn_cast<::mlir::ROCDL::BarrierOp>(opInst)) {

    llvm::LLVMContext &llvmContext = builder.getContext();
    builder.CreateFence(llvm::AtomicOrdering::Release,
                        llvmContext.getOrInsertSyncScopeID("workgroup"));
    createIntrinsicCall(builder, llvm::Intrinsic::amdgcn_s_barrier);
    builder.CreateFence(llvm::AtomicOrdering::Acquire,
                        llvmContext.getOrInsertSyncScopeID("workgroup"));
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::BlockDimXOp>(opInst)) {
valueMapping[op.res()] = createDeviceFunctionCall(builder, "__ockl_get_local_size", 0);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::BlockDimYOp>(opInst)) {
valueMapping[op.res()] = createDeviceFunctionCall(builder, "__ockl_get_local_size", 1);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::BlockDimZOp>(opInst)) {
valueMapping[op.res()] = createDeviceFunctionCall(builder, "__ockl_get_local_size", 2);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::BlockIdXOp>(opInst)) {
valueMapping[op.res()] = createIntrinsicCall(builder,llvm::Intrinsic::amdgcn_workgroup_id_x);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::BlockIdYOp>(opInst)) {
valueMapping[op.res()] = createIntrinsicCall(builder,llvm::Intrinsic::amdgcn_workgroup_id_y);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::BlockIdZOp>(opInst)) {
valueMapping[op.res()] = createIntrinsicCall(builder,llvm::Intrinsic::amdgcn_workgroup_id_z);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::GridDimXOp>(opInst)) {
valueMapping[op.res()] = createDeviceFunctionCall(builder, "__ockl_get_global_size", 0);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::GridDimYOp>(opInst)) {
valueMapping[op.res()] = createDeviceFunctionCall(builder, "__ockl_get_global_size", 1);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::GridDimZOp>(opInst)) {
valueMapping[op.res()] = createDeviceFunctionCall(builder, "__ockl_get_global_size", 2);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::MubufLoadOp>(opInst)) {

      valueMapping[op.res()] = createIntrinsicCall(builder,
          llvm::Intrinsic::amdgcn_buffer_load, {valueMapping.lookup(op.rsrc()), valueMapping.lookup(op.vindex()), valueMapping.lookup(op.offset()), valueMapping.lookup(op.glc()), 
          valueMapping.lookup(op.slc())}, {convertType(op.getResult().getType().cast<LLVM::LLVMType>())});
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::MubufStoreOp>(opInst)) {

    auto vdataType = convertType(op.vdata().getType().cast<LLVM::LLVMType>());
    createIntrinsicCall(builder,
          llvm::Intrinsic::amdgcn_buffer_store, {valueMapping.lookup(op.vdata()), valueMapping.lookup(op.rsrc()), valueMapping.lookup(op.vindex()),
          valueMapping.lookup(op.offset()), valueMapping.lookup(op.glc()), valueMapping.lookup(op.slc())}, {vdataType});
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::ThreadIdXOp>(opInst)) {
valueMapping[op.res()] = createIntrinsicCall(builder,llvm::Intrinsic::amdgcn_workitem_id_x);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::ThreadIdYOp>(opInst)) {
valueMapping[op.res()] = createIntrinsicCall(builder,llvm::Intrinsic::amdgcn_workitem_id_y);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::ThreadIdZOp>(opInst)) {
valueMapping[op.res()] = createIntrinsicCall(builder,llvm::Intrinsic::amdgcn_workitem_id_z);
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_16x16x16f16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_16x16x16f16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_16x16x1f32>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_16x16x1f32,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_16x16x2bf16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_16x16x2bf16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_16x16x4f16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_16x16x4f16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_16x16x4f32>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_16x16x4f32,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_16x16x8bf16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_16x16x8bf16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_32x32x1f32>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_32x32x1f32,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_32x32x2bf16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_32x32x2bf16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_32x32x2f32>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_32x32x2f32,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_32x32x4bf16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_32x32x4bf16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_32x32x4f16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_32x32x4f16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_32x32x8f16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_32x32x8f16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_4x4x1f32>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_4x4x1f32,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_4x4x2bf16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_4x4x2bf16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_f32_4x4x4f16>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_f32_4x4x4f16,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_i32_16x16x16i8>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_i32_16x16x16i8,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_i32_16x16x4i8>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_i32_16x16x4i8,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_i32_32x32x4i8>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_i32_32x32x4i8,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_i32_32x32x8i8>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_i32_32x32x8i8,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
if (auto op = dyn_cast<::mlir::ROCDL::mfma_i32_4x4x4i8>(opInst)) {

    llvm::Module *module = builder.GetInsertBlock()->getModule();
    llvm::Function *fn = llvm::Intrinsic::getDeclaration(
        module,
        llvm::Intrinsic::amdgcn_mfma_i32_4x4x4i8,
        { 
        });
    auto operands = lookupValues(opInst.getOperands());
    valueMapping[op.res()] = builder.CreateCall(fn, operands);
  
  return success();
}
