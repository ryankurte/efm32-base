#ifndef AAT_DEF_H
#define AAT_DEF_H

#define APP_ADDRESS_TABLE_TYPE          (0x0AA7)
#define BOOTLOADER_ADDRESS_TABLE_TYPE   (0x0BA7)
#define RAMEXE_ADDRESS_TABLE_TYPE       (0x0EA7)

#define AAT_VERSION                     (0x0109)
#define AAT_MAJOR_VERSION               (0x0100)
#define AAT_MAJOR_VERSION_MASK          (0xFF00)

//Magic numbers are used to check if preinstalled software is present to skip crc check
//EBL DFU files contain real crc values which will be checked that image is valid
#define IMAGE_CRC_MAGIC                 (0xB1736D20)
#define IMAGE_TIMESTAMP_MAGIC           (0xACE135C3)

typedef struct aat_s{
  uint32_t *topOfStack;           // Stack initialization pointer
  void     (*resetVector)(void);  // Reset vector
  void     (*nmiHandler)(void);   // NMI Handler
  void     (*hardFaultHandler)(void); // Hardfault handler
  uint16_t type;                  // 0x0AA7 for Application Address Table, 0x0BA7 for Bootloader Address Table, 0x0EA7 for RAMEXE
  uint16_t version;               // Current version = 0x0109 (first byte is majorversion, last byte is minorversion)
  uint32_t *vectorTable;          // Pointer to the real Cortex-M vector table
  uint8_t  platInfo;              // Platform type - defined in micro.h
  uint8_t  microInfo;             // Micro type - defined in micro.h
  uint8_t  phyInfo;               // Phy type - defined in micro.h
  uint8_t  aatSize;               // Size of the AAT in bytes.
  uint16_t softwareVersion;       // SOFTWARE_VERSION (Defined by the stack)
  uint16_t softwareBuild;         // EMBER_BUILD_NUMBER (Defined by the stack)
  uint32_t timeStamp;             // Unix epoch time for EBL generation
  uint8_t  imageInfo[32];         // User defined string. Usually just NULL (our tools don't fill this out).
  uint32_t imageCrc;              // CRC of the pageRanges defined below.
  uint16_t pageRanges[6];         // Each pageRange is two bytes. The first defines the first page used, and the second byte is the last page in the range.
  void     *simeeBottom;          // Pointer to the bottom of SimEE
  uint32_t customerApplicationVersion; // Customer defined version number.
  void     *internalStorageBottom; // Location of internal storage.
  uint8_t  imageStamp[8];         // AES-MMO of image. Not used for security, but for checking if on-chip image is the same as the EBL version.
  uint8_t  familyInfo;            // family type - defined in micro.h
  uint8_t  reserved0[3];          // Reserved for bootloader expansion
  uint32_t reserved1[5];          // Reserved for bootloader expansion
} aat_t;

#endif
