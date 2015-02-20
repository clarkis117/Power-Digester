#include "Hasher.h"


//-- Class Constructors

Hasher::Hasher(System::String^% HashAlg)
{
	try
	{
		//validate HashAlg String
		if (!(System::String::IsNullOrEmpty(HashAlg) || System::String::IsNullOrWhiteSpace(HashAlg)))
		{
			//Check for Null After Setting hash alg
			//Set the Hash Algorithm
			SetHasherState(HashAlg);
			//then set algorithm
		}
		else
		{
			throw gcnew System::ArgumentException("");
		}



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
	System::IO::Stream^ hi = gcnew System::IO::FileStream(System::IO::File::Create("tst"));
	this->update(hi);
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
/*
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
*/
//-- Static Functions

char Hasher::isValidHashAlgString(System::String^ HashAlg)
{
	try
	{
		char isAlgStringValid = false;

		//Check for a normal HashAlg
		for each (auto algString in NormalHashAlgs)
		{
			if (HashAlg == algString)
			{
				return isAlgStringValid = 'N';
			}
		}

		//Chech for a keyed HashAlg
		for each (auto algString in KeyedHashAlgs)
		{
			if (HashAlg == algString)
			{
				return isAlgStringValid = 'K';
			}
		}

		return isAlgStringValid;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Validate HashAlgString Method Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
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

void Hasher::SetHasherState(System::String^% NameofHashAlg)
{
	try
	{
		char alg = isValidHashAlgString(NameofHashAlg);

		if (alg == false)
		{
			throw gcnew System::ArgumentException("", NameofHashAlg);
		}
		else if (NameofHashAlg == "RIPEMD160" || NameofHashAlg == "System.Security.Cryptography.RIPEMD160") //check for RIPMD160
		{
			//set state
			isUsingKeyedHashAlg = false;
			//set alg
			this->HashAlgorithm = gcnew System::Security::Cryptography::RIPEMD160Managed;
		}
		else if (alg == 'N')
		{
			//set states
			this->isUsingKeyedHashAlg = false;
			//set alg
			this->HashAlgorithm = System::Security::Cryptography::HashAlgorithm::Create(NameofHashAlg);
		}
		else if (alg == 'K')
		{
			//set state
			this->isUsingKeyedHashAlg = true;
			//set alg
			this->HashAlgorithm = System::Security::Cryptography::HMAC::Create(NameofHashAlg);
		}

	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "SetHashAlg Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

Hasher::~Hasher()
{

}