#include "code_generation/codegen_visitor.h"
#include "parser/abstract_syntax_tree.h"
#include "tokens.h"
#include <iostream>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

using namespace compiler::ast;
using namespace compiler::parser::ast::node;

codegen_visitor_t::codegen_visitor_t() {
  context = std::make_unique<llvm::LLVMContext>();
  builder = std::make_unique<llvm::IRBuilder<>>(*context);
  module = std::make_unique<llvm::Module>("my_module", *context);
  last_value = nullptr;
  current_function = nullptr;
}

llvm::Value *codegen_visitor_t::log_error_v(const char *str) {
  std::cerr << "Error: " << str << std::endl;
  return nullptr;
}

llvm::Type *codegen_visitor_t::get_llvm_type(token_e type, int pointer_level) {
  llvm::Type *base_type = nullptr;

  // Map token types to LLVM types
  switch (type) {
  case token_e::tok_int:
    base_type = llvm::Type::getInt32Ty(*context);
    break;
  case token_e::tok_float:
    base_type = llvm::Type::getFloatTy(*context);
    break;
  case token_e::tok_char:
    base_type = llvm::Type::getInt8Ty(*context);
    break;
  case token_e::tok_bool:
    base_type = llvm::Type::getInt1Ty(*context);
    break;
  case token_e::tok_void:
    base_type = llvm::Type::getVoidTy(*context);
    break;
  default:
    base_type = llvm::Type::getInt32Ty(*context); // Default to int
    break;
  }

  // Handle pointer levels - use LLVM 21 opaque pointer API
  for (int i = 0; i < pointer_level; i++) {
    base_type = llvm::PointerType::get(*context, 0);
  }

  return base_type;
}

llvm::AllocaInst *codegen_visitor_t::create_entry_block_alloca(
    llvm::Function *func, const std::string &var_name, llvm::Type *type) {
  llvm::IRBuilder<> tmp_builder(&func->getEntryBlock(),
                                func->getEntryBlock().begin());
  return tmp_builder.CreateAlloca(type, nullptr, var_name);
}

void codegen_visitor_t::visit_literal_expr(literal_expr_t *t_literal_expr) {
  // Check if this is an identifier (variable reference)
  if (t_literal_expr->type == token_e::tok_id) {
    // This is a variable reference
    std::string var_name = std::visit(
        [](auto &&arg) -> std::string {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, char *>) {
            return std::string(arg);
          } else if constexpr (std::is_same_v<T, std::string>) {
            return arg;
          }
          return "";
        },
        t_literal_expr->value);

    // Look up the variable in the symbol table
    auto it = named_values.find(var_name);
    if (it != named_values.end()) {
      // Load its value
      llvm::AllocaInst *alloca = it->second;
      last_value =
          builder->CreateLoad(alloca->getAllocatedType(), alloca, var_name);
      return;
    } else {
      last_value = log_error_v("Unknown variable name");
      return;
    }
  }

  // Otherwise, handle as a literal value
  last_value = std::visit(
      [this](auto &&arg) -> llvm::Value * {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
          return llvm::ConstantInt::get(*context, llvm::APInt(32, arg, true));
        } else if constexpr (std::is_same_v<T, float>) {
          return llvm::ConstantFP::get(*context, llvm::APFloat(arg));
        } else if constexpr (std::is_same_v<T, double>) {
          return llvm::ConstantFP::get(*context, llvm::APFloat(arg));
        } else if constexpr (std::is_same_v<T, char>) {
          return llvm::ConstantInt::get(*context, llvm::APInt(8, arg, true));
        } else if constexpr (std::is_same_v<T, char *> ||
                             std::is_same_v<T, const std::string &>) {
          // String literal - create a global string constant
          return builder->CreateGlobalString(arg, "str");
        }
        return nullptr;
      },
      t_literal_expr->value);
}

void codegen_visitor_t::visit_binary_expr(binary_expr_t *t_binary_expr) {
  t_binary_expr->left->accept_visitor(this);
  llvm::Value *left = last_value;

  t_binary_expr->right->accept_visitor(this);
  llvm::Value *right = last_value;

  if (!left || !right) {
    last_value = nullptr;
    return;
  }

  // Handle different operators
  switch (t_binary_expr->op) {
  case token_e::tok_plus:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFAdd(left, right, "addtmp");
    } else {
      last_value = builder->CreateAdd(left, right, "addtmp");
    }
    break;
  case token_e::tok_minus:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFSub(left, right, "subtmp");
    } else {
      last_value = builder->CreateSub(left, right, "subtmp");
    }
    break;
  case token_e::tok_star:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFMul(left, right, "multmp");
    } else {
      last_value = builder->CreateMul(left, right, "multmp");
    }
    break;
  case token_e::tok_slash:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFDiv(left, right, "divtmp");
    } else {
      last_value = builder->CreateSDiv(left, right, "divtmp");
    }
    break;
  case token_e::tok_lt:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFCmpULT(left, right, "cmptmp");
    } else {
      last_value = builder->CreateICmpSLT(left, right, "cmptmp");
    }
    break;
  case token_e::tok_gt:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFCmpUGT(left, right, "cmptmp");
    } else {
      last_value = builder->CreateICmpSGT(left, right, "cmptmp");
    }
    break;
  case token_e::tok_leq:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFCmpULE(left, right, "cmptmp");
    } else {
      last_value = builder->CreateICmpSLE(left, right, "cmptmp");
    }
    break;
  case token_e::tok_geq:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFCmpUGE(left, right, "cmptmp");
    } else {
      last_value = builder->CreateICmpSGE(left, right, "cmptmp");
    }
    break;
  case token_e::tok_eq:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFCmpUEQ(left, right, "cmptmp");
    } else {
      last_value = builder->CreateICmpEQ(left, right, "cmptmp");
    }
    break;
  case token_e::tok_neq:
    if (left->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFCmpUNE(left, right, "cmptmp");
    } else {
      last_value = builder->CreateICmpNE(left, right, "cmptmp");
    }
    break;
  default:
    last_value = log_error_v("Invalid binary operator");
    break;
  }
}

void codegen_visitor_t::visit_unary_expr(unary_expr_t *t_unary_expr) {
  t_unary_expr->operand->accept_visitor(this);
  llvm::Value *operand = last_value;

  if (!operand) {
    last_value = nullptr;
    return;
  }

  switch (t_unary_expr->op) {
  case token_e::tok_minus:
    if (operand->getType()->isFloatingPointTy()) {
      last_value = builder->CreateFNeg(operand, "negtmp");
    } else {
      last_value = builder->CreateNeg(operand, "negtmp");
    }
    break;
  case token_e::tok_exclaimationmark:
    last_value = builder->CreateNot(operand, "nottmp");
    break;
  case token_e::tok_star: // Dereference
    // In LLVM 21 with opaque pointers, we need to know the element type
    // For now, assume int32 - you may need to track types in your AST
    last_value =
        builder->CreateLoad(llvm::Type::getInt32Ty(*context), operand, "deref");
    break;
  default:
    last_value = log_error_v("Invalid unary operator");
    break;
  }
}

void codegen_visitor_t::visit_variable(variable_t *t_variable) {
  llvm::Type *var_type =
      get_llvm_type(t_variable->type, t_variable->pointer_level);

  // Create an alloca for the variable
  llvm::AllocaInst *alloca = create_entry_block_alloca(
      current_function, t_variable->identifier, var_type);

  // Generate code for the initializer expression
  if (t_variable->p_expr) {
    t_variable->p_expr->accept_visitor(this);
    llvm::Value *init_val = last_value;

    if (init_val) {
      builder->CreateStore(init_val, alloca);
    }
  }

  // Store in the symbol table
  named_values[t_variable->identifier] = alloca;

  // Don't set last_value - variable declarations don't produce values
  last_value = nullptr;
}

void codegen_visitor_t::visit_assign_expr(assign_expr_t *t_assign_expr) {
  // Look up the variable
  llvm::AllocaInst *variable = named_values[t_assign_expr->identifier];
  if (!variable) {
    last_value = log_error_v("Unknown variable name");
    return;
  }

  // Generate code for the RHS
  t_assign_expr->right->accept_visitor(this);
  llvm::Value *val = last_value;

  if (!val) {
    return;
  }

  builder->CreateStore(val, variable);
  last_value = val;
}

void codegen_visitor_t::visit_call_expr(call_expr_t *t_call_expr) {
  // Look up the function in the module
  llvm::Function *callee = module->getFunction(t_call_expr->function_name);
  if (!callee) {
    last_value = log_error_v("Unknown function referenced");
    return;
  }

  // Check argument count
  if (callee->arg_size() != t_call_expr->arguments.size()) {
    last_value = log_error_v("Incorrect number of arguments passed");
    return;
  }

  // Generate code for arguments
  std::vector<llvm::Value *> args_v;
  for (auto *arg : t_call_expr->arguments) {
    arg->accept_visitor(this);
    args_v.push_back(last_value);
    if (!args_v.back()) {
      last_value = nullptr;
      return;
    }
  }

  last_value = builder->CreateCall(callee, args_v, "calltmp");
}

void codegen_visitor_t::visit_return(return_t *t_return) {
  if (t_return->p_expr) {
    t_return->p_expr->accept_visitor(this);
    builder->CreateRet(last_value);
  } else {
    builder->CreateRetVoid();
  }
}

void codegen_visitor_t::visit_block(block_t *t_block) {
  for (auto *stmt : t_block->statements) {
    stmt->accept_visitor(this);
  }
}

void codegen_visitor_t::visit_function(function_t *t_function) {
  // Create function type
  std::vector<llvm::Type *> param_types;
  if (t_function->parameter_type_pointer_level_tuple) {
    for (const auto &param :
         *(t_function->parameter_type_pointer_level_tuple)) {
      param_types.push_back(
          get_llvm_type(std::get<token_e>(param), std::get<int>(param)));
    }
  }

  llvm::Type *ret_type =
      get_llvm_type(t_function->type, t_function->pointer_level);
  llvm::FunctionType *ft =
      llvm::FunctionType::get(ret_type, param_types, false);

  // Create the function
  llvm::Function *func =
      llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                             t_function->identifier, module.get());

  // Create entry basic block
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", func);
  builder->SetInsertPoint(bb);

  // Set up for code generation
  current_function = func;
  named_values.clear();

  // Set argument names and create allocas
  unsigned idx = 0;
  for (auto &arg : func->args()) {
    // Create an alloca for this argument
    llvm::AllocaInst *alloca = create_entry_block_alloca(
        func, "arg" + std::to_string(idx), arg.getType());

    // Store the initial value
    builder->CreateStore(&arg, alloca);

    // Add to symbol table
    named_values["arg" + std::to_string(idx)] = alloca;
    idx++;
  }

  // Generate code for the function body
  if (t_function->block) {
    t_function->block->accept_visitor(this);

    // Verify the function
    if (llvm::verifyFunction(*func, &llvm::errs())) {
      func->eraseFromParent();
      last_value = nullptr;
      return;
    }
  }

  last_value = func;
}

void codegen_visitor_t::visit_if(if_t *t_if) {
  // Generate condition
  t_if->p_expr->accept_visitor(this);
  llvm::Value *cond_v = last_value;

  if (!cond_v) {
    return;
  }

  // Convert condition to bool if necessary
  if (cond_v->getType()->isIntegerTy() &&
      cond_v->getType()->getIntegerBitWidth() != 1) {
    cond_v = builder->CreateICmpNE(
        cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "ifcond");
  }

  llvm::Function *func = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *then_bb = llvm::BasicBlock::Create(*context, "then", func);
  llvm::BasicBlock *merge_bb = llvm::BasicBlock::Create(*context, "ifcont");

  builder->CreateCondBr(cond_v, then_bb, merge_bb);

  // Emit then block
  builder->SetInsertPoint(then_bb);
  t_if->p_stmt->accept_visitor(this);
  builder->CreateBr(merge_bb);

  // Emit merge block - insert it into the function
  func->insert(func->end(), merge_bb);
  builder->SetInsertPoint(merge_bb);
}

void codegen_visitor_t::visit_else(else_t *t_else) {
  // This should be handled together with if statement in a more complete
  // implementation
  t_else->p_stmt->accept_visitor(this);
}

void codegen_visitor_t::visit_for(compiler::parser::ast::node::for_t *t_for) {
  llvm::Function *func = builder->GetInsertBlock()->getParent();

  // Execute the initialization statement (p_first)
  if (t_for->p_first) {
    t_for->p_first->accept_visitor(this);
  }

  // Create blocks for the loop
  llvm::BasicBlock *cond_bb =
      llvm::BasicBlock::Create(*context, "forcond", func);
  llvm::BasicBlock *loop_bb =
      llvm::BasicBlock::Create(*context, "forloop", func);
  llvm::BasicBlock *incr_bb =
      llvm::BasicBlock::Create(*context, "forincr", func);
  llvm::BasicBlock *after_bb =
      llvm::BasicBlock::Create(*context, "afterfor", func);

  // Jump to condition block
  builder->CreateBr(cond_bb);

  // Emit condition block
  builder->SetInsertPoint(cond_bb);
  if (t_for->p_expr) {
    t_for->p_expr->accept_visitor(this);
    llvm::Value *cond_v = last_value;

    // Convert condition to bool if necessary
    if (cond_v && cond_v->getType()->isIntegerTy() &&
        cond_v->getType()->getIntegerBitWidth() != 1) {
      cond_v = builder->CreateICmpNE(
          cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "forcond");
    }

    builder->CreateCondBr(cond_v, loop_bb, after_bb);
  } else {
    // No condition means infinite loop (or loop until break)
    builder->CreateBr(loop_bb);
  }

  // Emit loop body
  builder->SetInsertPoint(loop_bb);
  if (t_for->p_block) {
    t_for->p_block->accept_visitor(this);
  }
  builder->CreateBr(incr_bb);

  // Emit increment block
  builder->SetInsertPoint(incr_bb);
  if (t_for->p_last) {
    t_for->p_last->accept_visitor(this);
  }
  builder->CreateBr(cond_bb);

  // Emit after block
  builder->SetInsertPoint(after_bb);
}

void codegen_visitor_t::visit_while(while_t *t_while) {
  llvm::Function *func = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *cond_bb =
      llvm::BasicBlock::Create(*context, "whilecond", func);
  llvm::BasicBlock *loop_bb =
      llvm::BasicBlock::Create(*context, "whileloop", func);
  llvm::BasicBlock *after_bb =
      llvm::BasicBlock::Create(*context, "afterwhile", func);

  // Jump to condition
  builder->CreateBr(cond_bb);

  // Emit condition
  builder->SetInsertPoint(cond_bb);
  t_while->p_expr->accept_visitor(this);
  llvm::Value *cond_v = last_value;

  if (cond_v->getType()->isIntegerTy() &&
      cond_v->getType()->getIntegerBitWidth() != 1) {
    cond_v = builder->CreateICmpNE(
        cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "whilecond");
  }

  builder->CreateCondBr(cond_v, loop_bb, after_bb);

  // Emit loop body
  builder->SetInsertPoint(loop_bb);
  t_while->p_stmt->accept_visitor(this);
  builder->CreateBr(cond_bb);

  // Emit after block
  builder->SetInsertPoint(after_bb);
}

void codegen_visitor_t::visit_do(do_t *t_do) {
  llvm::Function *func = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *loop_bb =
      llvm::BasicBlock::Create(*context, "doloop", func);
  llvm::BasicBlock *cond_bb =
      llvm::BasicBlock::Create(*context, "docond", func);
  llvm::BasicBlock *after_bb =
      llvm::BasicBlock::Create(*context, "afterdo", func);

  // Jump to loop
  builder->CreateBr(loop_bb);

  // Emit loop body
  builder->SetInsertPoint(loop_bb);
  t_do->p_stmt->accept_visitor(this);
  builder->CreateBr(cond_bb);

  // Emit condition
  builder->SetInsertPoint(cond_bb);
  t_do->p_expr->accept_visitor(this);
  llvm::Value *cond_v = last_value;

  if (cond_v->getType()->isIntegerTy() &&
      cond_v->getType()->getIntegerBitWidth() != 1) {
    cond_v = builder->CreateICmpNE(
        cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "docond");
  }

  builder->CreateCondBr(cond_v, loop_bb, after_bb);

  // Emit after block
  builder->SetInsertPoint(after_bb);
}

void codegen_visitor_t::visit_subscript_expr(
    subscript_expr_t *t_subscript_expr) {
  (void)t_subscript_expr;
  last_value = log_error_v("Subscript expressions not yet implemented");
}

void codegen_visitor_t::dump_ir() { module->print(llvm::errs(), nullptr); }
