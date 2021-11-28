#include "astnode.h"
#include<map>
#include<string>

extern int spaces;
extern std::unique_ptr<LLVMContext> theContext;
extern std::unique_ptr<Module> theModule;
extern std::unique_ptr<IRBuilder<>> builder;
extern std::map<std::string, AllocaInst *> namedValues;
extern std::unique_ptr<legacy::FunctionPassManager> theFPM;
extern int grammererror;
extern std::map<std::string, AllocaInst *> curNamedValues;

extern BasicBlock *continueBasicBlock;
void printspaces() {
  for (int i = 0; i < spaces; ++i)
    std::cout << " ";
}
void printGrammerInfo(std::string nodeName, int line) {
  printspaces();
  std::cout << nodeName << " (" << line << ")" << std::endl;
}

void printSemanticError(int type, int line, std::string info = "") {
  grammererror = 1;
  std::cout << "Error type " << type << " at Line " << line << "."
            << std::endl;
}

int parseNIdentifier(NIdentifier &nIdentifier) {
  printspaces();
  std::cout << "ID: " << nIdentifier.name << std::endl;
  return 0;
}

Value *LogErrorV(const char *Str) {
  // std::cout << Str << std::endl;
  return nullptr;
}

void InitializeModuleAndPassManager() {
  // Open a new module.
  theContext = std::make_unique<LLVMContext>();
  theModule = std::make_unique<Module>("test", *theContext);

  // theModule->setDataLayout(dL);

  // Create a new builder for the module.
  builder = std::make_unique<IRBuilder<>>(*theContext);

  // Create a new pass manager attached to it.
  theFPM = std::make_unique<legacy::FunctionPassManager>(theModule.get());

  // Promote allocas to registers.
  //theFPM->add(createPromoteMemoryToRegisterPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  //theFPM->add(createInstructionCombiningPass());
  // Reassociate expressions.
  //theFPM->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  //theFPM->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  //theFPM->add(createCFGSimplificationPass());

  theFPM->doInitialization();
}

Function *getFunction(std::string Name) {
  // First, see if the function has already been added to the current module.
  if (auto *F = theModule->getFunction(Name))
    return F;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block
/// of the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
                                          StringRef VarName, Type *varType) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(varType, nullptr, VarName);
}

int NInteger::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "INT"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NFloat::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "FLOAT"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NChar::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  printspaces();
  std::cout << "CHAR"
            << ": " << value << std::endl;
  spaces -= 2;
  return 0;
}
int NIdentifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "ID"
            << ": " << name << std::endl;
  spaces -= 2;
  return 0;
}

//(a+1).Id
int NDotOperator::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  exp.parse();
  printspaces();
  std::cout << "DOT" << std::endl;
  parseNIdentifier(id);
  // id.parse();
  spaces -= 2;
  return 0;
}


int NListOperator::parse() {
  printGrammerInfo(getNodeName(), line);
  spaces += 2;
  lhs.parse();
  printspaces();
  std::cout << "LB" << std::endl;
  rhs.parse();
  printspaces();
  std::cout << "RB" << std::endl;
  spaces -= 2;
  return 0;
}
// Id[2+1]
// a +  3  exp
  

int NArgs::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  exp.parse(); //不知道是什么表达式
  if (nArgs) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nArgs->parse(); //不断递归
  }
  spaces -= 2;
  return 0;
}
//exp
//  

int NMethodCall::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  parseNIdentifier(id);
  // id.parse();
  printspaces();
  std::cout << "LP" << std::endl;
  if (nargs) {
    nargs->parse();
  }  
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;   
  // get(nargs)
  
  return 0;
}


int NParenOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "LP" << std::endl;
  printspaces();
  exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;

  //( exp )
  return 0;
}


int NSingleOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << name << std::endl;
  hs.parse();

  // - exp 
  spaces -= 2;
  return 0;
}


int NBinaryOperator::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  lhs.parse();
  printspaces();
  if (name.substr(0, 5) == "RELOP")
    std::cout << "RELOP" << std::endl;
  else
    std::cout << name << std::endl;
  rhs.parse();
  spaces -= 2;
  

  return 0;
}


int NAssignment::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  lhs.parse();
  printspaces();
  std::cout << name << std::endl;
  rhs.parse();
  spaces -= 2;
  return 0;
}


int NSpecifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "TYPE: " << type << std::endl;
  spaces -= 2;
  return 0;
}


int NVarDec::parse() {
  printGrammerInfo(getNodeName(), line);

  if (v.size()) {
    spaces += 2;
    for (int i = 0; i < v.size(); ++i) {
      printGrammerInfo(getNodeName(), line);

      spaces += 2;
    }
    parseNIdentifier(Id);
    // Id.parse();
    spaces -= 2;
    for (int i = 0; i < v.size(); ++i) {
      printspaces();
      std::cout << "LB" << std::endl;
      printspaces();
      std::cout << "INT: " << v[i] << std::endl;
      printspaces();
      std::cout << "RB" << std::endl;
      spaces -= 2;
    }
  } else {
    spaces += 2;
    parseNIdentifier(Id);
    // Id.parse();
    spaces -= 2;
  }
  return 0;
}


int NParamDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nSpecifier.parse();
  varDec.parse();
  spaces -= 2;
  return 0;
}


int NVarList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nParamDec.parse();
  if (nVarList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nVarList->parse();
  }
  spaces -= 2;
  return 0;
}


int NFunDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  parseNIdentifier(Id);
  // Id.parse();
  printspaces();
  std::cout << "LP" << std::endl;
  if (arguments)
    arguments->parse();
  printspaces();
  std::cout << "RP" << std::endl;
  spaces -= 2;
  return 0;
}
int NDec::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  vardec.parse();
  if (exp) {
    printspaces();
    std::cout << "ASSIGNOP" << std::endl;
    exp->parse();
  }
  spaces -= 2;
  return 0;
}
int NDecList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  dec.parse();
  if (nDecList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nDecList->parse();
  }
  spaces -= 2;
  return 0;
}
int NDef::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nSpecifier.parse();
  if (nDecList)
    nDecList->parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NDefList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nDef.parse();
  if (nDefList) {
    nDefList->parse();
  }
  spaces -= 2;
  return 0;
}
int NStructSpecifier::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printGrammerInfo("StructSpecifier", line);

  spaces += 2;
  printspaces();
  std::cout << "STRUCT" << std::endl;
  if (deflist) {
    if (tag) {
      printGrammerInfo("OptTag", line);
      spaces += 2;
      parseNIdentifier(*tag);
      spaces -= 2;
      printspaces();
      std::cout << "LC" << std::endl;
      deflist->parse();
      printspaces();
      std::cout << "RC" << std::endl;
    } else {
      deflist->parse();
    }
  } else if (tag) {
    printGrammerInfo("Tag", line);

    spaces += 2;
    parseNIdentifier(*tag);
    spaces -= 2;
  }
  spaces -= 2;
  spaces -= 2;
  return 0;
}
int NStmtList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nStmt.parse();
  if (nStmtList)
    nStmtList->parse();
  spaces -= 2;
  return 0;
}

int NCompSt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "LC" << std::endl;
  if (ndeflist)
    ndeflist->parse();
  if (nstmtlist)
    nstmtlist->parse();
  printspaces();
  std::cout << "RC" << std::endl;
  spaces -= 2;
  return 0;
}
int NExpStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  this->exp.parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NCompStStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  compst.parse();
  spaces -= 2;
  return 0;
}
int NRetutnStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "RETURN" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NIfStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "IF" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  spaces -= 2;
  return 0;
}
int NIfElseStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "IF" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  printspaces();
  std::cout << "ELSE" << std::endl;
  this->stmt_else.parse();
  spaces -= 2;
  return 0;
}
int NWhileStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "WHILE" << std::endl;
  printspaces();
  std::cout << "LP" << std::endl;
  this->exp.parse();
  printspaces();
  std::cout << "RP" << std::endl;
  this->stmt.parse();
  spaces -= 2;
  return 0;
}
int NBreakStmt::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  printspaces();
  std::cout << "BREAK" << std::endl;
  printspaces();
  std::cout << "SEMI" << std::endl;
  spaces -= 2;
  return 0;
}
int NExtDecList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nVarDec.parse();
  if (nExtDecList) {
    printspaces();
    std::cout << "COMMA" << std::endl;
    nExtDecList->parse();
  }
  spaces -= 2;
  return 0;
}
int NExtDef::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  specifier.parse();
  if (fundec) {
    fundec->parse();
    if (compst) {
      compst->parse();
    }
  } else {
    if (nextdeclist) {
      nextdeclist->parse();
    }
    printspaces();
    std::cout << "SEMI" << std::endl;
  }

  spaces -= 2;
  return 0;
}
int NExtDefList::parse() {
  printGrammerInfo(getNodeName(), line);

  spaces += 2;
  nExtDef.parse();
  if (nExtDefList)
    nExtDefList->parse();
  spaces -= 2;
  return 0;
}
int NProgram::parse() {
  printGrammerInfo("Program", line);
  spaces += 2;
  if (nextdeflist)
    nextdeflist->parse();
  spaces -= 2;
  return 0;
}



// codegen()
Value *Node::codegen() {
  assert(false); // Never use this function.
  // This is a list.
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}


Value *NExpression::codegen() {
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}

Value *NInteger::codegen() {
  return ConstantInt::get(*theContext, APInt(32, value, true));
}    

Value *NFloat::codegen() {
  // begin
  return ConstantFP::get(*theContext,APFloat(value));
  // end
}
Value *NChar::codegen() {
  // begin
  return ConstantInt::get(*theContext,APInt(8,value,false));
  // end
}


std::map<Value*,AllocaInst*>mp;

//不确定
Value *NIdentifier::codegen() {
  // begin
  //后悔了 不如直接用string
  return ConstantDataArray::getString(*theContext,name,true);
  // end
}


//func (exp,exp,exp)
Value *NArgs::codegen() { return exp.codegen(); }


Value *NMethodCall::codegen() {
  // begin
  Function *calleeF = theModule->getFunction(id.name);
  if(calleeF == nullptr) {
      printSemanticError(2,line);
      exit(0);
  }

  std::vector<Value *> argsV;
  //参数怎么放，如何得到alloc分配的信息，如何从exp中 获得信息
  NArgs *arg = nargs;
  
   for(auto &Farg : calleeF->args()) {
      Type *t = Farg.getType();
      if (t != arg->codegen())
   }

  for(;arg != nullptr;arg = arg->nArgs) {
      Value *v = arg->codegen();
      v->getType()
      argsV.push_back(v);
  }

  Value *retval = builder->CreateCall(calleeF,argsV);
  return retval;
  // end
}


Value *NParenOperator::codegen() { return exp.codegen(); }


// Id 是不是也要变化
Value *NSingleOperator::codegen() {
  // begin
  Value * v = hs.codegen();
  Value *vv;
  if(hs.name != "") {  //Id
    if(mp[v] == nullptr) {
      printSemanticError(1,line);
      exit(0);
    }
    else {
        vv = builder->CreateLoad(mp[v]->getAllocatedType(),mp[v]);
    }
  } else {
      vv = v;
  }

  switch (op)
  {
  case 282:
    return builder->CreateNot(vv);
  case 275:
    return builder->CreateNeg(vv);
  default:
    break;
  }
  return nullptr;
  // end
}



Value *NBinaryOperator::codegen() {
  // begin
  //如果不是标志符怎么办？怎么去处理
  Value *lid,*rid,*l,*r;
  lid = lhs.codegen();
  rid = rhs.codegen();
  if(lhs.name != "") {
      if(mp[lid] == nullptr) {
        printSemanticError(1,line);
        exit(0);
      } else {
        l = builder->CreateLoad(mp[lid]->getAllocatedType(),mp[lid]);
      }
  } else {
    l = lid;
  }

  if(rhs.name != "") {
      if(mp[rid] == nullptr) {
        printSemanticError(1,line);
        exit(0);
      } else {
        r = builder->CreateLoad(mp[rid]->getAllocatedType(),mp[rid]);
      }
  } else {
    r = rid;
  }

  switch (op)
  {
  // case 279:

  //   break;
  case 280:
      return builder->CreateAnd(l,r);
  case 281:
      return builder->CreateOr(l,r);
  case 274:
      return builder->CreateAdd(l,r);
  case 275:
      return builder->CreateSub(l,r);
  case 276:
      return builder->CreateMul(l,r);
  case 277:
      return builder->CreateFDiv(l,r);
  default:
    break;
  }
  return nullptr;
  // end
}


Value *NAssignment::codegen() {
  // Assignment requires the LHS to be an identifier.
  // begin
  Value* lv,*rv;
  AllocaInst * a;

  lv = lhs.codegen();
  rv = rhs.codegen();
  // rhs.
  if(mp[lv] != nullptr)
  {
    if(rv->getType() != mp[lv]->getAllocatedType()) {
      printSemanticError(5,line);
      exit(0);
    } 
    builder->CreateStore(rv,mp[lv]);
  }  
  else 
  {
    if(lhs.name != "") {
      printSemanticError(1,line);
      exit(0);
    } else {
      printSemanticError(6,line);
      exit(0);
    }
  }

  return nullptr;
  // end
}


Value *NSpecifier::codegen() {
  // begin
  // return getType();
  // end
}

Type *NSpecifier::getType() {
  if (type == "int")
    return Type::getInt32Ty(*theContext);
  if (type == "float")
    return Type::getFloatTy(*theContext);
  if (type == "char")
    return Type::getInt8Ty(*theContext);
  assert(false);
  return Type::getInt32Ty(*theContext);
}


// a
// a[2][3]  
Value *NVarDec::codegen() {
  // begin
  // builder->
  return Id.codegen();
  //不支持数组 目前
  // end
}

//函数形参
Value *NParamDec::codegen() {
  // begin
  return nullptr;
  // end
}

std::pair<std::string, Type *> NParamDec::getType() {
  assert(varDec.v.size() == 0);
  std::pair<std::string, Type *> tmp(varDec.Id.name, nSpecifier.getType());
  return tmp;
}

//函数形参组
Value *NVarList::codegen() {
  assert(false); // Never use this function.
  // This is a list.
  return ConstantInt::get(*theContext, APInt(32, 0, true));
}


Function *NFunDec::funcodegen(Type *retType) {
  // check if it exists the same name of fun
  
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif

  if (theModule->getFunction(Id.name)) {
    printSemanticError(4, line, "Redefined " + Id.name);
    return nullptr;
  }

  std::vector<Type *> argsTypes;
  std::vector<std::string> argNames;
  
  for (NVarList *item = arguments; item; item = item->nVarList) {
    
    auto tmp = item->nParamDec.getType();
    
    argNames.push_back(tmp.first);
    
    argsTypes.push_back(tmp.second);
  }


  FunctionType *ft = FunctionType::get(retType, argsTypes, false);
  
  Function *f =
      Function::Create(ft, Function::ExternalLinkage, Id.name, theModule.get());
      //Id.name 就是函数名字
  unsigned idx = 0;
  
  for (auto &arg : f->args()) {
    arg.setName(argNames[idx++]);
  }
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif

  return f; 
}



Value *NDef::codegen() {
  // begin
  Type *s = nSpecifier.getType();
  // builder->CreateAlloca
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif
  
  if(nDecList != nullptr) {
      NDecList *d = nDecList;
      for(;d != nullptr;d = d->nDecList) {
          if(mp[d->dec.vardec.codegen()] != nullptr) {
              printSemanticError(3,line);
              exit(0);
          }
          // 但是还要赋值怎么办  ？
          AllocaInst *alloca_tmp = builder->CreateAlloca(s,nullptr,d->dec.vardec.Id.name);
          if(d->dec.exp != nullptr) {
              Value *v = d->dec.exp->codegen();
              builder->CreateStore(v,alloca_tmp);
          } 
          mp[d->dec.vardec.codegen()] = alloca_tmp; //创建的变量 保存起来
          #ifdef debug
          std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
          #endif
      } 
      
  } 

  return nullptr;  
}


Value *NDefList::codegen() {
  // begin
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
    #endif
    
  nDef.codegen();
  
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
    #endif
    
  if(nDefList != nullptr) 
     nDefList->codegen();
  return nullptr;
  // end
}


Value *NStmtList::codegen() {
  auto *retVal = nStmt.codegen();
  if (nStmtList)
    retVal = nStmtList->codegen();
  return retVal;
}


Value *NCompSt::codegen() {
  // 自行处理变量作用域的问题
  
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif
  
  Value *retVal = nullptr;
  if (ndeflist)
    retVal = ndeflist->codegen();
  
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif
  
  if (nstmtlist)
    retVal = nstmtlist->codegen();
  return retVal;
}


Value *NExpStmt::codegen() { return exp.codegen(); }

Value *NCompStStmt::codegen() {
  // begin

  return  compst.codegen();
  // end
}


Value *NRetutnStmt::codegen() {

  Function *theFun = builder->GetInsertBlock()->getParent();
  
  BasicBlock *bb = BasicBlock::Create(*theContext, "ret", theFun);
  
  builder->CreateBr(bb);
  
  builder->SetInsertPoint(bb);
  
  Value *retVal = exp.codegen();

  Value *v = builder->CreateLoad(mp[retVal]->getAllocatedType(),mp[retVal]);
  
  // check the return type and fundec type
  // begin
  Type *t = theFun->getReturnType();
  
  
  // end
  builder->CreateRet(v);
  return v;
}


Value *NIfStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  // begin

  return nullptr;
  // end
}
Value *NIfElseStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  // begin

  return nullptr;
  // end
}
Value *NWhileStmt::codegen() {
  Function *theFun = builder->GetInsertBlock()->getParent();
  BasicBlock *condb = BasicBlock::Create(*theContext, "cond", theFun);
  // begin

  return nullptr;
  // end
}
Value *NBreakStmt::codegen() {
  // begin

  return nullptr;
  // end
}



Value *NExtDefVarDec::codegen() {
  // begin
  // Type *t = 
  return nullptr;
  // end
}


//函数定义
Value *NExtDefFunDec::codegen() {
  Type *retType = specifier.getType();
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif
  
  Function *f = fundec->funcodegen(retType);
  if (!f) {
    return nullptr;
  }
  assert(compst != nullptr); // Assert compst is not null.
  BasicBlock *bb = BasicBlock::Create(*theContext, "entry", f);
  builder->SetInsertPoint(bb);
  namedValues.clear();
  for (auto &arg : f->args()) {
    // Create an alloca for this variable.
    AllocaInst *alloca =
        CreateEntryBlockAlloca(f, arg.getName(), arg.getType());

    if (curNamedValues[std::string(arg.getName())]) {
      printSemanticError(3, line, "Redefined " + arg.getName().str());
      return LogErrorV("Unknown function referenced");
    }
    // Store the initial value into the alloca.
    builder->CreateStore(&arg, alloca);
    // Add arguments to variable symbol table.
    namedValues[std::string(arg.getName())] = alloca;
    curNamedValues[std::string(arg.getName())] = alloca;
  }
  if (Value *retVal = compst->codegen()) {
    // Finish off the function.

    // Validate the generated code, checking for consistency.
    verifyFunction(*f);

    // Run the optimizer on the function.
    // theFPM->run(*f);
    return f;
  }
  // Error reading body, remove function.
  f->eraseFromParent();
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif
  
  return nullptr;
}


Value *NExtDefList::codegen() {
  
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif
  
  auto *lastCode = nExtDef.codegen();
  // lastCode->print(errs());
  // assert(nExtDefList == nullptr);
  
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif

  if (nExtDefList)
    lastCode = nExtDefList->codegen();
  return lastCode;
}



Value *NProgram::codegen() {
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl;
  #endif

  //默认输出函数putchar
  std::vector<Type *> putArgs;
  putArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *putType =
      FunctionType::get(builder->getInt32Ty(), putArgs, false);
  Function *putFunc = Function::Create(putType, Function::ExternalLinkage,
                                       "putchar", theModule.get());

  //默认输入函数getchar
  std::vector<Type *> getArgs;
  // getArgs.push_back(Type::getInt32Ty(*theContext));

  FunctionType *getType =
      FunctionType::get(builder->getInt32Ty(), getArgs, false);
  Function *getFunc = Function::Create(getType, Function::ExternalLinkage,
                                       "getchar", theModule.get());

  Value *lastCode = nextdeflist->codegen();
 
  #ifdef debug
  std::cout<<"line: "<<__LINE__<<" file: "<<__FUNCTION__<<std::endl; 
    #endif
    
  if (grammererror)
    return nullptr;
  return lastCode;
}