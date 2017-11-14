#include "ClangSACheckers.h"

#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"

#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/CheckerRegistry.h"

#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h"

using namespace clang;
using namespace ento;

namespace {
	class KHeapFabricChecker : public Checker<check::PreCall> {
		mutable std::unique_ptr<BugType> BT;
		mutable std::vector<std::string> structs;
		mutable const IdentifierInfo *IImalloc;

		void initIdentifierInfo(const ASTContext &ACtx) const;
		void collectStructs(const Expr *E, std::vector<std::string> &structs) const;

		public:
		void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
	};
}

void KHeapFabricChecker::initIdentifierInfo(const ASTContext &ACtx) const {
	if (IImalloc) return;

	IImalloc =  &ACtx.Idents.get("malloc");
}

void KHeapFabricChecker::collectStructs(const Expr *E, std::vector<std::string> &types) const {
	if (isa<BinaryOperator>(E)) {
		const BinaryOperator *BO = cast<BinaryOperator>(E);

		collectStructs(BO->getLHS(), types);
		collectStructs(BO->getRHS(), types);

		return;
	}

	if (!isa<UnaryExprOrTypeTraitExpr>(E)) return;

	const UnaryExprOrTypeTraitExpr *UEOTT = cast<UnaryExprOrTypeTraitExpr>(E);

	if (UEOTT->getKind() != UETT_SizeOf) return;

	const QualType T = UEOTT->getTypeOfArgument();

	types.push_back(T.getAsString());
}

void KHeapFabricChecker::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
	initIdentifierInfo(C.getASTContext());

	if (Call.getCalleeIdentifier() != IImalloc) return;

	const Expr *E = Call.getArgExpr(0);

	if (!E) return;

	collectStructs(E, structs);

	if (structs.size() == 0) return;

	if (!BT)
		BT.reset(new BugType(this, "Struct used in dynamic allocation", "KHeap Fabric"));

	ExplodedNode *N = C.generateNonFatalErrorNode();

	std::unique_ptr<BugReport> Report(new BugReport(*BT, BT->getName(), N));

	for (size_t i = 0; i < structs.size(); ++i) {
		Report->addExtraText(StringRef(structs[i]));
	}

	C.emitReport(std::move(Report));

	structs.clear();
}

void ento::registerKHeapFabricChecker(CheckerManager &Mgr) {
	Mgr.registerChecker<KHeapFabricChecker>();
}
