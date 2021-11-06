#ifndef H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5
#define H8E11CCEE_05CA_4E3E_BF3C_1E19204214E5

#include "mcl/keyword.h"
#include "mcl/status.h"
#include <stdint.h>

MCL_STDC_BEGIN

MCL_TYPE_DEF(MclLinkNode) {
	MclLinkNode *next;
	MclLinkNode *prev;
	uintptr_t handle;
};

typedef MclLinkNode MclLinkHead;

MCL_TYPE_DEF(MclLink) {
	MclLinkHead head;
	uint32_t count;
};

typedef void (*MclLinkNodeHandleDeleter)(uintptr_t);

#define MCL_LINK_HANDLE(node) ((node)->handle)
#define MCL_LINK_SET_HANDLE(node, value)			\
	do {											\
		(node)->handle = (uintptr_t)(value);		\
	} while(0)

#define MCL_LINK_IS_EMPTY(link) (0 == (link)->count)

#define MCL_LINK_FIRST(link) ((link)->head.next)
#define MCL_LINK_FIRST_SAFE(link) (MCL_LINK_IS_EMPTY(link) ? NULL : MCL_LINK_FIRST(link))

#define MCL_LINK_LAST(link) ((link)->head.prev)
#define MCL_LINK_LAST_SAFE(link) (MCL_LINK_IS_EMPTY(link) ? NULL : MCL_LINK_LAST(link))

#define MCL_LINK_NEXT(node) ((node)->next)
#define MCL_LINK_NEXT_SAFE(link, node) 				\
	((NULL == (node)) ? MCL_LINK_FIRST_SAFE(link) : (((node)->next == &(link)->head) ? NULL : MCL_LINK_NEXT(node)))

#define MCL_LINK_PREV(node) ((node)->prev)
#define MCL_LINK_PREV_SAFE(link, node) 				\
	((NULL == (node)) ? MCL_LINK_FIRST_SAFE(link) : (((node)->prev == &(link)->head) ? NULL : MCL_LINK_PREV(node)))

#define MCL_LINK_ENTRY(ptr, type, member) ((type*)((VOS_CHAR*)(ptr) - (VOS_CHAR*)(&((type*)0)->member)))

#define MCL_LINK_FOR_EACH(link, node)				\
	for ((node) = MCL_LINK_FIRST_SAFE((link));		\
		(MclLinkNode*)NULL != (node);				\
		(node) = MCL_LINK_NEXT_SAFE((link), (node)))

#define MCL_LINK_FOR_EACH_SAFE(link, node, tmpNode)	\
	for ((node) = MCL_LINK_FIRST_SAFE((link)), (tmpNode) = MCL_LINK_NEXT_SAFE((link), (node));	\
		(MclLinkNode*)NULL != (node);				\
		(node) = (tmpNode), (tmpNode) = MCL_LINK_NEXT_SAFE((link), (node)))

#define MCL_LINK_VISIT_VOID(link, type, visitor, ...)\
	do {											\
		MclLinkNode *node = NULL;					\
		MCL_LINK_FOR_EACH((link), (node)) {			\
			(void)visitor((type*)((node)->handle), ##__VA_ARGS__);	\
		}											\
	} while(0)

#define MCL_LINK_VISIT(link, type, visitor, ...)	\
	do {											\
		MclLinkNode *node = NULL;					\
		MCL_LINK_FOR_EACH((link), (node)) {			\
			MCL_ASSERT_SUCC_CALL(visitor((type*)((node)->handle), ##__VA_ARGS__));	\
		}											\
	} while(0)

MclLink* MclLink_Create();
void MclLink_Delete(MclLink*, MclLinkNodeHandleDeleter);

void MckLink_Init(MclLink*);
void MclLink_Clear(MclLink*, MclLinkNodeHandleDeleter);

bool MclLink_IsEmpty(const MclLink*);
uint32_t MclLink_GetSize(const MclLink*);

void MclLink_AddBeforeSafe(MclLink*, MclLinkNode* nextNode, MclLinkNode* node);
void MclLink_AddAfterSafe(MclLink*, MclLinkNode* nextNode, MclLinkNode* node);
void MclLink_AddHeadSafe(MclLink*, MclLinkNode*);
void MclLink_AddTailSafe(MclLink*, MclLinkNode*);
void MclLink_RemoveSafe(MclLink*, MclLinkNode*);
void MclLink_DeleteNode(MclLink*, MclLinkNode*, MclLinkNodeHandleDeleter);

MclStatus MclLink_AddHeadWithHandle(MclLink*, uintptr_t handle);
MclStatus MclLink_AddTailWithHandle(MclLink*, uintptr_t handle);

MclLinkNode* MclLink_FindNodeByHandle(const MclLink*, uintptr_t handle);

void MclLink_RemoveNodeWithHandle(MclLink*, uintptr_t handle);
void MclLink_DeleteNodeWithHandle(MclLink*, uintptr_t handle, MclLinkNodeHandleDeleter);

MCL_STDC_END

#endif
