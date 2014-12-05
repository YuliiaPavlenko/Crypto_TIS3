#include <stdafx.h>
#include "SecureKey.h"
#include "PermutationTables.h"
#include <iostream>
//#include <bitset>

using namespace std;

const int bitsShifting[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
const int kIterationNumber = 16;

#define SWAPBITS(source, result, bitIndex) ( result = ((result << 1) | ((source >> bitIndex) & 1)) )

SecureKey::SecureKey():
	countIterations(0)
{

}

SecureKey::SecureKey(uint64 userKey):
	countIterations(0),
	originalKey(userKey)
{
	//roundsKeys = new uint64[16];
	
	
	cdPermutation();
}

SecureKey::~SecureKey()
{
	//delete[] roundsKeys;
}

void SecureKey::roundsKeysGeneration(bool encryptedFlag)
{

	unsigned countStart[2] = {0, kIterationNumber};
	unsigned countEnd[2] = {kIterationNumber, 0};

		for (unsigned i = countStart[encryptedFlag]; i < countEnd[encryptedFlag]; (encryptedFlag?(i--):(i++))) 
		{
			char shiftedBits = getShiftingBits(i);
			leftShifting(shiftedBits);

			uint64 concatinaitedBlocks = BlockC;
			concatinaitedBlocks = (concatinaitedBlocks << 28) | BlockD;

			roundsKeys[i] = calculateSecureKey(concatinaitedBlocks);

	//		cout << bitset<16>(roundsKeys[i]>>32) <<  " " << bitset<32>(roundsKeys[i])  << endl << endl;
		}
	
}

char SecureKey::getShiftingBits(int index)
{
	return bitsShifting[index];
}

//Применяется ф-ция H для превращения 56-битной последовательности в 48-битный ключ
uint64 SecureKey::calculateSecureKey(const uint64 &source)
{
	uint64 lastCalculatedKey = 0;
	for (int i = 0; i < 48; ++i) {
		lastCalculatedKey <<= 1;
		lastCalculatedKey |= (source >> H[i]) & 1;
	}
	return lastCalculatedKey;
}

//Применяется ф-ция G для удаления битов четности и перестановки оставшихся битов
void SecureKey::cdPermutation()
{
	BlockC = 0;
	BlockD = 0;

	// C

	SWAPBITS(originalKey, BlockC, 7);
	SWAPBITS(originalKey, BlockC, 15);
	SWAPBITS(originalKey, BlockC, 23);
	SWAPBITS(originalKey, BlockC, 31);
	SWAPBITS(originalKey, BlockC, 39);
	SWAPBITS(originalKey, BlockC, 47);
	SWAPBITS(originalKey, BlockC, 55);

	SWAPBITS(originalKey, BlockC, 63);
	SWAPBITS(originalKey, BlockC, 6);
	SWAPBITS(originalKey, BlockC, 14);
	SWAPBITS(originalKey, BlockC, 22);
	SWAPBITS(originalKey, BlockC, 30);
	SWAPBITS(originalKey, BlockC, 38);
	SWAPBITS(originalKey, BlockC, 46);

	SWAPBITS(originalKey, BlockC, 54);
	SWAPBITS(originalKey, BlockC, 62);
	SWAPBITS(originalKey, BlockC, 5);
	SWAPBITS(originalKey, BlockC, 13);
	SWAPBITS(originalKey, BlockC, 21);
	SWAPBITS(originalKey, BlockC, 29);
	SWAPBITS(originalKey, BlockC, 37);

	SWAPBITS(originalKey, BlockC, 45);
	SWAPBITS(originalKey, BlockC, 53);
	SWAPBITS(originalKey, BlockC, 61);
	SWAPBITS(originalKey, BlockC, 4);
	SWAPBITS(originalKey, BlockC, 12);
	SWAPBITS(originalKey, BlockC, 20);
	SWAPBITS(originalKey, BlockC, 28);

	// D

	SWAPBITS(originalKey, BlockD, 1);
	SWAPBITS(originalKey, BlockD, 9);
	SWAPBITS(originalKey, BlockD, 17);
	SWAPBITS(originalKey, BlockD, 25);
	SWAPBITS(originalKey, BlockD, 33);
	SWAPBITS(originalKey, BlockD, 41);
	SWAPBITS(originalKey, BlockD, 49);
	
	SWAPBITS(originalKey, BlockD, 57);
	SWAPBITS(originalKey, BlockD, 2);
	SWAPBITS(originalKey, BlockD, 10);
	SWAPBITS(originalKey, BlockD, 18);
	SWAPBITS(originalKey, BlockD, 26);
	SWAPBITS(originalKey, BlockD, 34);
	SWAPBITS(originalKey, BlockD, 42);

	SWAPBITS(originalKey, BlockD, 50);
	SWAPBITS(originalKey, BlockD, 58);
	SWAPBITS(originalKey, BlockD, 3);
	SWAPBITS(originalKey, BlockD, 11);
	SWAPBITS(originalKey, BlockD, 19);
	SWAPBITS(originalKey, BlockD, 27);
	SWAPBITS(originalKey, BlockD, 35);

	SWAPBITS(originalKey, BlockD, 43);
	SWAPBITS(originalKey, BlockD, 51);
	SWAPBITS(originalKey, BlockD, 59);
	SWAPBITS(originalKey, BlockD, 36);
	SWAPBITS(originalKey, BlockD, 44);
	SWAPBITS(originalKey, BlockD, 52);
	SWAPBITS(originalKey, BlockD, 60);
/*	
	for (int i = 0; i < 28; ++i) {
		BlockC <<= 1;
		BlockC |= (originalKey >> C[i]) & 1;
	}

	for (int i = 0; i < 28; ++i) {
		BlockD <<= 1;
		BlockD |= (originalKey >> D[i]) & 1;
	}
*/
//	cout << bitset<28>(BlockC) << " " << bitset<28>(BlockD)  << endl << endl;


}

//Циклический сдвиг блоков C и D на bits (1 || 2) битов влево
void SecureKey::leftShifting(char bits)
{
	int mask = (1 | bits) << (28 - bits);
	short bit = (BlockC & mask) >> (28 - bits);
	BlockC <<= bits;
	BlockC &= 0x0FFFFFFF;
	BlockC |= bit;
	
	bit = (BlockD & mask) >> (28 - bits);
	BlockD <<= bits;
	BlockD &= 0x0FFFFFFF;
	BlockD |= bit;

//	cout << bitset<28>(BlockC) << " " << bitset<28>(BlockD)  << endl << endl;
}


// Проверка слабого ключа
 
bool SecureKey::weakKeysCheck(const uint64& key)
{
	return (key == 0x0101010101010101)
		|| (key == 0xFEFEFEFEFEFEFEFE)
		|| (key == 0x1F1F1F1F1F1F1F1F)
		|| (key == 0xE0E0E0E0E0E0E0E0);
}

bool SecureKey::partiallyWeakKeysCheck(const uint64 * keys)
{
	weakKeysCheck(keys[0]);
	weakKeysCheck(keys[1]);
	weakKeysCheck(keys[2]);

	return (
			(keys[0] == 0x01FE01FE01FE01FE) & (keys[1] == 0xFE01FE01FE01FE01))
		|| ((keys[0] == 0x1FE01FE01FE01FE0) & (keys[1] == 0xE0F1E0F1E0F1E0F1))
		|| ((keys[0] == 0x01E001E001F101F1) & (keys[1] == 0xE001E001F101F101))
		|| ((keys[0] == 0x1FFE1FFE0EFE0EFE) & (keys[1] == 0xFE1FFE1FFE0EFE0E))
		|| ((keys[0] == 0x011F011F010E010E) & (keys[1] == 0x1F011F010E010E01))
		|| ((keys[0] == 0xE0FEE0FEF1FEF1FE) & (keys[1] == 0xFEE0FEE0FEF1FEF1)
			)

		|| 
			(
			(keys[1] == 0x01FE01FE01FE01FE) & (keys[2] == 0xFE01FE01FE01FE01))
		|| ((keys[1] == 0x1FE01FE01FE01FE0) & (keys[2] == 0xE0F1E0F1E0F1E0F1))
		|| ((keys[1] == 0x01E001E001F101F1) & (keys[2] == 0xE001E001F101F101))
		|| ((keys[1] == 0x1FFE1FFE0EFE0EFE) & (keys[2] == 0xFE1FFE1FFE0EFE0E))
		|| ((keys[1] == 0x011F011F010E010E) & (keys[2] == 0x1F011F010E010E01))
		|| ((keys[1] == 0xE0FEE0FEF1FEF1FE) & (keys[2] == 0xFEE0FEE0FEF1FEF1)
			);
		/*
		|| (key == 0xFE01FE01FE01FE01)
		|| (key == 0xE0F1E0F1E0F1E0F1)
		|| (key == 0xE001E001F101F101)
		|| (key == 0xFE1FFE1FFE0EFE0E)
		|| (key == 0x1F011F010E010E01)
		|| (key == 0xFEE0FEE0FEF1FEF1);
		*/
}
//получение ключа раунда
uint64 SecureKey::getRoundKey (char index)
{
	return roundsKeys[index];
}

void SecureKey::setKey(uint64 key)
{
	originalKey = key;
	cdPermutation();
}

/*не используем

void SecureKey::reverseKeys()
{
	for (int i = 0; i < kIterationNumber/2; ++i)
	{
		uint64 tmp = roundsKeys[i];
		roundsKeys[i] = roundsKeys[kIterationNumber - 1 - i];
		roundsKeys[kIterationNumber - 1 - i] = tmp;
		
	}

}
*/