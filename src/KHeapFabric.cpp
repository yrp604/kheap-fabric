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

void KHeapFabric::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
	initIdentifierInfo(C.getASTContext());

	if (Call.getCalleeIdentifier() != IImalloc) return;
}

void ento::registerMainCallChecker(CheckerManager &Mgr) {
	Mgr.registerChecker<KHeapFabric>();
}
