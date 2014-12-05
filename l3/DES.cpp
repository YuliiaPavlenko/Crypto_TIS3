#include "stdafx.h"
#include "DES.h"
#include "PermutationTables.h"
#include "Sboxes.h"
#include "SecureKey.h"

#define SWAPBITS(source, result, bitIndex)\
	( result = ((result << 1) | ((source >> bitIndex) & 1)) )

#define SWAPBITS1st(source, result, bitIndex)\
(result = ((source >> bitIndex) & 1))

#define SWAPBITSGROUP(source, result, bitIndex)\
	( result = ((result << 6) | ((source >> bitIndex) & 63)) )

#define SWAPBITSGROUP5(source, result, bitIndex)\
	( result = ((result << 5) | ((source >> bitIndex) & 31)) )

//tmp???
#define CRYPTROUND(leftWord,rightWord)\
	tmp = leftWord ^ fCrypt(rightWord);\
	leftWord = rightWord;\
	rightWord = tmp;\
	++iterationIndex

DES::DES(SecureKey* keyObj) 
	: keyObject(keyObj)
{

}

uint64 DES::makeCrypt(uint64 block)
{
	initialPermutation(block);
	unsigned rightWord = (block & 0x00000000FFFFFFFF);
	unsigned leftWord = (block & 0xFFFFFFFF00000000) >> 32;
	iterationIndex = 0;
	unsigned tmp;

	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);

	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);

	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);

	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);
	CRYPTROUND(leftWord, rightWord);

	leftWord = leftWord ^ fCrypt(rightWord);
	block = 0;
	block = (leftWord << 32) | rightWord;

	return endPermutation(block);
}

/*void DES::cryptRound()//macro
{
	unsigned tmp = leftWord ^ fCrypt(rightWord);;
	leftWord = rightWord;
	rightWord = tmp;
	++iterationIndex;
}*/

uint64 DES::initialPermutation(uint64 source)
{
	uint64 destination = 0;
	SWAPBITS1st(source, destination, 62);
	SWAPBITS(source, destination, 54);
	SWAPBITS(source, destination, 46);
	SWAPBITS(source, destination, 38);
	SWAPBITS(source, destination, 30);
	SWAPBITS(source, destination, 22);
	SWAPBITS(source, destination, 14);
	SWAPBITS(source, destination, 6);
								  
	SWAPBITS(source, destination, 60);
	SWAPBITS(source, destination, 52);
	SWAPBITS(source, destination, 44);
	SWAPBITS(source, destination, 36);
	SWAPBITS(source, destination, 28);
	SWAPBITS(source, destination, 20);
	SWAPBITS(source, destination, 12);
	SWAPBITS(source, destination, 4);
								  
	SWAPBITS(source, destination, 58);
	SWAPBITS(source, destination, 50);
	SWAPBITS(source, destination, 42);
	SWAPBITS(source, destination, 34);
	SWAPBITS(source, destination, 26);
	SWAPBITS(source, destination, 18);
	SWAPBITS(source, destination, 10);
	SWAPBITS(source, destination, 2);
								  
	SWAPBITS(source, destination, 56);
	SWAPBITS(source, destination, 48);
	SWAPBITS(source, destination, 40);
	SWAPBITS(source, destination, 32);
	SWAPBITS(source, destination, 24);
	SWAPBITS(source, destination, 16);
	SWAPBITS(source, destination, 8);
	SWAPBITS(source, destination, 0);
								  
	SWAPBITS(source, destination, 63);
	SWAPBITS(source, destination, 55);
	SWAPBITS(source, destination, 47);
	SWAPBITS(source, destination, 39);
	SWAPBITS(source, destination, 31);
	SWAPBITS(source, destination, 23);
	SWAPBITS(source, destination, 15);
	SWAPBITS(source, destination, 7);
								  
	SWAPBITS(source, destination, 61);
	SWAPBITS(source, destination, 53);
	SWAPBITS(source, destination, 45);
	SWAPBITS(source, destination, 37);
	SWAPBITS(source, destination, 29);
	SWAPBITS(source, destination, 21);
	SWAPBITS(source, destination, 13);
	SWAPBITS(source, destination, 5);
								  
	SWAPBITS(source, destination, 59);
	SWAPBITS(source, destination, 51);
	SWAPBITS(source, destination, 43);
	SWAPBITS(source, destination, 35);
	SWAPBITS(source, destination, 27);
	SWAPBITS(source, destination, 19);
	SWAPBITS(source, destination, 11);
	SWAPBITS(source, destination, 3);
								  
	SWAPBITS(source, destination, 57);
	SWAPBITS(source, destination, 49);
	SWAPBITS(source, destination, 41);
	SWAPBITS(source, destination, 33);
	SWAPBITS(source, destination, 25);
	SWAPBITS(source, destination, 17);
	SWAPBITS(source, destination, 9);
	SWAPBITS(source, destination, 1);
	
	return destination;
}

uint64 DES::endPermutation(uint64 source)
{
	uint64 destination = 0;

	SWAPBITS1st(source, destination, 32);
	SWAPBITS(source, destination, 0);
	SWAPBITS(source, destination, 40);
	SWAPBITS(source, destination, 8);
	SWAPBITS(source, destination, 48);
	SWAPBITS(source, destination, 16);
	SWAPBITS(source, destination, 56);
	SWAPBITS(source, destination, 24);
								  
	SWAPBITS(source, destination, 33);
	SWAPBITS(source, destination, 1);
	SWAPBITS(source, destination, 41);
	SWAPBITS(source, destination, 9);
	SWAPBITS(source, destination, 49);
	SWAPBITS(source, destination, 17);
	SWAPBITS(source, destination, 57);
	SWAPBITS(source, destination, 25);
								  
	SWAPBITS(source, destination, 34);
	SWAPBITS(source, destination, 2);
	SWAPBITS(source, destination, 42);
	SWAPBITS(source, destination, 10);
	SWAPBITS(source, destination, 50);
	SWAPBITS(source, destination, 18);
	SWAPBITS(source, destination, 58);
	SWAPBITS(source, destination, 26);
								  
	SWAPBITS(source, destination, 35);
	SWAPBITS(source, destination, 3);
	SWAPBITS(source, destination, 43);
	SWAPBITS(source, destination, 11);
	SWAPBITS(source, destination, 51);
	SWAPBITS(source, destination, 19);
	SWAPBITS(source, destination, 59);
	SWAPBITS(source, destination, 27);
								  
	SWAPBITS(source, destination, 36);
	SWAPBITS(source, destination, 4);
	SWAPBITS(source, destination, 44);
	SWAPBITS(source, destination, 12);
	SWAPBITS(source, destination, 52);
	SWAPBITS(source, destination, 20);
	SWAPBITS(source, destination, 60);
	SWAPBITS(source, destination, 28);
								  
	SWAPBITS(source, destination, 37);
	SWAPBITS(source, destination, 5);
	SWAPBITS(source, destination, 45);
	SWAPBITS(source, destination, 13);
	SWAPBITS(source, destination, 53);
	SWAPBITS(source, destination, 21);
	SWAPBITS(source, destination, 61);
	SWAPBITS(source, destination, 29);
								  
	SWAPBITS(source, destination, 38);
	SWAPBITS(source, destination, 6);
	SWAPBITS(source, destination, 46);
	SWAPBITS(source, destination, 14);
	SWAPBITS(source, destination, 54);
	SWAPBITS(source, destination, 22);
	SWAPBITS(source, destination, 62);
	SWAPBITS(source, destination, 30);
								  
	SWAPBITS(source, destination, 39);
	SWAPBITS(source, destination, 7);
	SWAPBITS(source, destination, 47);
	SWAPBITS(source, destination, 15);
	SWAPBITS(source, destination, 55);
	SWAPBITS(source, destination, 23);
	SWAPBITS(source, destination, 63);
	SWAPBITS(source, destination, 31);
	
	return destination;
}

uint64 DES::pPermutation(uint64 source)
{
	uint64 destination = 0;

	SWAPBITS1st(source, destination, 16);
	SWAPBITS(source, destination, 25);
	SWAPBITS(source, destination, 12);
	SWAPBITS(source, destination, 11);
	SWAPBITS(source, destination, 3);
	SWAPBITS(source, destination, 20);
	SWAPBITS(source, destination, 4);
	SWAPBITS(source, destination, 15);

	SWAPBITS(source, destination, 31);
	SWAPBITS(source, destination, 17);
	SWAPBITS(source, destination, 9);
	SWAPBITS(source, destination, 6);
	SWAPBITS(source, destination, 27);
	SWAPBITS(source, destination, 14);
	SWAPBITS(source, destination, 1);
	SWAPBITS(source, destination, 22);

	SWAPBITS(source, destination, 30);
	SWAPBITS(source, destination, 24);
	SWAPBITS(source, destination, 8);
	SWAPBITS(source, destination, 18);
	SWAPBITS(source, destination, 0);
	SWAPBITS(source, destination, 5);
	SWAPBITS(source, destination, 29);
	SWAPBITS(source, destination, 23);

	SWAPBITS(source, destination, 13);
	SWAPBITS(source, destination, 19);
	SWAPBITS(source, destination, 2);
	SWAPBITS(source, destination, 26);
	SWAPBITS(source, destination, 10);
	SWAPBITS(source, destination, 21);
	SWAPBITS(source, destination, 28);
	SWAPBITS(source, destination, 7);

	return destination;
}

unsigned DES::fCrypt(unsigned right)
{
	register unsigned  blockResult;
	register unsigned offset = 448;
	uint64 extendedRight = extension32To48(right);
	register uint64 key = keyObject->getRoundKey(iterationIndex) ^ extendedRight;
	//key ^= extendedRight ;

	//1
	register unsigned sequnce6Bit = key & 63;//32
	blockResult = S[offset + sequnce6Bit];
	//2
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[offset + sequnce6Bit];
	//3
	offset -= 64;
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[offset + sequnce6Bit];
	//4
	offset -= 64;
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[offset + sequnce6Bit];
	//5
	offset -= 64;
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[offset + sequnce6Bit];
	//6
	offset -= 64;
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[offset + sequnce6Bit];
	//7
	offset -= 64;
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[offset + sequnce6Bit];
	//8
	offset -= 64;
	key >>= 6;
	sequnce6Bit = key & 63;
	blockResult = (blockResult << 4) | S[sequnce6Bit];
	 pPermutation(blockResult); 
	
	 return pPermutation(blockResult);
}

uint64 DES::extension32To48(unsigned rightWord)
{
	uint64 extendedWord = 0;
	
	//( result = ((result << 1) | ((source >> bitIndex) & 1)) )
	SWAPBITS1st(rightWord, extendedWord, 0);
	SWAPBITSGROUP5(rightWord, extendedWord, 27);
	SWAPBITSGROUP(rightWord, extendedWord, 23);
	SWAPBITSGROUP(rightWord, extendedWord, 19);
	SWAPBITSGROUP(rightWord, extendedWord, 15);
	SWAPBITSGROUP(rightWord, extendedWord, 11);
	SWAPBITSGROUP(rightWord, extendedWord, 7);
	SWAPBITSGROUP(rightWord, extendedWord, 3);
	/*extendedWord = (extendedWord << 5) | ((rightWord >> 27 ) & 31);
	extendedWord = (extendedWord << 6) | ((rightWord >> 23 ) & 63);
	extendedWord = (extendedWord << 6) | ((rightWord >> 19 ) & 63);
	extendedWord = (extendedWord << 6) | ((rightWord >> 15 ) & 63);
	extendedWord = (extendedWord << 6) | ((rightWord >> 11 ) & 63);
	extendedWord = (extendedWord << 6) | ((rightWord >> 7 ) & 63);
	extendedWord = (extendedWord << 6) | ((rightWord >> 3 ) & 63);
	extendedWord = (extendedWord << 5) | ((rightWord >> 0 ) & 31);*/
	SWAPBITSGROUP5(rightWord, extendedWord, 0);
	SWAPBITS(rightWord, extendedWord, 31);

	return extendedWord;
}

