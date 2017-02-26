#pragma once
class HashedString
{
public:
	HashedString();
	~HashedString();


	unsigned int Hash(const char * i_string);
	unsigned int Hash(const void * i_bytes, unsigned int i_bytecount);
};

