/////////////////////////////////////////////////////////////////////////////
//
// File: aes_decrypt_cfb.c
//
// Copyright S. Brennen Ball, 2010
//
// The author provides no guarantees, warantees, or promises, implied or
//  otherwise.  By using this software you agree to indemnify the author
//  of any damages incurred by using it.
//
/////////////////////////////////////////////////////////////////////////////

#include "aes.h"
#include "aes_src.h"


#if (AES_ALLOW_NON_ECB_BLOCK_CIPHER_MODES != 0)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// void aes_decrypt_cfb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block,
//                      unsigned char * output_plain_text_block, unsigned int len);
//
// Description:
//  Decrypts an input block of cipher text into an output block of plain text using cipher feedback (CFB) mode
//
// Parameters:
//  aes_data_t * aes_data_p - data structure for this instance
//  const unsigned char * input_plain_text_block - plain text to be encrypted (the first len bytes will be encrypted)
//  unsigned char * output_cipher_text_block - encrypted cipher text (must be at least len bytes in length)
//  unsigned int len - number of bytes to be encrypted (can be any valid unsigned integer value)
//
// Return value:
//  None
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aes_decrypt_cfb(aes_data_t * aes_data_p, const unsigned char * input_cipher_text_block, unsigned char * output_plain_text_block, unsigned int len) __REENTRANT
{
	unsigned int i;

	//Since CFB allows turning a block cipher into a stream cipher, we can decrypt as many bytes as we want
	for(i = 0; i < len; i++)
	{
		//There are no key bytes left, so encrypt the IV/intermediate data to get another key byte block
		if(aes_data_p->rem_key_bytes == 0)
		{
			aes_encrypt_ecb(aes_data_p, aes_data_p->iv_or_intermediate_data, aes_data_p->iv_or_intermediate_data);
			aes_data_p->rem_key_bytes = 16;
		}

		//The general algorithm for CFB decryption is
		// 1. Set the current output plain text byte to the XOR of the current key byte and the current byte from the input cipher text
		// 2. Copy the current input cipher text byte to the current IV/intermediate data byte
		// 3. Decrement the number of remaining key block bytes
		output_plain_text_block[i] = aes_data_p->iv_or_intermediate_data[16 - aes_data_p->rem_key_bytes] ^ input_cipher_text_block[i];
		aes_data_p->iv_or_intermediate_data[16 - aes_data_p->rem_key_bytes] = input_cipher_text_block[i];
		aes_data_p->rem_key_bytes--;
	}
}
#endif
