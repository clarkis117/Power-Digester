
ref class AbstractHasher abstract : System::Object
{
private:

	//Internal Types accessed by the class's public properties
	System::String^ HashAlgName;

	cli::array<System::Byte>^ ComputedHash;


public:

	//Default Constructor
	AbstractHasher(System::String^% HashAlg);

	//--Two updater methods for setting data to be hashed
	virtual AbstractHasher^ update(System::IO::Stream^% StreamToHash);

	virtual AbstractHasher^ update(cli::array<System::Byte>^% BytesToHash);

	//-- Class properties
	property cli::array<System::Byte>^ Value
	{
		cli::array<System::Byte>^ get() { return ComputedHash; }
		void set(cli::array<System::Byte>^ value) { ComputedHash = value; }
	}

	property System::String^ Algorithm
	{
		System::String^ get() { return HashAlgName; }
		void set(System::String^ value)
		{ HashAlgName = value; }
	}

	System::String^ isValidHashAlg(System::String^ HashAlg);

	static System::String^ formatHashAsString(cli::array<System::Byte>^ UnFormatedHash);


protected:

	virtual cli::array<System::Byte>^ computeHash();

	//This grabs an array of valid alg strings
	virtual cli::array<System::String^>^ getValidHashArgList();

	//This replaces set state, Argument is validated by the time it reaches here
	virtual void setHashAlgorithm(System::String^ HashAlgName);

};

//Class constructor
AbstractHasher::AbstractHasher(System::String^% HashAlgName)
{
	try
	{
		if (!(System::String::IsNullOrEmpty(HashAlgName) && System::String::IsNullOrWhiteSpace(HashAlgName)))
		{
			//Set the state
			System::String^ Alg = this->isValidHashAlg(HashAlgName);

			if (Alg != nullptr)
			{
				this->setHashAlgorithm(Alg);
			}
			else //else if there is no match
			{
				throw gcnew System::ArgumentException("No matching algorithm found");
			}
		}
		else
		{
			throw gcnew System::ArgumentNullException("HashAlgName"+HashAlgName);
		}
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Hasher: " + ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

/*
	This method validates the input argument for constructing the hash algorithm class.
	->the virtual method getValidHashArg is called in this method.
	->returns null if no match is found

*/
System::String^ AbstractHasher::isValidHashAlg(System::String^ HashAlg)
{
	try
	{
		if (!(System::String::IsNullOrEmpty(HashAlg) || System::String::IsNullOrWhiteSpace(HashAlg)))
		{
			//Get the arrayy containing the valid strings
			cli::array<System::String^>^ ValidHashArgList = this->getValidHashArgList();
			
			//Check for null 
			if (ValidHashArgList != nullptr)
			{
				for (int i = 0; i < ValidHashArgList->Length; ++i)
				{
					//match
					if (HashAlg->ToLower() == ValidHashArgList[i]->ToLower())
					{
						//if match found return element form array
						return ValidHashArgList[i];
					}
				}

				//no match has been found return null
				return nullptr;
			}
			else //throw not implemented if null
			{
				throw gcnew System::NotImplementedException("getValidHashArgList: is invalid or not implemented properly");
			}
		}
		else  //throw null argument execption
		{
			throw gcnew System::ArgumentNullException(HashAlg);
		}
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Validate HashAlgString Method Failed: " + ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

System::String^ AbstractHasher::formatHashAsString(cli::array<System::Byte>^ UnformatedHash)
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
		System::String^ FunctionErrorMessage = "Format Hash to String Method Failed: " + ex->Message;

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}