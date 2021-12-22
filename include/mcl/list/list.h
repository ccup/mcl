#ifndef HE44FCF65_514D_42A5_AF8D_7445F0D43BCF
#define HE44FCF65_514D_42A5_AF8D_7445F0D43BCF

#include "mcl/stdtype.h"

#define MCL_LIST_NODE(NodeType) struct { NodeType* next; NodeType* prev; }
#define MCL_LIST(NodeType)  MCL_LIST_NODE(NodeType)

#define MCL_LIST_SENTINEL(list, elem, link)         \
    (elem*)((char*)(&(list)->next) - offsetof(elem, link))

#define MCL_LIST_FIRST(list) (list)->next
#define MCL_LIST_LAST(list)  (list)->prev

#define MCL_LIST_NEXT(ep, link)  (ep)->link.next
#define MCL_LIST_PREV(ep, link)  (ep)->link.prev

#define MCL_LIST_INIT(list, NodeType, link) do {    \
    MCL_LIST_FIRST((list)) = MCL_LIST_SENTINEL((list), NodeType, link); \
    MCL_LIST_LAST((list))  = MCL_LIST_SENTINEL((list), NodeType, link); \
} while (0)

#define MCL_LIST_EMPTY(list, NodeType, link)        \
    (MCL_LIST_FIRST((list)) == MCL_LIST_SENTINEL((list), NodeType, link))

#define MCL_LIST_ELEM_INIT(ep, link) do {           \
    MCL_LIST_NEXT((ep), link) = (ep);               \
    MCL_LIST_PREV((ep), link) = (ep);               \
} while (0)

#define MCL_LIST_SPLICE_BEFORE(le, e1, en, link) do {       \
    MCL_LIST_NEXT((en), link) = (le);                       \
    MCL_LIST_PREV((e1), link) = MCL_LIST_PREV((le), link);  \
    MCL_LIST_NEXT(MCL_LIST_PREV((le), link), link) = (e1);  \
    MCL_LIST_PREV((le), link) = (en);                       \
} while (0)

#define MCL_LIST_SPLICE_AFTER(le, e1, en, link) do {        \
    MCL_LIST_PREV((e1), link) = (le);                       \
    MCL_LIST_NEXT((en), link) = MCL_LIST_NEXT((le), link);  \
    MCL_LIST_PREV(MCL_LIST_NEXT((le), link), link) = (en);  \
    MCL_LIST_NEXT((le), link) = (e1);                       \
} while (0)

#define MCL_LIST_INSERT_BEFORE(le, ne, link) \
    MCL_LIST_SPLICE_BEFORE((le), (ne), (ne), link)

#define MCL_LIST_INSERT_AFTER(le, ne, link)  \
    MCL_LIST_SPLICE_AFTER((le), (ne), (ne), link)

#define MCL_LIST_SPLICE_HEAD(list, e1, en, elem, link)  \
    MCL_LIST_SPLICE_AFTER(MCL_LIST_SENTINEL((list), elem, link), (e1), (en), link)

#define MCL_LIST_SPLICE_TAIL(list, e1, en, elem, link)  \
    MCL_LIST_SPLICE_BEFORE(MCL_LIST_SENTINEL((list), elem, link), (e1), (en), link)

#define MCL_LIST_INSERT_HEAD(list, ne, NodeType, link)  \
    MCL_LIST_SPLICE_HEAD((list), (ne), (ne), NodeType, link)

#define MCL_LIST_INSERT_TAIL(list, ne, NodeType, link)  \
    MCL_LIST_SPLICE_TAIL((list), (ne), (ne), NodeType, link)

#define MCL_LIST_UNSPLICE(e1, en, link) do {            \
    MCL_LIST_NEXT(MCL_LIST_PREV((e1), link), link) = MCL_LIST_NEXT((en), link);\
    MCL_LIST_PREV(MCL_LIST_NEXT((en), link), link) = MCL_LIST_PREV((e1), link);\
} while (0)

#define MCL_LIST_REMOVE(ep, link) MCL_LIST_UNSPLICE((ep), (ep), link)

#define MCL_LIST_FOREACH(list, NodeType, link, ep)      \
    for (ep  = MCL_LIST_FIRST(list);                    \
         ep != MCL_LIST_SENTINEL(list, NodeType, link); \
         ep  = MCL_LIST_NEXT(ep, link))

#define MCL_LIST_FOREACH_SAFE(list, NodeType, link, e1, ep2)        \
    for (e1  = MCL_LIST_FIRST(list), ep2 = MCL_LIST_NEXT(e1, link); \
         e1 != MCL_LIST_SENTINEL(list, NodeType, link);             \
         e1 = ep2, ep2 = MCL_LIST_NEXT(e1, link))

#endif
