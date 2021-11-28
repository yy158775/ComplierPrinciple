Value *ExprIfAST::Codegen() {

    //进行条件的代码
    Value *CondV = Cond->Codegen();

    if (CondV == 0)
        return 0;
    //这里生成的就是icmp ne指令就比如说icmp ne i32 %booltmp, 0
    CondV = Builder.CreateICmpNE(
                                 CondV, ConstantInt::get(Type::getInt32Ty(MyGlobalContext), 0), "ifcond");

    
    //在结构良好的LLVM IR中，每条指令都嵌入在一个基本块中。您可以从getParent()获取BasicBlock。getParent()将始终在LLVM IR层次结构中向前走一步，即。您可以从BasicBlock中获得父函数，从函数中得到模块。

    //返回当前插入点
    /**
     此代码创建与if/then/else语句相关的基本块，并直接对应于上面示例中的块,下面就是去获取正在构建的当前函数对象。它通过请求构建器获得当前的BasicBlock，并请求该块获得它的“父块”(它当前嵌入的函数)。
     */
    Function *TheFunction = Builder.GetInsertBlock()->getParent();

    /*
     一旦它有了这个，它就会创建三个块。
     注意，它将“TheFunction”传递给“then”块的构造函数。
     这导致构造函数自动将新块插入到指定函数的末尾。
     创建了另外两个块，但还没有插入到函数中。
     */
    //创建then的代码块
    BasicBlock *ThenBB = BasicBlock::Create(MyGlobalContext, "then", TheFunction);

    //创建else的代码块
    BasicBlock *ElseBB = BasicBlock::Create(MyGlobalContext, "else");

    //如果指定了父参数，那么基本块将自动插入到函数的末尾(如果InsertBefore是0)，或者在指定的基本块之前。
    //ifcont:块
    BasicBlock *MergeBB = BasicBlock::Create(MyGlobalContext, "ifcont");

    /**
     一旦这些块被创建，我们就可以发出在它们之间进行选择的条件分支。注意，创建新块不会隐式地影响IRBuilder，因此它仍然插入到条件进入的块中。还要注意，它正在创建一个分支到“then”块和“else”块，即使“else”块还没有插入到函数中。这都没问题:这是LLVM支持转发引用的标准方式。
     */
    //创建一个有条件的“br Cond, TrueDest, falseDest的指令。
    //br i1 %ifcond, label %then, label %else
    Builder.CreateCondBr(CondV, ThenBB, ElseBB);


    /**
     在插入条件分支之后，我们将构建器移动到“then”块中。严格地说，这个调用将插入点移动到指定块的末尾。但是，由于“then”块是空的，它也从在块的开始插入开始，其实就是去设置插入点
     */
    Builder.SetInsertPoint(ThenBB);

    //生成Then的IR表示，一旦设置了插入点，我们就递归地将AST中的“then”进行调用Codegen
    //所以我们这里需要先去生成不影响我们phi的设置
    Value *ThenV = Then->Codegen();

    if (ThenV == 0)
        return 0;



    //为了完成“then”块，我们为merge块创建一个无条件的分支,我们为merge block创建一个无条件的分支
    //就是br label %ifcont这条指令
    Builder.CreateBr(MergeBB);


    /*
     下面这一行非常微妙，但非常重要。基本的问题是，当我们在合并块中创建Phi节点时，我们需要设置块/值对，以指示Phi如何工作
     重要的是，Phi节点期望在CFG中为块的每个前身其实就是
     then:                                             ; preds = %entry
     br label %ifcont。
     这李的then就表示是前身
     
     CFG就是控制流图(Conttol flow graph)是用在编译器的一种抽象的数据结构，它是一个过程或程序的抽象表现，由编译器在内部维护
     
     为什么，当我们把它设置到上面的5行时，我们得到当前块了吗？
     
     问题是，“Then” expression本身可能会改变构建器所释放的块，例如，它包含一个嵌套的“if/ Then /else”表达式。因为递归地调用codegen()可以任意改变当前块的概念，所以我们需要为设置Phi节点的代码获取最新的值。
     
     */
    //Then的codegen可以改变当前的块，在PHI中更新ThenBB
    ThenBB = Builder.GetInsertBlock();


    /*
     else”块的代码生成基本上与“then”块的代码生成相同。
     唯一显著的区别是第一行，它将“else”块添加到函数中。还记得之前创建了“else”块，但没有添加到函数中。
     既然已经发出了“then”和“else”块，我们就可以用合并代码结束
     */
    TheFunction->getBasicBlockList().push_back(ElseBB);

    Builder.SetInsertPoint(ElseBB);

    Value *ElseV = Else->Codegen();

    if (ElseV == 0)
        return 0;

    //创建br指令，也就是br label %ifcont
    Builder.CreateBr(MergeBB);

    ElseBB = Builder.GetInsertBlock();

    /**
     ifcont:                                           ; preds = %else, %then
     %iftmp = phi i32 [ 1, %then ], [ %addtmp3, %else ]
     ret i32 %iftmp
     }
     */
    //插入MergeBB块：
    TheFunction->getBasicBlockList().push_back(MergeBB);

    Builder.SetInsertPoint(MergeBB);

     //分支语句：需要phi merge节点
    PHINode *PN =
    Builder.CreatePHI(Type::getInt32Ty(MyGlobalContext), 2, "iftmp");

    //这里就是生产 %iftmp = phi i32 [ 1, %then ], [ %addtmp3, %else ]
    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);

    return PN;
}