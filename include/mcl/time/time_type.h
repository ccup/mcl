#ifndef H1148C153_FD63_4A84_81B3_09B3977A4560
#define H1148C153_FD63_4A84_81B3_09B3977A4560

#include "mcl/keyword.h"

MCL_STDC_BEGIN

///////////////////////////////////////////////////////////
typedef uint64_t MclTimeSec;

static const MclTimeSec MCL_TIME_SEC_INVALID = MCL_UINT64_MAX;

MCL_INLINE bool MclTimeSec_IsValid(MclTimeSec sec) {
	return sec != MCL_TIME_SEC_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint32_t MclTimeSecDiff;

static const MclTimeSecDiff MCL_TIME_SEC_DIFF_INVALID = MCL_UINT32_MAX;

MCL_INLINE bool MclTimeSecDiff_IsValid(MclTimeSecDiff diff) {
	return diff != MCL_TIME_SEC_DIFF_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint64_t MclTimeMs;

static const MclTimeMs MCL_TIME_MS_INVALID  = MCL_UINT64_MAX;

MCL_INLINE bool MclTimeMs_IsValid(MclTimeMs ms) {
	return ms != MCL_TIME_MS_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint32_t MclTimeMsDiff;

static const MclTimeMsDiff  MCL_TIME_MS_DIFF_INVALID  = MCL_UINT32_MAX;

MCL_INLINE bool MclTimeMsDiff_IsValid(MclTimeMsDiff diff) {
	return diff != MCL_TIME_MS_DIFF_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint64_t MclTimeUs;

static const MclTimeUs MCL_TIME_US_INVALID  = MCL_UINT64_MAX;

MCL_INLINE bool MclTimeUs_IsValid(MclTimeUs us) {
	return us != MCL_TIME_US_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint32_t MclTimeUsDiff;

static const MclTimeUsDiff  MCL_TIME_US_DIFF_INVALID  = MCL_UINT32_MAX;

MCL_INLINE bool MclTimeUsDiff_IsValid(MclTimeUsDiff diff) {
	return diff != MCL_TIME_US_DIFF_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint64_t MclTimeNs;

static const MclTimeNs MCL_TIME_NS_INVALID  = MCL_UINT64_MAX;

MCL_INLINE bool MclTimeNs_IsValid(MclTimeNs ns) {
	return ns != MCL_TIME_NS_INVALID;
}

///////////////////////////////////////////////////////////
typedef uint32_t MclTimeNsDiff;

static const MclTimeNsDiff  MCL_TIME_NS_DIFF_INVALID  = MCL_UINT32_MAX;

MCL_INLINE bool MclTimeNsDiff_IsValid(MclTimeNsDiff diff) {
	return diff != MCL_TIME_NS_DIFF_INVALID;
}

MCL_STDC_END

#endif
