#include "Hasher.h"


Hasher::Hasher(System::String^% HashAlg, System::IO::Stream^% StreamToHash)
{
	//Set the ComputeHash Mode
	this->StreamMode = true;

	//Set the Hash Algorithm
	this->HashAlgorithm = SetHashAlg(HashAlg);
	
	//Set HashAlgName
	this->HashAlgName = HashAlg;

	//Set the Variable to be processed
	this->StreamToHash = StreamToHash;
}

Hasher::Hasher(System::String^% HashAlg, array<System::Byte>^% BytesToHash)
{
	//Set the ComputeHash Mode
	this->ByteMode = true;

	//Set the Hash Algorithm
	this->HashAlgorithm = SetHashAlg(HashAlg);

	//Set HashAlgName
	this->HashAlgName = HashAlg;

	//Set the Variable to be processed
	this->BytesToHash = BytesToHash;
}

Hasher^ Hasher::update(System::IO::Stream^% Stream)
{
	return this;
}

Hasher^ Hasher::update(cli::array<System::Byte>^% Bytes)
{
	return this;
}

cli::array<System::Byte>^ Hasher::ComputeHash()
{
	try
	{
		array<System::Byte>^ ComputedHash;

		//Determine Input Mode and Compute it
		if (this->StreamMode)
		{
			ComputedHash = this->HashAlgorithm->ComputeHash(this->StreamToHash);
		}
		else if (this->ByteMode)
		{
			ComputedHash = this->HashAlgorithm->ComputeHash(this->BytesToHash);
		}

		return *this;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = this->HashAlgName+" Hash Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

cli::array<System::Byte>^ Hasher::FormatHash()
{
	try
	{
		System::String^ FormatedHash = "";
		System::String^ hash = "";

		for each (System::Byte^ byte in unformated_hash)
		{
			hash = byte->ToString("x")->ToLower();
			FormatedHash += (hash->Length == 1 ? "0" : "") + hash;
		}

		return FormatedHash;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Format Hash to String Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

System::Security::Cryptography::HashAlgorithm^ Hasher::SetHashAlg(System::String^% NameofHashAlg)
{
	System::Security::Cryptography::HashAlgorithm^ Algorithm;

	array<System::String^>^ HashArgArray
		= gcnew  array<System::String^>{ "SHA1", "SHA256", "SHA384", "SHA512", "MD5", "RIPEMD160" };
	
	Algorithm = System::Security::Cryptography::MD5::Create();

	if ()
	{

	}

}

Hasher::~Hasher()
{
}