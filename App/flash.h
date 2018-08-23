#ifndef _flash_h
#define _flash_h
#include "stm32f1xx.h" 

#define 	FLASH_UPDATEFLAG_ADDR   		0x08005c00//upgrade flag flash store address 23K
#define 	Flash_APP_ADDR  				0x08006000//24K
#define 	NVIC_VectTab_FLASH				0X08000000
#define 	Bootloader_flag_ADD				0X08005c00
#define 	PROGRAM_APP_MODE      			0x55FFFF55 
#define 	PROGRAM_IAP_MODE      			0xAA0000AA 

#define		STM32_FLASH_BASE_ADD			0X0800E800

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
/************************** STM32 �ڲ� FLASH ���� *****************************/
#define 	STM32_FLASH_SIZE        64  // ��ѡSTM32��FLASH������С(��λΪK)
#define 	STM32_FLASH_WREN        1    // stm32оƬ����FLASH д��ʹ��(0������;1��ʹ��)

/* ��չ���� ------------------------------------------------------------------*/

/* �������� ------------------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );	//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );	//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );	//��ָ����ַ��ʼ����ָ�����ȵ�����
void CPU_FLASH_TEST(void);
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength );

void  ProgramReady(uint16_t _lenth);
void Set_Updata_flag(u32 data);
#endif

