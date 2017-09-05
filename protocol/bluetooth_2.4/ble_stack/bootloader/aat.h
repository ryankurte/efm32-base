#ifndef AAT_H
#define AAT_H

#include "aat_def.h"

//AAT
extern const void * const __vector_table[];
extern unsigned char CSTACK$$Limit;
extern void Reset_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);

#ifdef __ICCARM__
_Pragma("location=\".AAT\"")
const aat_t AppAddressTable
  =
  {
  .topOfStack = (uint32_t*)&CSTACK$$Limit,    //.topOfStack=(uint32_t*)&CSTACK$$Limit,
  .resetVector = Reset_Handler,           /*  1 - Reset (start instruction) */
  .nmiHandler = NMI_Handler,             /*  2 - NMI */
  .hardFaultHandler = HardFault_Handler,       /*  3 - HardFault */
  .type = APP_ADDRESS_TABLE_TYPE,     //uint16_t type;
  .version = AAT_MAJOR_VERSION,          //uint16_t version;
  .vectorTable = (void*)__vector_table,              //const HalVectorTableType *vectorTable;
  .aatSize = sizeof(AppAddressTable),
  .simeeBottom = (void*)0xFFFFFFFF,
  .imageCrc = IMAGE_CRC_MAGIC,
  .timeStamp = IMAGE_TIMESTAMP_MAGIC
  };
#endif
#ifdef __GNUC__
extern uint32_t __StackTop;
extern uint32_t __Vectors;
const aat_t AppAddressTable
__attribute__ ((section(".AAT")))
  =
  {
  .topOfStack = (uint32_t*)&__StackTop,    //.topOfStack=(uint32_t*)&CSTACK$$Limit,
  .resetVector = Reset_Handler,           /*  1 - Reset (start instruction) */
  .nmiHandler = NMI_Handler,             /*  2 - NMI */
  .hardFaultHandler = HardFault_Handler,       /*  3 - HardFault */
  .type = APP_ADDRESS_TABLE_TYPE,     //uint16_t type;
  .version = AAT_MAJOR_VERSION,          //uint16_t version;
  .vectorTable = (void*)&__Vectors,              //const HalVectorTableType *vectorTable;
  .aatSize = sizeof(AppAddressTable),
  .simeeBottom = (void*)0xFFFFFFFF,
  .imageCrc = IMAGE_CRC_MAGIC,
  .timeStamp = IMAGE_TIMESTAMP_MAGIC
  };
#endif
#endif
