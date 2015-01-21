#pragma once

//byte[] hash = new Hasher("sha256").update(data).value;

ref class Hasher : public System::Object
{

public:

	//Class Constructor Takes Name Sets Hash Algorithm
	Hasher( System::String^% HashAlgName );

	//Two updater methods for setting data to be hashed
	Hasher^ update( System::IO::Stream^% );

	Hasher^ update( cli::array<System::Byte>^% );

	//Property by which the hashed result is accessed
	property cli::array<System::Byte>^ value
	{
		cli::array<System::Byte>^ get() { return ComputedHash; }
		void set(cli::array<System::Byte>^ value) { ComputedHash = value; }
	}

	//Class distructor, any resources used are released by this
	~Hasher();

protected:

	//These will get called by update()
	cli::array<System::Byte>^ ComputeHash();

	cli::array<System::Byte>^ FormatHash();

private:

	cli::array<System::Byte>^ ComputedHash;

	bool ByteMode;
	bool StreamMode;

	System::IO::Stream^ StreamToHash;

	array<System::Byte>^ BytesToHash;

	System::Security::Cryptography::HashAlgorithm^ SetHashAlg(System::String^% NameofHashAlg);
	System::Security::Cryptography::HashAlgorithm^ HashAlgorithm;

	System::String^ HashAlgName;

};