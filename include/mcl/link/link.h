#ifndef HE44FCF65_514D_42A5_AF8D_7445F0D43BCF
#define HE44FCF65_514D_42A5_AF8D_7445F0D43BCF

#include "mcl/stdtype.h"

#define MCL_LINK_NODE(NodeType) struct { NodeType* next; NodeType* prev; }
#define MCL_LINK(NodeType)  MCL_LINK_NODE(NodeType)

#define MCL_LINK_SENTINEL(head, elem, link)         \
    (elem*)((char*)(&(head)->next) - offsetof(elem, link))

#define MCL_LINK_FIRST(head) (head)->next
#define MCL_LINK_LAST(head)  (head)->prev

#define MCL_LINK_NEXT(ep, link)  (ep)->link.next
#define MCL_LINK_PREV(ep, link)  (ep)->link.prev

#define MCL_LINK_INIT(head, NodeType, link) do {    \
    MCL_LINK_FIRST((head)) = MCL_LINK_SENTINEL((head), NodeType, link); \
    MCL_LINK_LAST((head))  = MCL_LINK_SENTINEL((head), NodeType, link); \
} while (0)

#define MCL_LINK_EMPTY(head, NodeType, link)        \
    (MCL_LINK_FIRST((head)) == MCL_LINK_SENTINEL((head), NodeType, link))

#define MCL_LINK_NODE_INIT(ep, link) do {           \
    MCL_LINK_NEXT((ep), link) = (ep);               \
    MCL_LINK_PREV((ep), link) = (ep);               \
} while (0)


#define MCL_LINK_NODE_IS_IN_LINK(ep, link)                \
    (ep && (ep->link.next) && (ep->link.prev) && (ep->link.next->link.prev == ep) && (ep->link.prev->link.next == ep))

#define MCL_LINK_SPLICE_BEFORE(le, e1, en, link) do {       \
    MCL_LINK_NEXT((en), link) = (le);                       \
    MCL_LINK_PREV((e1), link) = MCL_LINK_PREV((le), link);  \
    MCL_LINK_NEXT(MCL_LINK_PREV((le), link), link) = (e1);  \
    MCL_LINK_PREV((le), link) = (en);                       \
} while (0)

#define MCL_LINK_SPLICE_AFTER(le, e1, en, link) do {        \
    MCL_LINK_PREV((e1), link) = (le);                       \
    MCL_LINK_NEXT((en), link) = MCL_LINK_NEXT((le), link);  \
    MCL_LINK_PREV(MCL_LINK_NEXT((le), link), link) = (en);  \
    MCL_LINK_NEXT((le), link) = (e1);                       \
} while (0)

#define MCL_LINK_INSERT_BEFORE(le, ne, link) \
    MCL_LINK_SPLICE_BEFORE((le), (ne), (ne), link)

#define MCL_LINK_INSERT_AFTER(le, ne, link)  \
    MCL_LINK_SPLICE_AFTER((le), (ne), (ne), link)

#define MCL_LINK_SPLICE_HEAD(head, e1, en, elem, link)  \
    MCL_LINK_SPLICE_AFTER(MCL_LINK_SENTINEL((head), elem, link), (e1), (en), link)

#define MCL_LINK_SPLICE_TAIL(head, e1, en, elem, link)  \
    MCL_LINK_SPLICE_BEFORE(MCL_LINK_SENTINEL((head), elem, link), (e1), (en), link)

#define MCL_LINK_INSERT_HEAD(head, ne, NodeType, link)  \
    MCL_LINK_SPLICE_HEAD((head), (ne), (ne), NodeType, link)

#define MCL_LINK_INSERT_TAIL(head, ne, NodeType, link)  \
    MCL_LINK_SPLICE_TAIL((head), (ne), (ne), NodeType, link)

#define MCL_LINK_UNSPLICE(e1, en, link) do {            \
    MCL_LINK_NEXT(MCL_LINK_PREV((e1), link), link) = MCL_LINK_NEXT((en), link);\
    MCL_LINK_PREV(MCL_LINK_NEXT((en), link), link) = MCL_LINK_PREV((e1), link);\
} while (0)

#define MCL_LINK_REMOVE(ep, link) MCL_LINK_UNSPLICE((ep), (ep), link)

#define MCL_LINK_FOREACH(head, NodeType, link, ep)      \
    for (ep  = MCL_LINK_FIRST(head);                    \
         ep != MCL_LINK_SENTINEL(head, NodeType, link); \
         ep  = MCL_LINK_NEXT(ep, link))

#define MCL_LINK_FOREACH_SAFE(head, NodeType, link, e1, ep2)        \
    for (e1  = MCL_LINK_FIRST(head), ep2 = MCL_LINK_NEXT(e1, link); \
         e1 != MCL_LINK_SENTINEL(head, NodeType, link);             \
         e1 = ep2, ep2 = MCL_LINK_NEXT(e1, link))

#define MCL_LINK_INITIALIZER(head, NodeType, link)   		        \
{.next = MCL_LINK_SENTINEL((head), NodeType, link), .prev = MCL_LINK_SENTINEL((head), NodeType, link)}


#endif
