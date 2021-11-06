#ifndef HE44FCF65_514D_42A5_AF8D_7445F0D43BCF
#define HE44FCF65_514D_42A5_AF8D_7445F0D43BCF

#include "mcl/stdtype.h"

#define MCL_LIST_ENTRY(elem) struct { elem* next; elem* prev; }
#define MCL_LIST_HEAD(elem)  MCL_LIST_ENTRY(elem)

#define MCL_LIST_SENTINEL(head, elem, link) \
    (elem*)((char*)(&(head)->next) - offsetof(elem, link))

#define MCL_LIST_FIRST(head) (head)->next
#define MCL_LIST_LAST(head)  (head)->prev

#define MCL_LIST_NEXT(ep, link)  (ep)->link.next
#define MCL_LIST_PREV(ep, link)  (ep)->link.prev

#define MCL_LIST_INIT(head, elem, link) do {            \
    MCL_LIST_FIRST((head)) = MCL_LIST_SENTINEL((head), elem, link); \
    MCL_LIST_LAST((head))  = MCL_LIST_SENTINEL((head), elem, link); \
} while (0)

#define MCL_LIST_EMPTY(head, elem, link)             \
    (MCL_LIST_FIRST((head)) == MCL_LIST_SENTINEL((head), elem, link))

#define MCL_LIST_ELEM_INIT(ep, link) do {       \
    MCL_LIST_NEXT((ep), link) = (ep);            \
    MCL_LIST_PREV((ep), link) = (ep);            \
} while (0)

#define MCL_LIST_SPLICE_BEFORE(le, e1, en, link) do {     \
    MCL_LIST_NEXT((en), link) = (le);                      \
    MCL_LIST_PREV((e1), link) = MCL_LIST_PREV((le), link); \
    MCL_LIST_NEXT(MCL_LIST_PREV((le), link), link) = (e1); \
    MCL_LIST_PREV((le), link) = (en);                      \
} while (0)

#define MCL_LIST_SPLICE_AFTER(le, e1, en, link) do {      \
    MCL_LIST_PREV((e1), link) = (le);                      \
    MCL_LIST_NEXT((en), link) = MCL_LIST_NEXT((le), link); \
    MCL_LIST_PREV(MCL_LIST_NEXT((le), link), link) = (en); \
    MCL_LIST_NEXT((le), link) = (e1);                      \
} while (0)

#define MCL_LIST_INSERT_BEFORE(le, ne, link) \
    MCL_LIST_SPLICE_BEFORE((le), (ne), (ne), link)

#define MCL_LIST_INSERT_AFTER(le, ne, link)  \
    MCL_LIST_SPLICE_AFTER((le), (ne), (ne), link)

#define MCL_LIST_SPLICE_HEAD(head, e1, en, elem, link) \
    MCL_LIST_SPLICE_AFTER(MCL_LIST_SENTINEL((head), elem, link), (e1), (en), link)

#define MCL_LIST_SPLICE_TAIL(head, e1, en, elem, link) \
    MCL_LIST_SPLICE_BEFORE(MCL_LIST_SENTINEL((head), elem, link), (e1), (en), link)

#define MCL_LIST_INSERT_HEAD(head, ne, elem, link) \
    MCL_LIST_SPLICE_HEAD((head), (ne), (ne), elem, link)

#define MCL_LIST_INSERT_TAIL(head, ne, elem, link) \
    MCL_LIST_SPLICE_TAIL((head), (ne), (ne), elem, link)

#define MCL_LIST_UNSPLICE(e1, en, link) do {             \
    MCL_LIST_NEXT(MCL_LIST_PREV((e1), link), link) = MCL_LIST_NEXT((en), link);\
    MCL_LIST_PREV(MCL_LIST_NEXT((en), link), link) = MCL_LIST_PREV((e1), link);\
} while (0)

#define MCL_LIST_REMOVE(ep, link) MCL_LIST_UNSPLICE((ep), (ep), link)

#define MCL_LIST_FOREACH(ep, head, elem, link)      \
    for (ep  = MCL_LIST_FIRST(head);                \
         ep != MCL_LIST_SENTINEL(head, elem, link); \
         ep  = MCL_LIST_NEXT(ep, link))

#define MCL_LIST_FOREACH_SAFE(e1, ep2, head, elem, link)            \
    for (e1  = MCL_LIST_FIRST(head), ep2 = MCL_LIST_NEXT(e1, link); \
         e1 != MCL_LIST_SENTINEL(head, elem, link);                 \
         e1 = ep2, ep2 = MCL_LIST_NEXT(e1, link))

#endif
