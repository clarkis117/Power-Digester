#pragma once

ref class Hasher : System::Object
{

public:

	Hasher( System::String^% HashAlg, System::IO::Stream^% );
	Hasher( System::String^% HashAlg, array<System::Byte>^% );

	cli::array<System::Byte>^ ComputeHash();
	cli::array<System::Byte>^ FormatHash();

	~Hasher();

protected:
private:

	bool ByteMode;
	bool StreamMode;

	System::IO::Stream^ StreamToHash;
	array<System::Byte>^ BytesToHash;

	System::Security::Cryptography::HashAlgorithm^ SetHashAlg(System::String^% NameofHashAlg);
	System::Security::Cryptography::HashAlgorithm^ HashAlgorithm;

	System::String^ HashAlgName;

};

