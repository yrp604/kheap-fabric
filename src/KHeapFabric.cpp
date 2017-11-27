#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"

#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/CheckerRegistry.h"

#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h"

#include "clang/AST/RecordLayout.h"

#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace ento;

namespace {
	class KHeapFabricChecker : public Checker<check::PreCall> {
		mutable std::unique_ptr<BugType> BT;
		mutable std::set<std::string> structs;
		mutable std::map<std::string,std::string> struct_store;
		mutable std::set<const IdentifierInfo*> target_calls;

		void initIdentifierInfo(const ASTContext &ACtx) const;
		void collectStructs(const Expr *E, const ASTContext &ACtx, std::set<std::string> &structs) const;

		public:
			void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
	};
}

void KHeapFabricChecker::initIdentifierInfo(const ASTContext &ACtx) const {
	if (target_calls.size() > 0) return;

	target_calls.insert(&ACtx.Idents.get("kmalloc"));
	target_calls.insert(&ACtx.Idents.get("kzalloc"));
	target_calls.insert(&ACtx.Idents.get("vmalloc"));
}

void KHeapFabricChecker::collectStructs(const Expr *E, const ASTContext &ACtx, std::set<std::string> &types) const {
	if (isa<BinaryOperator>(E)) {
		const BinaryOperator *BO = cast<BinaryOperator>(E);

		collectStructs(BO->getLHS(), ACtx, types);
		collectStructs(BO->getRHS(), ACtx, types);

		return;
	}

	if (!isa<UnaryExprOrTypeTraitExpr>(E)) return;

	const UnaryExprOrTypeTraitExpr *UEOTT = cast<UnaryExprOrTypeTraitExpr>(E);

	if (UEOTT->getKind() != UETT_SizeOf) return;

	const QualType QT = UEOTT->getTypeOfArgument();

	const Type *T = QT.getTypePtr();

	if (!T->isStructureType()) return;

	std::string struct_name = QT.getAsString();

	const RecordDecl *RD = T->getAsStructureType()->getDecl();

	std::string layout;
	llvm::raw_string_ostream rsos(layout);

	ACtx.DumpRecordLayout(RD, rsos);

	structs.insert(struct_name);
	struct_store[struct_name] = rsos.str();
}

void KHeapFabricChecker::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
	initIdentifierInfo(C.getASTContext());

	if (target_calls.find(Call.getCalleeIdentifier()) == target_calls.end()) return;

	const Expr *E = Call.getArgExpr(0);

	if (!E) return;

	collectStructs(E, C.getASTContext(), structs);

	if (structs.size() == 0) return;

	if (!BT)
		BT.reset(new BugType(this, "Struct used in dynamic allocation", "KHeap Fabric"));

	ExplodedNode *N = C.generateNonFatalErrorNode();

	std::unique_ptr<BugReport> Report(new BugReport(*BT, BT->getName(), N));

	for (auto e : structs) {
		for (auto const &v : struct_store) {
			if (v.first != e) continue;

			Report->addExtraText(v.first);
			Report->addExtraText(v.second);
		}
	}

	C.emitReport(std::move(Report));

	structs.clear();
}

extern "C" const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;

extern "C"
void clang_registerCheckers(CheckerRegistry &registry) {
	registry.addChecker<KHeapFabricChecker>("alpha.security.KHeapFabric", "Recover the dynamically allocated structs");
}

