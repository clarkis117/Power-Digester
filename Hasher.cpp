#include "Hasher.h"


//-- Class Constructors

Hasher::Hasher(System::String^% HashAlg)
{
	try
	{
		//validate HashAlg String
		if (!(System::String::IsNullOrEmpty(HashAlg) || System::String::IsNullOrWhiteSpace(HashAlg)))
		{
			//Validate Algorithm name, on second thought validation here seems redundant
			//Check for Null After Setting hash alg

			//then set algorithm
		}

		//Set the Hash Algorithm
		this->HashAlgorithm = SetHashAlg(HashAlg);

		if (this->HashAlgorithm != nullptr)
		{

		}
		//Set HashAlgName
		this->HashAlgName = HashAlg;
	}
	catch (System::Exception^ ex)
	{

	}
}

Hasher::Hasher(cli::array<System::Byte>^% Key, System::String^% HashAlgName)
{
	//Validate HashAlg String

	//Validate Key

}

//-- End Class Constructors
//-- Update Functions

Hasher^ Hasher::update(System::IO::Stream^% Stream)
{
	return this;
}

Hasher^ Hasher::update(cli::array<System::Byte>^% Bytes)
{
	return this;
}

Hasher^ Hasher::update(cli::array<System::Byte>^% Key, System::IO::Stream^% StreamToHash)
{
	return this;
}

Hasher^ Hasher::update(cli::array<System::Byte>^% Key, cli::array<System::Byte>^% BytesToHash)
{
	return this;
}

// --End Update Functions

cli::array<System::Byte>^ Hasher::ComputeHash()
{
	try
	{
		array<System::Byte>^ hashcomputed;

		//Determine Input Mode and Compute it
		if (this->StreamMode)
		{
			hashcomputed = this->HashAlgorithm->ComputeHash(this->StreamToHash);
		}
		else if (this->ByteMode)
		{
			hashcomputed = this->HashAlgorithm->ComputeHash(this->BytesToHash);
		}

		return ;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = this->HashAlgName+"Hash Computation Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

//-- Static Functions

bool Hasher::ValidateHashAlgString(System::String^ HashAlg)
{
	array<System::String^>^ HashArgArray
		= gcnew  array<System::String^>{ "SHA", "SHA1", "System.Security.Cryptography.SHA1",
										"SHA256", "SHA384", "SHA512", "MD5", "RIPEMD160" };
	return bool;
}

System::String^ Hasher::FormatHash(cli::array<System::Byte>^ UnformatedHash)
{
	try
	{
		//Format the Computed Hash Into a lower case, single String
		System::String^ FormatedHash = "";
		
		for each (System::Byte^ byte in UnformatedHash)
		{
			System::String^ hash = byte->ToString("x")->ToLower();
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

//-- End Static Functions

System::Security::Cryptography::HashAlgorithm^ Hasher::SetHashAlg(System::String^% NameofHashAlg)
{
	//RIPEMD needs seperate mapp
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