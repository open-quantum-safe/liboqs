#include "api.h"

#if Saber_type == 1
	#include "recon_LightSaber.c"
#elif Saber_type == 2
	#include "recon_Saber.c"
#elif Saber_type == 3
	#include "recon_FireSaber.c"
#endif
