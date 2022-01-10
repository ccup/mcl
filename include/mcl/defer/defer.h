#ifndef MCL_774090C197554E02ABB46B5559A38F0A
#define MCL_774090C197554E02ABB46B5559A38F0A

#include "mcl/defer/defer_node.h"
#include "mcl/defer/defer_node_allocator.h"
#include "mcl/status.h"

MCL_STDC_BEGIN

MCL_TYPE(MclDefer) {
    MCL_LINK(MclDeferNode) defers;
    MclDeferNodeAllocator *allocator;
};

void MclDefer_Init(MclDefer*, MclDeferNodeAllocator *allocator);
void MclDefer_Clear(MclDefer*);

MclStatus MclDefer_AddNode(MclDefer*, MclDeferNode*);

MclDeferNode* MclDefer_AddFunc(MclDefer*, MclDeferFn, void *ctxt);

void MclDefer_Exec(MclDefer*);

///////////////////////////////////////////////////////////
#define MCL_DEFER(DEFER, ALLOCATOR)                         \
    {.defers = MCL_LINK_INITIALIZER(&(DEFER).defers, MclDeferNode, link), .allocator = (ALLOCATOR)}

#define MCL_DEFER_DEFAULT(DEFER)                            \
	MCL_DEFER(DEFER, &MCL_DEFER_NODE_ALLOCATOR_DEFAULT)

///////////////////////////////////////////////////////////
#define MCL_DEFER_ENABLE()                                  \
MCL_RAII(MclDefer_Exec) MclDefer __mcl_defer = MCL_DEFER_DEFAULT(__mcl_defer)

#define MCL_DEFER_ADD(FN, CTXT)                             \
MclDefer_AddFunc(&__mcl_defer, FN, CTXT)

#define MCL_DEFER_ENABLE_WITH(FN, CTXT)                     \
MCL_DEFER_ENABLE();                                         \
MCL_DEFER_ADD(FN, CTXT)

MCL_STDC_END

#endif
