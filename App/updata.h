#ifndef _updata_h
#define _updata_h
#ifdef __cplusplus
 extern "C" {
#endif
#include "base.h"
u16 ChkCrcValueEx(uint8_t *buf, uint16_t len, uint16_t FirstData);	 
void Jump_IAP_APP(void); 
	 
#ifdef __cplusplus
}
#endif	 

#endif


