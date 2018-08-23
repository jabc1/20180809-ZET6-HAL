#ifndef _uart_h
#define _uart_h

#ifdef __cplusplus
 extern "C" {
#endif
#define local_type	0xa0
#define local_soft	0x01
	 
	 
#pragma pack(push,1)
typedef struct{
	u8 status;
	u16 Rxlen;
	u8 RxBuff[600];//按照最长的接收情况算
}_Uart1;
typedef struct{
	u8 buff[11];
	u8 data[520];
}_Update;
typedef struct{
	u8 Type;
	u8 Soft;
	u16 Len;
	u16 Crc;
}_UpdateHead;
#pragma pack(pop)
extern _Uart1 Uart1;
void init_uart1(void);
void test_print(void);
void re_uart1_rx(void);
void MX_USART1_UART_Init(void);	 
void user_Uart1Handler(void); 
void init_queue(void);	 
#ifdef __cplusplus
}
#endif	

#endif

