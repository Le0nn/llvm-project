#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/ExprObjC.h"
#include "clang/AST/ExprOpenMP.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include <utility>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
using namespace clang;
using namespace ento;

namespace
{
    class ListDeclChecker : public Checker<check::ASTDecl<FunctionDecl>> {
    public:
    void checkASTDecl(const FunctionDecl *D,AnalysisManager &Mgr, BugReporter &BR) const;
    ListDeclChecker();
    ~ListDeclChecker();
    };
}
ListDeclChecker::ListDeclChecker()
{
    ;//fulllog = new Log(FUNC_LOGFILENAME,false);
}
ListDeclChecker::~ListDeclChecker()
{
    ;//delete fulllog;
}
void ListDeclChecker::checkASTDecl(const FunctionDecl *D,
                       AnalysisManager &Mgr,
                       BugReporter &BR) const {
    D = D->getDefinition() == nullptr ? D : D->getDefinition();
    if (!D->isThisDeclarationADefinition())
        return;

    std::string fullname = "";
    std::string retname = D->getReturnType().getAsString();
    std::string name = D->getQualifiedNameAsString();
    fullname = "|"+retname + "@" + name + "|";

    std::vector<std::vector<std::string>> args;
    for( FunctionDecl::param_const_iterator it = D->param_begin();it!=D->param_end();it++)
    {
        std::vector<std::string> temp;
        ParmVarDecl* param = *it;
        QualType type = param->getOriginalType();
        std::string argtypename = type.getAsString();
        temp.push_back(argtypename);
        llvm::StringRef argname = param->getName();
        if(argname.empty())
        {
            temp.push_back("");
        }
        else
        {
            temp.push_back(argname.str());
        }
        args.push_back(temp);
    }
    
    for(auto it : args)
    {
        std::string argt = it[0];
        std::string argn = it[1];
        fullname += "|"+argt + "@" + argn + "|";
    }
    std::cout << fullname << std::endl;
}

void ento::registerListDeclChecker(CheckerManager &mgr) {
  mgr.registerChecker<ListDeclChecker>();
}

bool ento::shouldRegisterListDeclChecker(const LangOptions &LO) {
  return true;
}
