// This is the main DLL file.

#include "Power Digester.h"

using namespace FileDigester;

void Digest::BeginProcessing(void)
{
	ErrorStrings^ BeginProcessingErrors = gcnew ErrorStrings;
	try
	{
		//Arg
		array<System::String^>^ HashArgArray 
			= gcnew  array<System::String^>{ "SHA1", "SHA256", "SHA384", "SHA512", "MD5", "RIPEMD160" };

		//Validate HashAlg String
		if (System::String::IsNullOrEmpty(HashAlg))
		{
			throw gcnew System::ArgumentException(BeginProcessingErrors->HashAlgArg 
				+ BeginProcessingErrors->IsNull, BeginProcessingErrors->HashAlgArg);
		}
		if (System::String::IsNullOrWhiteSpace(HashAlg))
		{
			throw gcnew System::ArgumentException(BeginProcessingErrors->HashAlgArg
				+ BeginProcessingErrors->IsWhiteSpace, BeginProcessingErrors->HashAlgArg);
		}

		bool condition = false;

		for (int i = 0; i < HashArgArray->Length; i++)
		{
			if (HashAlg == HashArgArray[i])
			{
				condition = true;
				break;
			}
		}

		if (!condition)
		{
			throw gcnew System::ArgumentException(BeginProcessingErrors->HashAlgArg
				+ "Is not a valid Hash Alg", HashAlg);
		}

		//Copy Hash Alg String to argument wrapper
		InputArgs->HashAlg = HashAlg;

	}
	catch (System::Exception ^ex)
	{
		System::Management::Automation::ErrorCategory error_category;

		Cmdlet::WriteVerbose("Error happened Terminating Cmdlet");
		Cmdlet::WriteDebug(ex->GetType()->ToString() + " :Caught Terminating Cmdlet");

		if (ex->GetBaseException()->GetType()->ToString() == "System.ArgumentException")
		{
			error_category = System::Management::Automation::ErrorCategory::InvalidArgument;
		}
		else if (ex->GetBaseException()->GetType()->ToString() == "System.IO.FileNotFoundException")
		{
			error_category = System::Management::Automation::ErrorCategory::ObjectNotFound;
		}
		else
		{
			error_category = System::Management::Automation::ErrorCategory::InvalidResult;
		}

		System::String^ fully_qualified_error_id =
			BeginProcessingErrors->BeginProcessingName + BeginProcessingErrors->Failed + ex->Message + BeginProcessingErrors->StackTrace
			+ ex->StackTrace + BeginProcessingErrors->BaseStackTrace + ex->GetBaseException()->StackTrace;

		//Most Likely Error in this case is Permission Denied, since we are grabing a file and looking at a directory
		System::Management::Automation::ErrorRecord ^error
			= gcnew System::Management::Automation::ErrorRecord(ex, fully_qualified_error_id, error_category, ex->GetBaseException()->Message);

		//Throw a terminating Error, This end execution of the cmdlet
		System::Management::Automation::Cmdlet::ThrowTerminatingError(error);
	}
}

void Digest::ProcessRecord(void)
{
	ErrorStrings^ ProcessErrors = gcnew ErrorStrings;
	try
	{

		//-- Get Validated HashAlg String
		System::Threading::Monitor::Enter(InputArgs); //This will wait for unlock if locked in another thread

		ArgumentWrapper^ ProcessArgs{ InputArgs }; //C++14 brace-copy

		System::Threading::Monitor::Exit(InputArgs);

		//-- Validate input args
		if (System::String::IsNullOrEmpty(FilePath)) 
		{
			throw gcnew System::ArgumentException(ProcessErrors->FilePath + ProcessErrors->IsNull, ProcessErrors->FilePath);
		}
		if (System::String::IsNullOrWhiteSpace(FilePath))
		{
			throw gcnew System::ArgumentException(ProcessErrors->FilePath + ProcessErrors->IsWhiteSpace, ProcessErrors->FilePath);
		}

		//Get filesteam from file
		System::IO::FileStream^ FiletoHash = this->Get_Open_File_Stream(FilePath);

		//System::IO::FileStream^ file 
		array<System::Byte>^ ByteHash;
		
		if (ProcessArgs->HashAlg == "MD5")
		{
			ByteHash = this->MD5_Hash_File(FiletoHash);
		}
		else if (ProcessArgs->HashAlg == "SHA1")
		{
			ByteHash = this->SHA1_Hash_File(FiletoHash);
		}
		else if (ProcessArgs->HashAlg == "SHA256")
		{
			ByteHash = this->SHA256_Hash_File(FiletoHash);
		}
		else if (ProcessArgs->HashAlg == "SHA384")
		{
			ByteHash = this->SHA384_Hash_File(FiletoHash);
		}
		else if (ProcessArgs->HashAlg == "SHA512")
		{
			ByteHash = this->SHA512_Hash_File(FiletoHash);
		}
		else if (ProcessArgs->HashAlg == "RIPEMD160")
		{
			ByteHash = this->RIPEMD160_Hash_File(FiletoHash);
		}

		FiletoHash->Close();

		System::String^ Ouput = this->Format_Hash_to_String(ByteHash);

		Cmdlet::WriteObject(Ouput);
	}
	catch (System::Exception ^ex)
	{
		System::Management::Automation::ErrorCategory error_category;

		Cmdlet::WriteVerbose("Error happened Terminating Cmdlet");
		Cmdlet::WriteDebug(ex->GetType()->ToString() + " :Caught Terminating Cmdlet");

		if (ex->GetBaseException()->GetType()->ToString() == "System.ArgumentException")
		{
			error_category = System::Management::Automation::ErrorCategory::InvalidArgument;
		}
		else if (ex->GetBaseException()->GetType()->ToString() == "System.IO.FileNotFoundException")
		{
			error_category = System::Management::Automation::ErrorCategory::ObjectNotFound;
		}
		else
		{
			error_category = System::Management::Automation::ErrorCategory::InvalidResult;
		}

		System::String^ fully_qualified_error_id =
			ProcessErrors->ProcessRecordName + ProcessErrors->Failed + ex->Message + ProcessErrors->StackTrace
			+ ex->StackTrace + ProcessErrors->BaseStackTrace + ex->GetBaseException()->StackTrace;

		//Most Likely Error in this case is Permission Denied, since we are grabing a file and looking at a directory
		System::Management::Automation::ErrorRecord ^error
			= gcnew System::Management::Automation::ErrorRecord(ex, fully_qualified_error_id, error_category, ex->GetBaseException()->Message);

		//Throw a terminating Error, This end execution of the cmdlet
		System::Management::Automation::Cmdlet::WriteError(error);
	}
}

//These two where not implemented because they were not needed in cleanup or error handling 
void Digest::EndProcessing(void)
{

}

void Digest::StopProcessing(void)
{

}

System::IO::FileStream^ Digest::Get_Open_File_Stream(System::String^ Path)
{
	System::IO::FileStream^ FiletoHash;
	try
	{
		//error conditions
		System::String^ File_Does_Not_Exist = "The File Does not exist";
		System::String^ Is_Directory = " Is a Directory";

		if (System::IO::File::Exists(Path))
		{
			FiletoHash = gcnew System::IO::FileStream(Path,
				System::IO::FileMode::Open,
				System::IO::FileAccess::Read,
				System::IO::FileShare::ReadWrite);
		}
		else if (System::IO::Directory::Exists(Path))
		{
			throw gcnew System::IO::FileLoadException(Is_Directory, Path);
		}
		else
		{
			throw gcnew System::IO::FileNotFoundException(File_Does_Not_Exist, Path);
		}

		return FiletoHash;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "Get File Stream Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

array<System::Byte>^ Digest::MD5_Hash_File(System::IO::Stream^% FiletoHash)
{
	try
	{
		array<System::Byte>^ MD5Hash;

		System::Security::Cryptography::MD5^ HashFunction
			= System::Security::Cryptography::MD5::Create();

		MD5Hash = HashFunction->ComputeHash(FiletoHash);


		return MD5Hash;
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "MD5 Hash Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

array<System::Byte>^ Digest::SHA1_Hash_File(System::IO::FileStream^% FiletoHash)
{
	try
	{
		//-- Create Return Object
		array<System::Byte>^ SHA1Hash;

		//-- Create Instance of Hash Class
		System::Security::Cryptography::SHA1^ HashFunction
			= System::Security::Cryptography::SHA1::Create();

		//-- Compute the new byte array from the file
		return SHA1Hash = HashFunction->ComputeHash(FiletoHash);
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "SHA1 Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

array<System::Byte>^ Digest::SHA256_Hash_File(System::IO::FileStream^% FiletoHash)
{
	try
	{
		array<System::Byte>^ SHA256Hash;

		System::Security::Cryptography::SHA256^ HashFunction
			= System::Security::Cryptography::SHA256::Create();

		return SHA256Hash = HashFunction->ComputeHash(FiletoHash);
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "SHA256 Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

array<System::Byte>^ Digest::SHA384_Hash_File(System::IO::FileStream^% FiletoHash)
{
	try
	{
		array<System::Byte>^ SHA384Hash;

		System::Security::Cryptography::SHA384^ HashFunction
			= System::Security::Cryptography::SHA384::Create();

		return SHA384Hash = HashFunction->ComputeHash(FiletoHash);
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "SHA384 Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

array<System::Byte>^ Digest::SHA512_Hash_File(System::IO::FileStream^% FiletoHash)
{
	try
	{
		array<System::Byte>^ SHA512Hash;

		System::Security::Cryptography::SHA512^ HashFunction
			= System::Security::Cryptography::SHA512::Create();

		return SHA512Hash = HashFunction->ComputeHash(FiletoHash);
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "SHA512 Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

array<System::Byte>^ Digest::RIPEMD160_Hash_File(System::IO::FileStream^% FiletoHash)
{
	try
	{
		array<System::Byte>^ RIPEMD160Hash;

		System::Security::Cryptography::RIPEMD160^ HashFunction
			= System::Security::Cryptography::RIPEMD160::Create();

		return RIPEMD160Hash = HashFunction->ComputeHash(FiletoHash);
	}
	catch (System::Exception^ ex)
	{
		System::String^ FunctionErrorMessage = "RIPEMD160 Failed";

		throw gcnew System::Exception(FunctionErrorMessage, ex);
	}
}

System::String^ Digest::Format_Hash_to_String(array<System::Byte>^% unformated_hash)
{
	try
	{
		System::String^ FormatedHash = "";
		System::String^ hash = "";

		for each ( System::Byte^ byte in unformated_hash)
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

//Let the complier make these
Digest::ErrorStrings::ErrorStrings(){}

Digest::ErrorStrings::~ErrorStrings(){}