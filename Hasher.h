#pragma once

//byte[] hash = new Hasher("sha256").update(data).value;

/*
	Two Types of hash Algs
	1. Normal
	2. Keyed
		-HMACs
		-MACTRIPLEDES
*/

ref class Hasher : public System::Object
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
	property cli::array<System::Byte>^ key
	{
		//If not a keyed hash alg then key returns null
		cli::array<System::Byte>^ get() { return Key; }
		void set(cli::array<System::Byte>^ value) { Key = value; }
	}

	//Property by which the hashed result is accessed
	property cli::array<System::Byte>^ value
	{
		cli::array<System::Byte>^ get() { return ComputedHash; }
		void set(cli::array<System::Byte>^ value) { ComputedHash = value; }
	}

	static System::String^ FormatHash(cli::array<System::Byte>^);

	static bool ValidateHashAlgString(System::String^ HashAlg);

	//Class distructor, any resources used are released by this
	~Hasher();

protected:

	//These will get called by update()
	cli::array<System::Byte>^ ComputeHash();

private:

	//Constructor set variables
	System::String^ HashAlgName;
	System::Security::Cryptography::HashAlgorithm^ HashAlgorithm;
	System::Security::Cryptography::HashAlgorithm^ SetHashAlg(System::String^% NameofHashAlg);

	//Class State Information
	bool Keyed_HashAlg;
	bool ByteMode;
	bool StreamMode;

	//Internal Types accessed by the class properties
	cli::array<System::Byte>^ ComputedHash;
	cli::array<System::Byte>^ Key;



	System::IO::Stream^ StreamToHash;

	array<System::Byte>^ BytesToHash;
};