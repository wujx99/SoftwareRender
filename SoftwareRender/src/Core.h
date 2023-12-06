#pragma once



#ifdef SR_DEBUG
#define SR_ENABLE_ASSERTS
#endif

#ifdef SR_ENABLE_ASSERTS //for debug
	#define SR_ASSERT(x, ...) { if(!(x)) { SR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SR_ASSERT(x, ...)
#endif
