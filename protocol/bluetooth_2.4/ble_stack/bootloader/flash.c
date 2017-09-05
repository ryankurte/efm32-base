/**************************************************************************//**
* @file flash.c
* @brief Bootloader flash writing functions.
* @version x.xx
******************************************************************************
* @section License
* <b>Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com</b>
*******************************************************************************
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
* obligation to support this Software. Silicon Labs is providing the
* Software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Silicon Labs will not be liable for any consequential, incidental, or
* special damages, or any other relief, or for any claim by any third party,
* arising from your use of this Software.
*
******************************************************************************/

#include "em_device.h"
#include "flash.h"
#include "config.h"

/**************************************************************************//**
 *
 * Programs a single word into flash.
 *
 * @param adr is the address of the word to program.
 * @param data is the word to program.
 *
 * This function will program one word into the on-chip flash.
 * Programming consists of ANDing the new data with the existing data; in
 * other words bits that contain 1 can remain 1 or be changed to 0, but bits
 * that are 0 can not be changed to 1.  Therefore, a word can be programmed
 * multiple times so long as these rules are followed; if a program operation
 * attempts to change a 0 bit to a 1 bit, that bit will not have its value
 * changed.
 *
 * This function will not return until the data has been programmed.
 *****************************************************************************/
RAMFUNC void FLASH_writeWord(uint32_t adr, uint32_t data)
{
  MSC->ADDRB    = adr;                     // Load address.
  MSC->WRITECMD = MSC_WRITECMD_LADDRIM;
  MSC->WDATA = data;                       // Load data.
  MSC->WRITECMD = MSC_WRITECMD_WRITEONCE;  // Trigger write once.
  while (MSC->STATUS & MSC_STATUS_BUSY) ;  // Waiting for the write to complete.
}

/**************************************************************************//**
 *
 * Program flash.
 *
 * @param block_start is a pointer to the base of the flash.
 * @param count is the number of bytes to be programmed. Must be a multiple of
 * four.
 * @param buffer is a pointer to a buffer holding the data.
 *
 *  This function is used to write data to the NVM. This is a blocking
 *   function.
 *****************************************************************************/
RAMFUNC void FLASH_writeBlock(void *block_start,
                              uint32_t count,
                              uint8_t const *buffer)
{
  /* Used as a temporary variable to create the blocks to write when padding to closest word. */
  uint32_t tempWord;
  uint8_t *pAddress = (uint8_t*)block_start;
  /* Enable writing to the MSC */
  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
  /* Loop over body. */
  while (count >= sizeof(tempWord)) {
    tempWord = *(uint32_t *)buffer;
    FLASH_writeWord((uint32_t ) pAddress, tempWord);

    pAddress   += sizeof(tempWord);
    buffer     += sizeof(tempWord);
    count      -= sizeof(tempWord);
  }

  /* Pad at the end */
  if (count > 0) {
    /* Get final word. */
    tempWord = *(uint32_t *)buffer;

    /* Fill rest of word with padding. */
    tempWord |= 0xFFFFFFFF << (8 * count);
    FLASH_writeWord((uint32_t ) pAddress, tempWord);
  }
  /* Disable writing to the MSC */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
}

/**************************************************************************//**
 *
 * Erase a block of flash.
 *
 * @param blockStart is the start address of the flash block to be erased.
 *
 * This function will erase one blocks on the on-chip flash.  After erasing,
 * the block will be filled with 0xff bytes.  Read-only and execute-only
 * blocks can not be erased.
 *
 * This function will not return until the block has been erased.
 *****************************************************************************/
RAMFUNC void FLASH_eraseOneBlock(uint32_t blockStart)
{
  uint32_t acc = 0xFFFFFFFF;
  uint32_t *ptr;
  /* Enable writing to the MSC */
  MSC->WRITECTRL |= MSC_WRITECTRL_WREN;
  // Optimization - check if block is allready erased.
  // This will typically happen when the chip is new.
  for (ptr = (uint32_t *)blockStart;
       ptr < (uint32_t *)(blockStart + FLASH_PAGE_SIZE);
       ptr++) {
    acc &= *ptr;
  }

  // If the accumulator is unchanged, there is no need to do an erase.
  if (acc != 0xFFFFFFFF) {
    MSC->ADDRB    = blockStart;               // Load address.
    MSC->WRITECMD = MSC_WRITECMD_LADDRIM;
    MSC->WRITECMD = MSC_WRITECMD_ERASEPAGE;   // Send Erase Page command.
    while (MSC->STATUS & MSC_STATUS_BUSY) ;   // Waiting for erase to complete.
  }
  /* Disable writing to the MSC */
  MSC->WRITECTRL &= ~MSC_WRITECTRL_WREN;
}
