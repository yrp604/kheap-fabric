# include "clang/StaticAnalyzer/Core/CheckerRegistry.h"

# include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
# include "clang/StaticAnalyzer/Core/Checker.h"
# include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
# include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

extern "C"
void clang_registerCheckers ( CheckerRegistry & registry ) {
	registry.addChecker<KHeapFabric>("alpha.core.KHeapFabric", "Generate kernel heap fabric");
}

extern "C" const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;

using namespace clang;
using namespace clang::ento;

namespace {
	class KHeapFabric : public Checker<check::PreCall> {
		mutable std::unique_ptr<BugType> BT;
		mutable const IdentifierInfo *IImalloc;

		public:
		void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
	};
}

void KHeapFabricr::initIdentifierInfo(ASTContext &ACtx) const {
	if (IImalloc) return;

	IImalloc =  &ACtx.Idents.get("malloc");
}

void KHeapFabric::collectStructs(Expr *E, vector<std::string>& structs) const {
	if (isa<BinaryOperator>(E)) {
		collectStructs(E.getLHS(), structs);
		collectStructs(E.getRHS(), structs);

		return;
	}

	if (!isa<UnaryExprOrTypeTraitExpr>(E)) return;

	if (E->getType() != UETT_SizeOf) return;

	const Expr *no_parens = E.IgnoreParens();

	const QT *t = E.getTypeOfArgument();

	structs.push_back(t.getAsString());
}

void KHeapFabric::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
	initIdentifierInfo(C.getASTContext());

	if (Call.getCalleeIdentifier() != IImalloc) return;

	const Expr *E = Call.getArgExpr(0);

	if (!E) return;

	std::vector<std::string> structs;

	collectStructs(e, structs);

	if (structs.size() == 0) return;

	std::string struct_types = structs[0];

	for (int i = 1; i < structs.size(); ++i)
		struct_types += ", " + structs[i];

	if (!BT)
		BT.reset(new BugType(this, struct_types, "KHeap Fabric"));

	ExplodedNode *N = C.generateErrorNode();
	auto Report = llvm::make_unique<BugReport>(*BT, BT->getName(), N);
	C.emitReport(std::move(Report));
}

void ento::registerMainCallChecker(CheckerManager &Mgr) {
	Mgr.registerChecker<KHeapFabric>();
}
