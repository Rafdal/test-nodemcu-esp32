/* FastCRC library code is placed under the MIT license
 * Copyright (c) 2014,2015 Frank Bösing
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//
// HW-calculations are 32BIT
//
// Thanks to:
// - Catalogue of parametrised CRC algorithms, CRC RevEng
// http://reveng.sourceforge.net/crc-catalogue/
//
// - Danjel McGougan (CRC-Table-Generator)
//

#include "Arduino.h"
#if defined(KINETISK)

#include "mk20dx128.h"
#include "FastCRC.h"

// ===============================================

typedef struct {
  union {
	  uint32_t CRC; //CRC Data register
	  struct {
		  uint16_t CRC16;
		  uint16_t CRC16_1;
	  };
	  /* struct {
		  uint8_t CRC8;
		  uint8_t CRC8_1;
		  uint8_t CRC8_2;
		  uint8_t CRC8_3;
	  }; */
  };
  //uint32_t GPOLY; //CRC Polynomial register
  //uint32_t CTRL;  //CRC Control register
} CRC_T;

static volatile CRC_T * const rCRC = (CRC_T *)0x40032000;

#define CRC_CTRL_WAS   25 // Write CRC Data Register As Seed(1) / Data(0)
#define CRC_CTRL_TCRC  24 // Width of CRC protocol (0=16 BIT, 1=32 BIT)
#define CRC_CTRL_TOTR1 29 // TOTR[1]

// ================= 7-BIT CRC ===================
/** Constructor
 * Enables CRC-clock
 */
/* FastCRC7::FastCRC7(){
  SIM_SCGC6 |= SIM_SCGC6_CRC;
} */

/** CRC 7
 * MultiMediaCard interface
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint8_t FastCRC7::crc7(const uint8_t *data, const uint16_t datalen)
{
  // poly=0x09 init=0x00 refin=false refout=false xorout=0x00 check=0x75
   return (generic(0x09, 0, CRC_FLAG_NOREFLECT, data, datalen));
}*/

/** Update
 * Call for subsequent calculations with previous seed
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint8_t FastCRC7::update(const uint8_t *data, const uint16_t datalen)
{

  const uint8_t *src = data;
  const uint8_t *target = src + datalen;

  while (((uintptr_t)src & 0x03) != 0 && (src < target)) {
    rCRC->CRC8_3 = *src++; //Write 8 BIT
  }

  while (src <= target-4) {
    rCRC->CRC = *( uint32_t  *)src; //Write 32 BIT
    src += 4;
  }

  while (src < target) {
    rCRC->CRC8_3 = *src++; //Write 8 Bit
  }


    return rCRC->CRC8_3 >> 1;
} */

/** generic function for all 7-Bit CRCs
 * @param polynom Polynom
 * @param seed Seed
 * @param flags Flags
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint8_t FastCRC7::generic(const uint8_t polynom, const uint8_t seed, const uint32_t flags, const uint8_t *data,const uint16_t datalen)
{

  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC) | (1<<CRC_CTRL_WAS);                      // 32Bit Mode, Prepare to write seed(25)
  rCRC->GPOLY = ((uint32_t)polynom)<<(24 + 1);                                       // Set polynom
  rCRC->CRC   = ((uint32_t)seed<<(24 + 1));                                          // Write seed
  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC);								  		  	 // Clear WAS Bit - prepare to write data

  return update(data, datalen);
} */

/* uint8_t FastCRC7::crc7_upd(const uint8_t *data, uint16_t datalen){return update(data, datalen);}



// ================= 8-BIT CRC ===================

///// Constructor
 // Enables CRC-clock
 ///
FastCRC8::FastCRC8(){
  SIM_SCGC6 |= SIM_SCGC6_CRC;
} */



/* uint8_t FastCRC8::smbus(const uint8_t *data, const uint16_t datalen)
{
  // poly=0x07 init=0x00 refin=false refout=false xorout=0x00 check=0xf4
  return generic(0x07, 0, CRC_FLAG_NOREFLECT, data, datalen);
} */

/** MAXIM 8-Bit CRC
 * equivalent to _crc_ibutton_update() in crc16.h from avr_libc
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint8_t FastCRC8::maxim(const uint8_t *data, const uint16_t datalen)
{
  // poly=0x31 init=0x00 refin=true refout=true xorout=0x00  check=0xa1
  return generic(0x31, 0, CRC_FLAG_REFLECT, data, datalen);
} */

/** Update
 * Call for subsequent calculations with previous seed
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint8_t FastCRC8::update(const uint8_t *data, const uint16_t datalen)
{

  const uint8_t *src = data;
  const uint8_t *target = src + datalen;

  while (((uintptr_t)src & 0x03) != 0 && (src < target)) {
    rCRC->CRC8_3 = *src++; //Write 8 BIT
  }

  while (src <= target-4) {
    rCRC->CRC = *( uint32_t  *)src; //Write 32 BIT
    src += 4;
  }

  while (src < target) {
    rCRC->CRC8_3 = *src++; //Write 8 Bit
  }

  if (rCRC->CTRL & (1<<CRC_CTRL_TOTR1))
    return rCRC->CRC8;
  else
    return rCRC->CRC8_3;
} */

/** generic function for all 8-Bit CRCs
 * @param polynom Polynom
 * @param seed Seed
 * @param flags Flags
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint8_t FastCRC8::generic(const uint8_t polynom, const uint8_t seed, const uint32_t flags, const uint8_t *data,const uint16_t datalen)
{

  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC) | (1<<CRC_CTRL_WAS);                      // 32Bit Mode, Prepare to write seed(25)
  rCRC->GPOLY = ((uint32_t)polynom)<<24;                                             // Set polynom
  rCRC->CRC   = ((uint32_t)seed<<24);                                                // Write seed
  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC);								  		  	 // Clear WAS Bit - prepare to write data

  return update(data, datalen);
} */
/*
uint8_t FastCRC8::smbus_upd(const uint8_t *data, uint16_t datalen){return update(data, datalen);}
uint8_t FastCRC8::maxim_upd(const uint8_t *data, uint16_t datalen){return update(data, datalen);} */

// ================= 16-BIT CRC ===================

/** Constructor
 * Enables CRC-clock
 */
FastCRC16::FastCRC16(){
  SIM_SCGC6 |= SIM_SCGC6_CRC;
}

/** CCITT
 * Alias "false CCITT"
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint16_t FastCRC16::ccitt(const uint8_t *data,const uint16_t datalen)
{
 // poly=0x1021 init=0xffff refin=false refout=false xorout=0x0000 check=0x29b1
  return generic(0x1021, 0XFFFF, CRC_FLAG_NOREFLECT, data, datalen);
} */

/** MCRF4XX
 * equivalent to _crc_ccitt_update() in crc16.h from avr_libc
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint16_t FastCRC16::mcrf4xx(const uint8_t *data,const uint16_t datalen)
{
 // poly=0x1021 init=0xffff refin=true refout=true xorout=0x0000 check=0x6f91
  return generic(0x1021, 0XFFFF, CRC_FLAG_REFLECT, data, datalen);
} */

/** MODBUS
 * equivalent to _crc_16_update() in crc16.h from avr_libc
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint16_t FastCRC16::modbus(const uint8_t *data, const uint16_t datalen)
{
 // poly=0x8005 init=0xffff refin=true refout=true xorout=0x0000 check=0x4b37
  return generic(0x8005, 0XFFFF, CRC_FLAG_REFLECT, data, datalen);
} */

/** KERMIT
 * Alias CRC-16/CCITT, CRC-16/CCITT-TRUE, CRC-CCITT
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
uint16_t FastCRC16::kermit(const uint8_t *data, const uint16_t datalen)
{
 // poly=0x1021 init=0x0000 refin=true refout=true xorout=0x0000 check=0x2189
 // sometimes byteswapped presentation of result
  return generic(0x1021, 0x00, CRC_FLAG_REFLECT, data, datalen);
}

/** XMODEM
 * Alias ZMODEM, CRC-16/ACORN
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint16_t FastCRC16::xmodem(const uint8_t *data, const uint16_t datalen)
{
  //width=16 poly=0x1021 init=0x0000 refin=false refout=false xorout=0x0000 check=0x31c3
  return generic(0x1021, 0, CRC_FLAG_NOREFLECT, data, datalen);
} */

/** X25
 * Alias CRC-16/IBM-SDLC, CRC-16/ISO-HDLC, CRC-B
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint16_t FastCRC16::x25(const uint8_t *data, const uint16_t datalen)
{
  // poly=0x1021 init=0xffff refin=true refout=true xorout=0xffff check=0x906e
  return generic(0x1021, 0XFFFF, CRC_FLAG_REFLECT | CRC_FLAG_XOR, data, datalen);
} */

/** Update
 * Call for subsequent calculations with previous seed
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
uint16_t FastCRC16::update(const uint8_t *data, const uint16_t datalen)
{
  const uint8_t *src = data;
  const uint8_t *target = src + datalen;

  while (((uintptr_t)src & 0x03) !=0 && (src < target))  {
    rCRC->CRC8_3 = *src++; //Write 8 BIT
  }

  while (src <= target-4) {
    rCRC->CRC = *( uint32_t  *)src; //Write 32 BIT
    src += 4;
  }

  while (src < target) {
    rCRC->CRC8_3 = *src++; //Write 8 Bit
  }

  if (rCRC->CTRL & (1<<CRC_CTRL_TOTR1))
    return rCRC->CRC16;
  else
    return rCRC->CRC16_1;
}

/** generic function for all 16-Bit CRCs
 * @param polynom Polynom
 * @param seed Seed
 * @param flags Flags
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
uint16_t FastCRC16::generic(const uint16_t polynom, const uint16_t seed, const uint32_t flags, const uint8_t *data, const uint16_t datalen)
{

  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC) | (1<<CRC_CTRL_WAS);// 32-Bit Mode, prepare to write seed(25)
  rCRC->GPOLY = ((uint32_t)polynom)<<16;                       // set polynom
  rCRC->CRC   = ((uint32_t)seed<<16);                          // this is the seed
  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC);				  	   // Clear WAS Bit - prepare to write data

  return update(data, datalen);
}

//uint16_t FastCRC16::ccitt_upd(const uint8_t *data, uint16_t len)  {return update(data, len);}
//uint16_t FastCRC16::mcrf4xx_upd(const uint8_t *data, uint16_t len){return update(data, len);}
uint16_t FastCRC16::kermit_upd(const uint8_t *data, uint16_t len) {return update(data, len);}
//uint16_t FastCRC16::modbus_upd(const uint8_t *data, uint16_t len) {return update(data, len);}
//uint16_t FastCRC16::xmodem_upd(const uint8_t *data, uint16_t len) {return update(data, len);}
//uint16_t FastCRC16::x25_upd(const uint8_t *data, uint16_t len)    {return update(data, len);}



// ================= 32-BIT CRC ===================

/** Constructor
 * Enables CRC-clock
 */
/* FastCRC32::FastCRC32(){
  SIM_SCGC6 |= SIM_SCGC6_CRC;
} */

/** CRC32
 * Alias CRC-32/ADCCP, PKZIP, Ethernet, 802.3
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint32_t FastCRC32::crc32(const uint8_t *data, const uint16_t datalen)
{
  // poly=0x04c11db7 init=0xffffffff refin=true refout=true xorout=0xffffffff check=0xcbf43926
  return generic(0x04C11DB7L, 0XFFFFFFFFL, CRC_FLAG_REFLECT | CRC_FLAG_XOR, data, datalen);
} */

/** CKSUM
 * Alias CRC-32/POSIX
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint32_t FastCRC32::cksum(const uint8_t *data, const uint16_t datalen)
{
  // width=32 poly=0x04c11db7 init=0x00000000 refin=false refout=false xorout=0xffffffff check=0x765e7680
  return generic(0x04C11DB7L, 0, CRC_FLAG_NOREFLECT | CRC_FLAG_XOR, data, datalen);
} */

/** Update
 * Call for subsequent calculations with previous seed
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
//#pragma GCC diagnostic ignored "-Wpointer-arith"
/* uint32_t FastCRC32::update(const uint8_t *data, const uint16_t datalen)
{

  const uint8_t *src = data;
  const uint8_t *target = src + datalen;

  while (((uintptr_t)src & 0x03) != 0  && (src < target)) {
    rCRC->CRC8_3 = *src++; //Write 8 BIT
  }

  while (src <= target-4) {
    rCRC->CRC = *( uint32_t  *)src; //Write 32 BIT
    src += 4;
  }

  while (src < target) {
    rCRC->CRC8_3 = *src++; //Write 8 Bit
  }

  return rCRC->CRC;
} */

/** generic function for all 32-Bit CRCs
 * @param polynom Polynom
 * @param seed Seed
 * @param flags Flags
 * @param data Pointer to Data
 * @param datalen Length of Data
 * @return CRC value
 */
/* uint32_t FastCRC32::generic(const uint32_t polynom, const uint32_t seed, const uint32_t flags, const uint8_t *data, const uint16_t datalen)
{

  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC) | (1<<CRC_CTRL_WAS); // 32Bit Mode, prepare to write seed(25)
  rCRC->GPOLY = polynom;                                        // Set polynom
  rCRC->CRC   = seed;                                           // This is the seed
  rCRC->CTRL  = flags | (1<<CRC_CTRL_TCRC);                     // Clear WAS Bit - prepare to write data

  return update(data, datalen);
}

uint32_t FastCRC32::crc32_upd(const uint8_t *data, uint16_t len){return update(data, len);}
uint32_t FastCRC32::cksum_upd(const uint8_t *data, uint16_t len){return update(data, len);} */
#endif // #if defined(KINETISK)