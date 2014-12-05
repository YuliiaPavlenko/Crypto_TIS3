#pragma once

template < typename blockType > class Crypt
{
public:
	bool isEncrypted;
	virtual blockType makeCrypt(blockType block) = 0;
};
