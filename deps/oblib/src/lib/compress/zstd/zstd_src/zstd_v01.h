/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef ZSTD_V01_H_28739879432
#define ZSTD_V01_H_28739879432

#if defined (__cplusplus)
extern "C" {
#endif

/* *************************************
*  Includes
***************************************/
#include <stddef.h>   /* size_t */


/* *************************************
*  Simple one-step function
***************************************/
/**
ZSTDv01_decompress() : decompress ZSTD frames compliant with v0.1.x format
    compressedSize : is the exact source size
    maxOriginalSize : is the size of the 'dst' buffer, which must be already allocated.
                      It must be equal or larger than originalSize, otherwise decompression will fail.
    return : the number of bytes decompressed into destination buffer (originalSize)
             or an errorCode if it fails (which can be tested using ZSTDv01_isError())
*/
size_t ZSTDv01_decompress( void* dst, size_t maxOriginalSize,
                     const void* src, size_t compressedSize);

/**
ZSTDv01_isError() : tells if the result of ZSTDv01_decompress() is an error
*/
unsigned ZSTDv01_isError(size_t code);


/* *************************************
*  Advanced functions
***************************************/
typedef struct ZSTDv01_Dctx_s ZSTDv01_Dctx;
ZSTDv01_Dctx* ZSTDv01_createDCtx(void);
size_t ZSTDv01_freeDCtx(ZSTDv01_Dctx* dctx);

size_t ZSTDv01_decompressDCtx(void* ctx,
                              void* dst, size_t maxOriginalSize,
                        const void* src, size_t compressedSize);

/* *************************************
*  Streaming functions
***************************************/
size_t ZSTDv01_resetDCtx(ZSTDv01_Dctx* dctx);

size_t ZSTDv01_nextSrcSizeToDecompress(ZSTDv01_Dctx* dctx);
size_t ZSTDv01_decompressContinue(ZSTDv01_Dctx* dctx, void* dst, size_t maxDstSize, const void* src, size_t srcSize);
/**
  Use above functions alternatively.
  ZSTD_nextSrcSizeToDecompress() tells how much bytes to provide as 'srcSize' to ZSTD_decompressContinue().
  ZSTD_decompressContinue() will use previous data blocks to improve compression if they are located prior to current block.
  Result is the number of bytes regenerated within 'dst'.
  It can be zero, which is not an error; it just means ZSTD_decompressContinue() has decoded some header.
*/

/* *************************************
*  Prefix - version detection
***************************************/
#define ZSTDv01_magicNumber   0xFD2FB51E   /* Big Endian version */
#define ZSTDv01_magicNumberLE 0x1EB52FFD   /* Little Endian version */


#if defined (__cplusplus)
}
#endif

#endif /* ZSTD_V01_H_28739879432 */
