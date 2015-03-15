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
	try
	{

		return this;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "" + ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
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
	//For loops in this case are faster than the foreach loop since the arrays are only accessed once
	try
	{
		char isAlgStringValid = false;

		//Check for a normal HashAlg
		for (int i = 0; i < NormalHashAlgs->Length; ++i)
		{
			if (HashAlg == NormalHashAlgs[i])
			{
				return isAlgStringValid = 'N';
			}
		}

		//Check for a keyed HashAlg
		for (int i = 0; i < KeyedHashAlgs->Length; ++i)
		{
			if (HashAlg == KeyedHashAlgs[i])
			{
				return isAlgStringValid = 'K';
			}
		}

		/*
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
		*/

		return isAlgStringValid;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Validate HashAlgString Method Failed: "+ ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

System::String^ Hasher::FormatHash(cli::array<System::Byte>^ UnformatedHash)
{
	try
	{
		//Format the Computed Hash Into a lower case, single String
		System::String^ FormatedHash = "";
		
		/*
		for (size_t i = 0; i < UnformatedHash->Length; ++i)
		{
			System::String^ hash = UnformatedHash[i].ToString("x")->ToLower();
			FormatedHash += (hash->Length == 1 ? "0" : "") + hash;
		}
		*/

		for each (auto byte in UnformatedHash)
		{
			System::String^ hash = byte.ToString("x")->ToLower();
			FormatedHash += (hash->Length == 1 ? "0" : "") + hash;
		}

		return FormatedHash;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Format Hash to String Method Failed: "+ ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

//-- End Static Functions

void Hasher::SetHasherState(System::String^% NameofHashAlg)
{
	try
	{
		//validate args
		if (!(System::String::IsNullOrEmpty(NameofHashAlg) || System::String::IsNullOrWhiteSpace(NameofHashAlg)))
		{
			//check for RIPEMD160
			if ((NameofHashAlg == "RIPEMD160") || (NameofHashAlg == "System.Security.Cryptography.RIPEMD160"))
			{
				//set state
				isUsingKeyedHashAlg = false;
				//set alg
				this->HashAlgorithm = gcnew System::Security::Cryptography::RIPEMD160Managed;
			}
			else
			{
				char alg = isValidHashAlgString(NameofHashAlg);

				if (alg == 'N')
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
					this->HashAlgorithm = System::Security::Cryptography::KeyedHashAlgorithm::Create(NameofHashAlg);
				}
				else
				{
					//throw argument exception
					System::String^ ErrorMessage = "HashAlg does not match a supported Algoritm";

					throw gcnew System::ArgumentException(ErrorMessage, NameofHashAlg);
				}
			}
		}
		else 		//check for easy construction
		{
			//throw argument execption
			System::String^ ErrorMessage = "Argument is not Valid";

			throw gcnew System::ArgumentException(ErrorMessage, NameofHashAlg);
		}
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "SetHashAlg Method Failed: " + ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

Hasher::~Hasher()
{

}