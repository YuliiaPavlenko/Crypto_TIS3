#pragma once
#include <stdio.h>
#include "stdafx.h"

typedef unsigned long long uint64;

class SecureKey {
public:
	SecureKey();
	SecureKey(uint64 userKey);
	~SecureKey();
public:
	void roundsKeysGeneration(bool encryptedFlag);
	static bool weakKeysCheck(const uint64& key);
	static bool partiallyWeakKeysCheck(const uint64 * keys);
	uint64 getRoundKey (char index);
	void setKey(uint64 key);
//	void reverseKeys();
	
private:
	char getShiftingBits(int index);
	void cdPermutation();
	void leftShifting(char bits);
	uint64 calculateSecureKey(const uint64 &source);
	
private:
	char countIterations;
	
	unsigned BlockC;
	unsigned BlockD;

	uint64 originalKey;
	uint64 roundsKeys[16];
};