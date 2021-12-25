#ifndef MCL_774090C197554E02ABB46B5559A38F0A
#define MCL_774090C197554E02ABB46B5559A38F0A

#include "mcl/defer/defer_node.h"
#include "mcl/keyword.h"
#include "mcl/symbol.h"

MCL_STDC_BEGIN

MCL_TYPE(MclDefer) {
    MCL_LINK(MclDeferNode) defers;
};

void MclDefer_Init(MclDefer*);
void MclDefer_Clear(MclDefer*);
void MclDefer_Add(MclDefer*, MclDeferNode*);
void MclDefer_Exec(MclDefer*);

////////////////////////////////////////////////////
#define __MCL_DEFER_NODE_NAME(id) MCL_SYMBOL_JOIN(mclDeferNode, id)

#define __MCL_DEFER_ADD(id, defer, fn, ctxt)                        \
MclDeferNode __MCL_DEFER_NODE_NAME(id) = MCL_DEFER_NODE(fn, ctxt);  \
MclDefer_Add(&defer, &__MCL_DEFER_NODE_NAME(id))

#define MCL_DEFER(defer)                                            \
{.defers = MCL_LINK_INITIALIZER(&defer.defers, MclDeferNode, link)}

#define __MCL_DEFER_DEFAULT  __mcl_defer_default

////////////////////////////////////////////////////
#define MCL_DEFER_ENABLE()                                          \
MCL_RAII(MclDefer_Exec) MclDefer __MCL_DEFER_DEFAULT = MCL_DEFER(__MCL_DEFER_DEFAULT)

#define MCL_DEFER_ADD(fn, ctxt)                                     \
__MCL_DEFER_ADD(MCL_SYMBOL_UNIQUE_ID, __MCL_DEFER_DEFAULT, fn, ctxt)

#define MCL_DEFER_ENABLE_WITH(fn, ctxt)                             \
MCL_DEFER_ENABLE();                                                 \
MCL_DEFER_ADD(fn, ctxt)

MCL_STDC_END

#endif
