#pragma once
#include "AbstractHasher.h"
//byte[] hash = new Hasher("sha256").update(data).value;

/*
	Two Types of hash Algs
	1. Normal
	2. Keyed
		-HMACs
		-MACTRIPLEDES
*/

/*
	Note make a wrapper around SetState, that returns bool
	make this new function the static validate function
*/

ref class Hasher : AbstractHasher
{

public:
	
	//Class Constructor Takes Name Sets Hash Algorithm
	Hasher( System::String^% HashAlgName );

	Hasher( cli::array<System::Byte>^% Key, System::String^% HashAlgName ); //If set with a non-keyed hash alg then key will be set to nullptr

	//Two updater methods for setting data to be hashed
	Hasher^ update( System::IO::Stream^% StreamToHash );

	Hasher^ update( cli::array<System::Byte>^% BytesToHash );

	//if hashalg is a non-keyed then key will be be set to nullptr
	Hasher^ update( cli::array<System::Byte>^% Key, System::IO::Stream^% StreamToHash );

	Hasher^ update( cli::array<System::Byte>^% Key, cli::array<System::Byte>^% BytesToHash );

	//Property by which the key result is accessed
	property cli::array<System::Byte>^ Key
	{
		//If not a keyed hash alg then key returns null
		cli::array<System::Byte>^ get() { return HMACkey; }
		void set(cli::array<System::Byte>^ value) { HMACkey = value; }
	}

	//Property by which the hashed result is accessed
	property cli::array<System::Byte>^ Value
	{
		cli::array<System::Byte>^ get() { return ComputedHash; }
		void set(cli::array<System::Byte>^ value) { ComputedHash = value; }
	}

	//Property by which the hashed result is accessed
	property System::String^ AlgorithmName
	{
		System::String^ get() { return HashAlgName; }
		void set(System::String^ value) { HashAlgName = value; }
	}


	static System::String^ formatHashAsString(cli::array<System::Byte>^ UnFormatedHash);

	static char isValidHashAlgString(System::String^ HashAlg);

	//Class distructor, any resources used are released by this
	~Hasher();

protected:

	//These will get called by update()
	virtual cli::array<System::Byte>^ ComputeHash();

private:

	//Internal Properties
	property System::Security::Cryptography::HashAlgorithm^ HashAlgorithm;
	property System::Security::Cryptography::KeyedHashAlgorithm^ HMAC;
	property bool isUsingKeyedHashAlg;

	//Distingish between keyed and non-keyed
	void setHasherStateAndMode(System::String^% NameofHashAlg);

	//Class State Information
	bool ByteMode;
	bool StreamMode;

	//Internal Types accessed by the class's public properties
	System::String^ HashAlgName;
	cli::array<System::Byte>^ ComputedHash;
	cli::array<System::Byte>^ HMACkey;

	//Static hash alg name arrays, Contains The valid list of constructor strings
	static array<System::String^>^ KeyedHashAlgs = { "System.Security.Cryptography.HMAC",
		"System.Security.Cryptography.KeyedHashAlgorithm",
		"HMACMD5",
		"System.Security.Cryptography.HMACMD5",
		"HMACRIPEMD160",
		"System.Security.Cryptography.HMACRIPEMD160",
		"HMACSHA1",
		"System.Security.Cryptography.HMACSHA1",
		"HMACSHA256",
		"System.Security.Cryptography.HMACSHA256",
		"HMACSHA384",
		"System.Security.Cryptography.HMACSHA384",
		"HMACSHA512",
		"System.Security.Cryptography.HMACSHA512",
		"MACTripleDES",
		"System.Security.Cryptography.MACTripleDES" };

	static array<System::String^>^ NormalHashAlgs = { "RIPEMD160",
		"System.Security.Cryptography.RIPEMD160",
		"SHA",
		"SHA1",
		"System.Security.Cryptography.SHA1",
		"System.Security.Cryptography.HashAlgorithm",
		"MD5",
		"System.Security.Cryptography.MD5",
		"SHA256",
		"SHA-256",
		"System.Security.Cryptography.SHA256",
		"SHA384",
		"SHA-384",
		"System.Security.Cryptography.SHA384",
		"SHA512",
		"SHA-512",
		"System.Security.Cryptography.SHA512" };
};