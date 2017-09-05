/**************************************************************************//**
* @file usbkbdscancodes.c
* @brief USB HID keyboard scancodes.
* @version 4.0.0
******************************************************************************
* @section License
* <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
*******************************************************************************
*
* This file is licensed under the Silabs License Agreement. See the file
* "Silabs_License_Agreement.txt" for details. Before using this software for
* any purpose, you must agree to the terms of that agreement.
*
******************************************************************************/

#include <stdint.h>
#include "usb-hidkbd/usbkbdscancodes.h"

static const uint8_t Ascii2modifier[];
static const uint8_t Ascii2Scancode[];

static const uint8_t Scancode2Ascii[];
static const uint8_t ShiftScancode2Ascii[];

/**************************************************************************//**
 * @brief
 *   Key modifier bit meanings.
 *
 * The modifier byte is defined as follows.
 *
 * Bit  Key
 *  0   LeftControl
 *  1   LeftShift
 *  2   LeftAlt
 *  3   Left GUI
 *  4   RightControl
 *  5   RightShift
 *  6   RightAlt
 *  7   Right GUI
 *
 * So key modifier 0 is no modifer, and 2 is "LeftShift".
 *****************************************************************************/

/***************************************************************************//**
 * @brief
 *   Converts the keyboard scancode for key modifier 0 to ASCII.
 *
 * @param[in] scancode
 *   Scancode you want to convert.
 *
 * @return
 *   @ref Scancode2Ascii.
 ******************************************************************************/
char USB_Hid0ScancodeToAscii(uint8_t scancode)
{
  return Scancode2Ascii[scancode];
}

/***************************************************************************//**
 * @brief
 *   Converts the keyboard scancode for key modifier 2 to ASCII.
 *
 * @param[in] scancode
 *   Scancode you want to convert.
 *
 * @return
 *   @ref ShiftScancode2Ascii.
 ******************************************************************************/
char USB_Hid2ScancodeToAscii(uint8_t scancode)
{
  return ShiftScancode2Ascii[scancode];
}

/***************************************************************************//**
 * @brief
 *   Converts the ASCII character to key modifier.
 *
 * @param[in] ASCII character
 *   Character you want to convert.
 *
 * @return
 *   @ref Ascii2modifier.
 ******************************************************************************/
uint8_t USB_AsciiToKeyModifier(char ascii)
{
  uint8_t idx = ascii;

  if ( idx < 128 ) {
    return Ascii2modifier[idx];
  } else {
    return 0;
  }
}

/***************************************************************************//**
 * @brief
 *   Converts the ASCII character to keyboard scancode.
 *
 * @param[in] ASCII character
 *   Character you want to convert.
 *
 * @return
 *   @ref Ascii2Scancode.
 ******************************************************************************/
uint8_t USB_AsciiToKeyScancode(char ascii)
{
  uint8_t idx = ascii;

  if ( idx < 128 ) {
    return Ascii2Scancode[idx];
  } else {
    return 0;
  }
}

/**************************************************************************//**
 * @brief
 *   Table of keyboard scancode for key modifier 0 to ASCII mappings.
 *   Refer to the "HID Usage Tables" standard document from www.usb.org,
 *   section 10 "Keyboard/Keypad Page (0x07)"
 *****************************************************************************/
static const uint8_t Scancode2Ascii[256] =
{
  /*  Dec  Hex Usage                                    */
  /*  ---------------------------------------           */
  0,    /*  0    00  Reserved (no event indicated)            */
  0,    /*  1    01  Keyboard ErrorRollOver                   */
  0,    /*  2    02  Keyboard POSTFail                        */
  0,    /*  3    03  Keyboard ErrorUndefined                  */
  'a',  /*  4    04  Keyboard a and A                         */
  'b',  /*  5    05  Keyboard b and B                         */
  'c',  /*  6    06  Keyboard c and C                         */
  'd',  /*  7    07  Keyboard d and D                         */
  'e',  /*  8    08  Keyboard e and E                         */
  'f',  /*  9    09  Keyboard f and F                         */
  'g',  /*  10   0A  Keyboard g and G                         */
  'h',  /*  11   0B  Keyboard h and H                         */
  'i',  /*  12   0C  Keyboard i and I                         */
  'j',  /*  13   0D  Keyboard j and J                         */
  'k',  /*  14   0E  Keyboard k and K                         */
  'l',  /*  15   0F  Keyboard l and L                         */
  'm',  /*  16   10  Keyboard m and M                         */
  'n',  /*  17   11  Keyboard n and N                         */
  'o',  /*  18   12  Keyboard o and O                         */
  'p',  /*  19   13  Keyboard p and P                         */
  'q',  /*  20   14  Keyboard q and Q                         */
  'r',  /*  21   15  Keyboard r and R                         */
  's',  /*  22   16  Keyboard s and S                         */
  't',  /*  23   17  Keyboard t and T                         */
  'u',  /*  24   18  Keyboard u and U                         */
  'v',  /*  25   19  Keyboard v and V                         */
  'w',  /*  26   1A  Keyboard w and W                         */
  'x',  /*  27   1B  Keyboard x and X                         */
  'y',  /*  28   1C  Keyboard y and Y                         */
  'z',  /*  29   1D  Keyboard z and Z                         */
  '1',  /*  30   1E  Keyboard 1 and !                         */
  '2',  /*  31   1F  Keyboard 2 and @                         */
  '3',  /*  32   20  Keyboard 3 and #                         */
  '4',  /*  33   21  Keyboard 4 and $                         */
  '5',  /*  34   22  Keyboard 5 and %                         */
  '6',  /*  35   23  Keyboard 6 and ^                         */
  '7',  /*  36   24  Keyboard 7 and &                         */
  '8',  /*  37   25  Keyboard 8 and *                         */
  '9',  /*  38   26  Keyboard 9 and (                         */
  '0',  /*  39   27  Keyboard 0 and )                         */
  '\r', /*  40   28  Keyboard Return(ENTER)                   */
  0,    /*  41   29  Keyboard ESCAPE                          */
  0,    /*  42   2A  Keyboard DELETE(Backspace)               */
  0,    /*  43   2B  Keyboard Tab                             */
  ' ',  /*  44   2C  Keyboard Spacebar                        */
  '-',  /*  45   2D  Keyboard - and (underscore)              */
  '=',  /*  46   2E  Keyboard = and +                         */
  '[',  /*  47   2F  Keyboard [ and {                         */
  ']',  /*  48   30  Keyboard ] and }                         */
  '\\', /*  49   31  Keyboard \ and |                         */
  '#',  /*  50   32  Keyboard Non-US# and ~                   */
  ';',  /*  51   33  Keyboard ; and :                         */
  '\'', /*  52   34  Keyboard "english writing left single and double quotation mark" */
  '`',  /*  53   35  Keyboard Grave Accent ` and Tilde ~      */
  ',',  /*  54   36  Keyboard , and <                         */
  '.',  /*  55   37  Keyboard . and >                         */
  '/',  /*  56   38  Keyboard / and ?                         */
  0,    /*  57   39  Keyboard CapsLock                        */
  0,    /*  58   3A  Keyboard F1                              */
  0,    /*  59   3B  Keyboard F2                              */
  0,    /*  60   3C  Keyboard F3                              */
  0,    /*  61   3D  Keyboard F4                              */
  0,    /*  62   3E  Keyboard F5                              */
  0,    /*  63   3F  Keyboard F6                              */
  0,    /*  64   40  Keyboard F7                              */
  0,    /*  65   41  Keyboard F8                              */
  0,    /*  66   42  Keyboard F9                              */
  0,    /*  67   43  Keyboard F10                             */
  0,    /*  68   44  Keyboard F11                             */
  0,    /*  69   45  Keyboard F12                             */
  0,    /*  70   46  Keyboard PrintScreen                     */
  0,    /*  71   47  Keyboard ScrollLock                      */
  0,    /*  72   48  Keyboard Pause                           */
  0,    /*  73   49  Keyboard Insert                          */
  0,    /*  74   4A  Keyboard Home                            */
  0,    /*  75   4B  Keyboard PageUp                          */
  0,    /*  76   4C  Keyboard Delete Forward                  */
  0,    /*  77   4D  Keyboard End                             */
  0,    /*  78   4E  Keyboard PageDown                        */
  0,    /*  79   4F  Keyboard RightArrow                      */
  0,    /*  80   50  Keyboard LeftArrow                       */
  0,    /*  81   51  Keyboard DownArrow                       */
  0,    /*  82   52  Keyboard UpArrow                         */
  0,    /*  83   53  Keypad NumLock and Clear                 */
  '/',  /*  84   54  Keypad /                                 */
  '*',  /*  85   55  Keypad *                                 */
  '-',  /*  86   56  Keypad -                                 */
  '+',  /*  87   57  Keypad +                                 */
  '\r', /*  88   58  Keypad ENTER                             */
  '1',  /*  89   59  Keypad 1 and End                         */
  '2',  /*  90   5A  Keypad 2 and Down Arrow                  */
  '3',  /*  91   5B  Keypad 3 and PageDn                      */
  '4',  /*  92   5C  Keypad 4 and Left Arrow                  */
  '5',  /*  93   5D  Keypad 5                                 */
  '6',  /*  94   5E  Keypad 6 and Right Arrow                 */
  '7',  /*  95   5F  Keypad 7 and Home                        */
  '8',  /*  96   60  Keypad 8 and Up Arrow                    */
  '9',  /*  97   61  Keypad 9 and PageUp                      */
  '0',  /*  98   62  Keypad 0 and Insert                      */
  '.',  /*  99   63  Keypad . and Delete                      */
  '\\', /*  100  64  Keyboard Non-US\ and |                   */
  0,    /*  101  65  Keyboard Application                     */
  0,    /*  102  66  Keyboard Power                           */
  '=',  /*  103  67  Keypad =                                 */
  0,    /*  104  68  Keyboard F13                             */
  0,    /*  105  69  Keyboard F14                             */
  0,    /*  106  6A  Keyboard F15                             */
  0,    /*  107  6B  Keyboard F16                             */
  0,    /*  108  6C  Keyboard F17                             */
  0,    /*  109  6D  Keyboard F18                             */
  0,    /*  110  6E  Keyboard F19                             */
  0,    /*  111  6F  Keyboard F20                             */
  0,    /*  112  70  Keyboard F21                             */
  0,    /*  113  71  Keyboard F22                             */
  0,    /*  114  72  Keyboard F23                             */
  0,    /*  115  73  Keyboard F24                             */
  0,    /*  116  74  Keyboard Execute                         */
  0,    /*  117  75  Keyboard Help                            */
  0,    /*  118  76  Keyboard Menu                            */
  0,    /*  119  77  Keyboard Select                          */
  0,    /*  120  78  Keyboard Stop                            */
  0,    /*  121  79  Keyboard Again                           */
  0,    /*  122  7A  Keyboard Undo                            */
  0,    /*  123  7B  Keyboard Cut                             */
  0,    /*  124  7C  Keyboard Copy                            */
  0,    /*  125  7D  Keyboard Paste                           */
  0,    /*  126  7E  Keyboard Find                            */
  0,    /*  127  7F  Keyboard Mute                            */
  0,    /*  128  80  Keyboard Volume Up                       */
  0,    /*  129  81  Keyboard Volume Down                     */
  0,    /*  130  82  Keyboard Locking Caps Lock               */
  0,    /*  131  83  Keyboard Locking Num Lock                */
  0,    /*  132  84  Keyboard Locking Scroll Lock             */
  ',',  /*  133  85  Keypad Comma                             */
  '=',  /*  134  86  Keypad Equal Sign                        */
  0,    /*  135  87  Keyboard International 1                 */
  0,    /*  136  88  Keyboard International 2                 */
  0,    /*  137  89  Keyboard International 3                 */
  0,    /*  138  8A  Keyboard International 4                 */
  0,    /*  139  8B  Keyboard International 5                 */
  0,    /*  140  8C  Keyboard International 6                 */
  0,    /*  141  8D  Keyboard International 7                 */
  0,    /*  142  8E  Keyboard International 8                 */
  0,    /*  143  8F  Keyboard International 9                 */
  0,    /*  144  90  Keyboard LANG1                           */
  0,    /*  145  91  Keyboard LANG2                           */
  0,    /*  146  92  Keyboard LANG3                           */
  0,    /*  147  93  Keyboard LANG4                           */
  0,    /*  148  94  Keyboard LANG5                           */
  0,    /*  149  95  Keyboard LANG6                           */
  0,    /*  150  96  Keyboard LANG7                           */
  0,    /*  151  97  Keyboard LANG8                           */
  0,    /*  152  98  Keyboard LANG9                           */
  0,    /*  153  99  Keyboard AlternateErase                  */
  0,    /*  154  9A  Keyboard SysReq/Attention                */
  0,    /*  155  9B  Keyboard Cancel                          */
  0,    /*  156  9C  Keyboard Clear                           */
  0,    /*  157  9D  Keyboard Prior                           */
  '\r', /*  158  9E  Keyboard Return                          */
  0,    /*  159  9F  Keyboard Separator                       */
  0,    /*  160  A0  Keyboard Out                             */
  0,    /*  161  A1  Keyboard Oper                            */
  0,    /*  162  A2  Keyboard Clear/Again                     */
  0,    /*  163  A3  Keyboard CrSel/Props                     */
  0,    /*  164  A4  Keyboard ExSel                           */
  0,    /*  165-175  A5-AF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0,    /*  176  B0  Keypad 00                                */
  0,    /*  177  B1  Keypad 000                               */
  ',',  /*  178  B2  Thousands Separator                      */
  '.',  /*  179  B3  Decimal Separator                        */
  0,    /*  180  B4  Currency Unit                            */
  0,    /*  181  B5  Currency Sub-unit                        */
  '(',  /*  182  B6  Keypad (                                 */
  ')',  /*  183  B7  Keypad )                                 */
  '{',  /*  184  B8  Keypad {                                 */
  '}',  /*  185  B9  Keypad }                                 */
  0,    /*  186  BA  Keypad Tab                               */
  0,    /*  187  BB  Keypad Backspace                         */
  'A',  /*  188  BC  Keypad A                                 */
  'B',  /*  189  BD  Keypad B                                 */
  'C',  /*  190  BE  Keypad C                                 */
  'D',  /*  191  BF  Keypad D                                 */
  'E',  /*  192  C0  Keypad E                                 */
  'F',  /*  193  C1  Keypad F                                 */
  '^',  /*  194  C2  Keypad XOR                               */
  '^',  /*  195  C3  Keypad ^                                 */
  '%',  /*  196  C4  Keypad %                                 */
  '<',  /*  197  C5  Keypad <                                 */
  '>',  /*  198  C6  Keypad >                                 */
  '&',  /*  199  C7  Keypad &                                 */
  0,    /*  200  C8  Keypad &&                                */
  '|',  /*  201  C9  Keypad |                                 */
  0,    /*  202  CA  Keypad ||                                */
  ':',  /*  203  CB  Keypad :                                 */
  '#',  /*  204  CC  Keypad #                                 */
  ' ',  /*  205  CD  Keypad Space                             */
  '@',  /*  206  CE  Keypad @                                 */
  '!',  /*  207  CF  Keypad !                                 */
  0,    /*  208  D0  Keypad Memory Store                      */
  0,    /*  209  D1  Keypad Memory Recall                     */
  0,    /*  210  D2  Keypad Memory Clear                      */
  0,    /*  211  D3  Keypad Memory Add                        */
  0,    /*  212  D4  Keypad Memory Subtract                   */
  0,    /*  213  D5  Keypad Memory Multiply                   */
  0,    /*  214  D6  Keypad Memory Divide                     */
  0,    /*  215  D7  Keypad +/-                               */
  0,    /*  216  D8  Keypad Clear                             */
  0,    /*  217  D9  Keypad Clear Entry                       */
  0,    /*  218  DA  Keypad Binary                            */
  0,    /*  219  DB  Keypad Octal                             */
  0,    /*  220  DC  Keypad Decimal                           */
  0,    /*  221  DD  Keypad Hexadecimal                       */
  0, 0, /*  222-223  DE-DF Reserved                           */
  0,    /*  224  E0  Keyboard LeftControl                     */
  0,    /*  225  E1  Keyboard LeftShift                       */
  0,    /*  226  E2  Keyboard LeftAlt                         */
  0,    /*  227  E3  Keyboard Left GUI                        */
  0,    /*  228  E4  Keyboard RightControl                    */
  0,    /*  229  E5  Keyboard RightShift                      */
  0,    /*  230  E6  Keyboard RightAlt                        */
  0,    /*  231  E7  Keyboard Right GUI                       */
  0,    /*  232-255  E8-FF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/**************************************************************************//**
 * @brief
 *   Table of keyboard scancode for key modifier 2 (aka "LeftShift") to
 *   ASCII mappings.
 *   Refer to the "HID Usage Tables" standard document from www.usb.org,
 *   section 10 "Keyboard/Keypad Page (0x07)"
 *****************************************************************************/
static const uint8_t ShiftScancode2Ascii[256] =
{
  /*  Dec  Hex Usage                                    */
  /*  ---------------------------------------           */
  0,    /*  0    00  Reserved (no event indicated)            */
  0,    /*  1    01  Keyboard ErrorRollOver                   */
  0,    /*  2    02  Keyboard POSTFail                        */
  0,    /*  3    03  Keyboard ErrorUndefined                  */
  'A',  /*  4    04  Keyboard a and A                         */
  'B',  /*  5    05  Keyboard b and B                         */
  'C',  /*  6    06  Keyboard c and C                         */
  'D',  /*  7    07  Keyboard d and D                         */
  'E',  /*  8    08  Keyboard e and E                         */
  'F',  /*  9    09  Keyboard f and F                         */
  'G',  /*  10   0A  Keyboard g and G                         */
  'H',  /*  11   0B  Keyboard h and H                         */
  'I',  /*  12   0C  Keyboard i and I                         */
  'J',  /*  13   0D  Keyboard j and J                         */
  'K',  /*  14   0E  Keyboard k and K                         */
  'L',  /*  15   0F  Keyboard l and L                         */
  'M',  /*  16   10  Keyboard m and M                         */
  'N',  /*  17   11  Keyboard n and N                         */
  'O',  /*  18   12  Keyboard o and O                         */
  'P',  /*  19   13  Keyboard p and P                         */
  'Q',  /*  20   14  Keyboard q and Q                         */
  'R',  /*  21   15  Keyboard r and R                         */
  'S',  /*  22   16  Keyboard s and S                         */
  'T',  /*  23   17  Keyboard t and T                         */
  'U',  /*  24   18  Keyboard u and U                         */
  'V',  /*  25   19  Keyboard v and V                         */
  'W',  /*  26   1A  Keyboard w and W                         */
  'X',  /*  27   1B  Keyboard x and X                         */
  'Y',  /*  28   1C  Keyboard y and Y                         */
  'Z',  /*  29   1D  Keyboard z and Z                         */
  '!',  /*  30   1E  Keyboard 1 and !                         */
  '@',  /*  31   1F  Keyboard 2 and @                         */
  '#',  /*  32   20  Keyboard 3 and #                         */
  '$',  /*  33   21  Keyboard 4 and $                         */
  '%',  /*  34   22  Keyboard 5 and %                         */
  '^',  /*  35   23  Keyboard 6 and ^                         */
  '&',  /*  36   24  Keyboard 7 and &                         */
  '*',  /*  37   25  Keyboard 8 and *                         */
  '(',  /*  38   26  Keyboard 9 and (                         */
  ')',  /*  39   27  Keyboard 0 and )                         */
  '\n', /*  40   28  Keyboard Return(ENTER)                   */
  0,    /*  41   29  Keyboard ESCAPE                          */
  0,    /*  42   2A  Keyboard DELETE(Backspace)               */
  0,    /*  43   2B  Keyboard Tab                             */
  ' ',  /*  44   2C  Keyboard Spacebar                        */
  '_',  /*  45   2D  Keyboard - and (underscore)              */
  '+',  /*  46   2E  Keyboard = and +                         */
  '{',  /*  47   2F  Keyboard [ and {                         */
  '}',  /*  48   30  Keyboard ] and }                         */
  '|',  /*  49   31  Keyboard \ and |                         */
  '~',  /*  50   32  Keyboard Non-US# and ~                   */
  ':',  /*  51   33  Keyboard ; and :                         */
  '"',  /*  52   34  Keyboard "english writing left single and double quotation mark" */
  '~',  /*  53   35  Keyboard Grave Accent ` and Tilde ~      */
  '<',  /*  54   36  Keyboard , and <                         */
  '>',  /*  55   37  Keyboard . and >                         */
  '?',  /*  56   38  Keyboard / and ?                         */
  0,    /*  57   39  Keyboard CapsLock                        */
  0,    /*  58   3A  Keyboard F1                              */
  0,    /*  59   3B  Keyboard F2                              */
  0,    /*  60   3C  Keyboard F3                              */
  0,    /*  61   3D  Keyboard F4                              */
  0,    /*  62   3E  Keyboard F5                              */
  0,    /*  63   3F  Keyboard F6                              */
  0,    /*  64   40  Keyboard F7                              */
  0,    /*  65   41  Keyboard F8                              */
  0,    /*  66   42  Keyboard F9                              */
  0,    /*  67   43  Keyboard F10                             */
  0,    /*  68   44  Keyboard F11                             */
  0,    /*  69   45  Keyboard F12                             */
  0,    /*  70   46  Keyboard PrintScreen                     */
  0,    /*  71   47  Keyboard ScrollLock                      */
  0,    /*  72   48  Keyboard Pause                           */
  0,    /*  73   49  Keyboard Insert                          */
  0,    /*  74   4A  Keyboard Home                            */
  0,    /*  75   4B  Keyboard PageUp                          */
  0,    /*  76   4C  Keyboard Delete Forward                  */
  0,    /*  77   4D  Keyboard End                             */
  0,    /*  78   4E  Keyboard PageDown                        */
  0,    /*  79   4F  Keyboard RightArrow                      */
  0,    /*  80   50  Keyboard LeftArrow                       */
  0,    /*  81   51  Keyboard DownArrow                       */
  0,    /*  82   52  Keyboard UpArrow                         */
  0,    /*  83   53  Keypad NumLock and Clear                 */
  '/',  /*  84   54  Keypad /                                 */
  '*',  /*  85   55  Keypad *                                 */
  '-',  /*  86   56  Keypad -                                 */
  '+',  /*  87   57  Keypad +                                 */
  '\n', /*  88   58  Keypad ENTER                             */
  0,    /*  89   59  Keypad 1 and End                         */
  0,    /*  90   5A  Keypad 2 and Down Arrow                  */
  0,    /*  91   5B  Keypad 3 and PageDn                      */
  0,    /*  92   5C  Keypad 4 and Left Arrow                  */
  0,    /*  93   5D  Keypad 5                                 */
  0,    /*  94   5E  Keypad 6 and Right Arrow                 */
  0,    /*  95   5F  Keypad 7 and Home                        */
  0,    /*  96   60  Keypad 8 and Up Arrow                    */
  0,    /*  97   61  Keypad 9 and PageUp                      */
  0,    /*  98   62  Keypad 0 and Insert                      */
  0,    /*  99   63  Keypad . and Delete                      */
  '|',  /*  100  64  Keyboard Non-US\ and |                   */
  0,    /*  101  65  Keyboard Application                     */
  0,    /*  102  66  Keyboard Power                           */
  '=',  /*  103  67  Keypad =                                 */
  0,    /*  104  68  Keyboard F13                             */
  0,    /*  105  69  Keyboard F14                             */
  0,    /*  106  6A  Keyboard F15                             */
  0,    /*  107  6B  Keyboard F16                             */
  0,    /*  108  6C  Keyboard F17                             */
  0,    /*  109  6D  Keyboard F18                             */
  0,    /*  110  6E  Keyboard F19                             */
  0,    /*  111  6F  Keyboard F20                             */
  0,    /*  112  70  Keyboard F21                             */
  0,    /*  113  71  Keyboard F22                             */
  0,    /*  114  72  Keyboard F23                             */
  0,    /*  115  73  Keyboard F24                             */
  0,    /*  116  74  Keyboard Execute                         */
  0,    /*  117  75  Keyboard Help                            */
  0,    /*  118  76  Keyboard Menu                            */
  0,    /*  119  77  Keyboard Select                          */
  0,    /*  120  78  Keyboard Stop                            */
  0,    /*  121  79  Keyboard Again                           */
  0,    /*  122  7A  Keyboard Undo                            */
  0,    /*  123  7B  Keyboard Cut                             */
  0,    /*  124  7C  Keyboard Copy                            */
  0,    /*  125  7D  Keyboard Paste                           */
  0,    /*  126  7E  Keyboard Find                            */
  0,    /*  127  7F  Keyboard Mute                            */
  0,    /*  128  80  Keyboard Volume Up                       */
  0,    /*  129  81  Keyboard Volume Down                     */
  0,    /*  130  82  Keyboard Locking Caps Lock               */
  0,    /*  131  83  Keyboard Locking Num Lock                */
  0,    /*  132  84  Keyboard Locking Scroll Lock             */
  ',',  /*  133  85  Keypad Comma                             */
  '=',  /*  134  86  Keypad Equal Sign                        */
  0,    /*  135  87  Keyboard International 1                 */
  0,    /*  136  88  Keyboard International 2                 */
  0,    /*  137  89  Keyboard International 3                 */
  0,    /*  138  8A  Keyboard International 4                 */
  0,    /*  139  8B  Keyboard International 5                 */
  0,    /*  140  8C  Keyboard International 6                 */
  0,    /*  141  8D  Keyboard International 7                 */
  0,    /*  142  8E  Keyboard International 8                 */
  0,    /*  143  8F  Keyboard International 9                 */
  0,    /*  144  90  Keyboard LANG1                           */
  0,    /*  145  91  Keyboard LANG2                           */
  0,    /*  146  92  Keyboard LANG3                           */
  0,    /*  147  93  Keyboard LANG4                           */
  0,    /*  148  94  Keyboard LANG5                           */
  0,    /*  149  95  Keyboard LANG6                           */
  0,    /*  150  96  Keyboard LANG7                           */
  0,    /*  151  97  Keyboard LANG8                           */
  0,    /*  152  98  Keyboard LANG9                           */
  0,    /*  153  99  Keyboard AlternateErase                  */
  0,    /*  154  9A  Keyboard SysReq/Attention                */
  0,    /*  155  9B  Keyboard Cancel                          */
  0,    /*  156  9C  Keyboard Clear                           */
  0,    /*  157  9D  Keyboard Prior                           */
  '\n', /*  158  9E  Keyboard Return                          */
  0,    /*  159  9F  Keyboard Separator                       */
  0,    /*  160  A0  Keyboard Out                             */
  0,    /*  161  A1  Keyboard Oper                            */
  0,    /*  162  A2  Keyboard Clear/Again                     */
  0,    /*  163  A3  Keyboard CrSel/Props                     */
  0,    /*  164  A4  Keyboard ExSel                           */
  0,    /*  165-175  A5-AF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0,    /*  176  B0  Keypad 00                                */
  0,    /*  177  B1  Keypad 000                               */
  ',',  /*  178  B2  Thousands Separator                      */
  '.',  /*  179  B3  Decimal Separator                        */
  0,    /*  180  B4  Currency Unit                            */
  0,    /*  181  B5  Currency Sub-unit                        */
  '(',  /*  182  B6  Keypad (                                 */
  ')',  /*  183  B7  Keypad )                                 */
  '{',  /*  184  B8  Keypad {                                 */
  '}',  /*  185  B9  Keypad }                                 */
  0,    /*  186  BA  Keypad Tab                               */
  0,    /*  187  BB  Keypad Backspace                         */
  'A',  /*  188  BC  Keypad A                                 */
  'B',  /*  189  BD  Keypad B                                 */
  'C',  /*  190  BE  Keypad C                                 */
  'D',  /*  191  BF  Keypad D                                 */
  'E',  /*  192  C0  Keypad E                                 */
  'F',  /*  193  C1  Keypad F                                 */
  '^',  /*  194  C2  Keypad XOR                               */
  '^',  /*  195  C3  Keypad ^                                 */
  '%',  /*  196  C4  Keypad %                                 */
  '<',  /*  197  C5  Keypad <                                 */
  '>',  /*  198  C6  Keypad >                                 */
  '&',  /*  199  C7  Keypad &                                 */
  0,    /*  200  C8  Keypad &&                                */
  '|',  /*  201  C9  Keypad |                                 */
  0,    /*  202  CA  Keypad ||                                */
  ':',  /*  203  CB  Keypad :                                 */
  '#',  /*  204  CC  Keypad #                                 */
  ' ',  /*  205  CD  Keypad Space                             */
  '@',  /*  206  CE  Keypad @                                 */
  '!',  /*  207  CF  Keypad !                                 */
  0,    /*  208  D0  Keypad Memory Store                      */
  0,    /*  209  D1  Keypad Memory Recall                     */
  0,    /*  210  D2  Keypad Memory Clear                      */
  0,    /*  211  D3  Keypad Memory Add                        */
  0,    /*  212  D4  Keypad Memory Subtract                   */
  0,    /*  213  D5  Keypad Memory Multiply                   */
  0,    /*  214  D6  Keypad Memory Divide                     */
  0,    /*  215  D7  Keypad +/-                               */
  0,    /*  216  D8  Keypad Clear                             */
  0,    /*  217  D9  Keypad Clear Entry                       */
  0,    /*  218  DA  Keypad Binary                            */
  0,    /*  219  DB  Keypad Octal                             */
  0,    /*  220  DC  Keypad Decimal                           */
  0,    /*  221  DD  Keypad Hexadecimal                       */
  0, 0, /*  222-223  DE-DF Reserved                           */
  0,    /*  224  E0  Keyboard LeftControl                     */
  0,    /*  225  E1  Keyboard LeftShift                       */
  0,    /*  226  E2  Keyboard LeftAlt                         */
  0,    /*  227  E3  Keyboard Left GUI                        */
  0,    /*  228  E4  Keyboard RightControl                    */
  0,    /*  229  E5  Keyboard RightShift                      */
  0,    /*  230  E6  Keyboard RightAlt                        */
  0,    /*  231  E7  Keyboard Right GUI                       */
  0,    /*  232-255  E8-FF Reserved                           */
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint8_t Ascii2modifier[128] =
{
  /* 0x00 - 0x1f */
/* ^@   ^A   ^B   ^C   ^D   ^E   ^F   ^G */
  0x3, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
/* ^H   ^I   ^J   ^K   ^L   ^M   ^M   ^O */
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
/* ^P   ^Q   ^R   ^S   ^T   ^U   ^V   ^W */
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
/* ^X   ^Y   ^Z   ^[   ^\   ^]   ^^   ^_ */
  0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x3, 0x3,
  /* 0x20 - 0x3f */
/* sp    !    "    #    $    %    &    ' */
  0x0, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x0,
/*  (    )    *    +    ,    -    .    / */
  0x2, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0,
/*  0    1    2    3    4    5    6    7 */
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
/*  8    9    :    ;    <    =    >    ? */
  0x0, 0x0, 0x2, 0x0, 0x2, 0x0, 0x2, 0x2,
  /* 0x40 - 0x5f */
/*  @    A    B    C    D    E    F    G */
  0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
/*  H    I    J    K    L    M    M    O */
  0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
/*  P    Q    R    S    T    U    V    W */
  0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2,
/*  X    Y    Z    [    \    ]    ^    _ */
  0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x2, 0x2,
  /* 0x60 - 0x7f */
/*  `    a    b    c    d    e    f    g */
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
/*  h    i    j    k    l    m    m    o */
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
/*  p    q    r    s    t    u    v    w */
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
/*  x    y    z    {    |    }    ~   ^? */
  0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x2, 0x0,
};

static const uint8_t Ascii2Scancode[128] =
{
  /* 0x00 - 0x1f */
/*  ^@    ^A    ^B    ^C    ^D    ^E    ^F    ^G */
  0x1f, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
/*  ^H    ^I    ^J    ^K    ^L    ^M    ^M    ^O */
  0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
/*  ^P    ^Q    ^R    ^S    ^T    ^U    ^V    ^W */
  0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
/*  ^X    ^Y    ^Z    ^[    ^\    ^]    ^^    ^_ */
  0x1b, 0x1c, 0x1d, 0x2f, 0x31, 0x30, 0x23, 0x2d,
  /* 0x20 - 0x3f */
/*  sp     !     "     #     $     %     &     ' */
  0x2c, 0x1e, 0x34, 0x20, 0x21, 0x22, 0x24, 0x34,
/*   (     )     *     +     ,     -     .     / */
  0x26, 0x27, 0x25, 0x2e, 0x36, 0x2d, 0x35, 0x38,
/*   0     1     2     3     4     5     6     7 */
  0x27, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24,
/*   8     9     :     ;     <     =     >     ? */
  0x25, 0x26, 0x33, 0x33, 0x36, 0x2e, 0x37, 0x38,
  /* 0x40 - 0x5f */
/*   @     A     B     C     D     E     F     G */
  0x1f, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
/*   H     I     J     K     L     M     M     O */
  0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
/*   P     Q     R     S     T     U     V     W */
  0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
/*   X     Y     Z     [     \     ]     ^     _ */
  0x1b, 0x1c, 0x1d, 0x2f, 0x31, 0x30, 0x23, 0x2d,
  /* 0x60 - 0x7f */
/*   `     a     b     c     d     e     f     g */
  0x35, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
/*   h     i     j     k     l     m     m     o */
  0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12,
/*   p     q     r     s     t     u     v     w */
  0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
/*   x     y     z     {     |     }     ~    ^? */
  0x1b, 0x1c, 0x1d, 0x2f, 0x31, 0x30, 0x35, 0x2a,
};
