#ifndef H866299D7_2FE2_4B71_87B1_AD08A1D6CB34
#define H866299D7_2FE2_4B71_87B1_AD08A1D6CB34

#include "mcl/macro/symbol.h"

///////////////////////////////////////////////////////////
#define MCL_MACRO_FLOW_LABEL(id1,id2)     					\
    MCL_SYMBOL_JOIN(MPI_LABEL_ ## id1 ## _ ## id2 ## _, __LINE__)

///////////////////////////////////////////////////////////
#define MCL_MACRO_FLOW_BEFORE(FLOW_ID, BEFORE_CODE)     	\
    if (1) {                                    			\
        BEFORE_CODE;                                 		\
        goto MCL_MACRO_FLOW_LABEL(FLOW_ID, BODY);       	\
    } else                                      			\
    MCL_MACRO_FLOW_LABEL(FLOW_ID, BODY):

///////////////////////////////////////////////////////////
#define MCL_MACRO_FLOW_AFTER(FLOW_ID, AFTER_CODE)       	\
    if (1)                                      			\
        goto MCL_MACRO_FLOW_LABEL(FLOW_ID, BODY);       	\
    else                                        			\
        while (1)                               			\
            if (1) {                            			\
                AFTER_CODE;                          		\
                break;                          			\
            } else                              			\
            MCL_MACRO_FLOW_LABEL(FLOW_ID, BODY):

///////////////////////////////////////////////////////////
#define MCL_MACRO_FLOW_DECLARE(FLOW_ID, declaration)    	\
    if (0)                                              	\
        ;                                               	\
    else                                                	\
        for (declaration;;)                             	\
            if (1) {                                    	\
                goto MCL_MACRO_FLOW_LABEL(FLOW_ID, BODY);	\
                MCL_MACRO_FLOW_LABEL(FLOW_ID, done): break;	\
            } else                                      	\
                while (1)                               	\
                    if (1)                              	\
                        goto MCL_MACRO_FLOW_LABEL(FLOW_ID, done);\
                    else                                	\
                    MCL_MACRO_FLOW_LABEL(FLOW_ID, BODY):

#endif
