// File Digester.h

#pragma once

using namespace System;
using namespace System::Management::Automation;

namespace FileDigester {

	//[CmdletAttribute(VerbsCommon::Get, "HashFromFile")]
	[Cmdlet(VerbsCommon::Get, "Hash", DefaultParameterSetName = "FilePath")]
	public ref class Digest : public PSCmdlet
	{

	public:		
		/*
			Currently there are four usage senerios in mind for this Cmdlet
			1. String to a File
			2. FileInfo
			3. Stream
			4. Byte Array
		*/

		//Senerio 1, FilePath
		[ParameterAttribute(Mandatory = true, Position = 0,
			ValueFromPipeline = true, ValueFromPipelineByPropertyName = true,
			HelpMessage = "Path to the File, you want Digest",
			ParameterSetName = "FilePath")]
		[ValidateNotNullAttribute]
		property System::String^ FilePath;

		//Senerio 2, FileInfo Object 		//Essentally we want pass through of the file info object
		[ParameterAttribute(Mandatory = true, Position = 0,
			ValueFromPipeline = true, ValueFromPipelineByPropertyName = true,
			HelpMessage = "The File, you want Digest",
			ParameterSetName = "FileInfo")]
		[ValidateNotNullAttribute]
		property System::IO::FileInfo^ File;

		//Senerio 3, Stream Object
		[ParameterAttribute(Mandatory = true, Position = 0,
			ValueFromPipeline = true, ValueFromPipelineByPropertyName = true,
			HelpMessage = "The Stream, you want Digest",
			ParameterSetName = "Stream")]
		[ValidateNotNullAttribute]
		property System::IO::Stream^ Stream;

		//Senerio 4, Byte Array Object
		[ParameterAttribute(Mandatory = true, Position = 0,
			ValueFromPipeline = true, ValueFromPipelineByPropertyName = true,
			HelpMessage = "The Stream, you want Digest",
			ParameterSetName = "Stream")]
		[ValidateNotNullAttribute]
		property cli::array<System::Byte>^ Bytes;

		[ParameterAttribute(Mandatory = true, Position = 1,
			HelpMessage = "Hash Algorithm you want to use")]
		[ValidateNotNullAttribute]
		property System::String^ HashAlg;

		//Input for hashalg key


	protected:
		//-- These functions represent the life cycle of our cmdlet, They are overriden from the cmdlet class and Called by the Powershell Runtime
		
		//-- The life cycle functions can execute on different threads
		/*
		BeginProcessing, is always called by the runtime first

		ProcessRecord is called if you override it, or if one of your parameters accepts pipeline input
		! A Critical Note! about ProcessRecord it's called for every object in the pipeline
		and It's not garanteed by the runtime that these calls will occur on the same thread, SO IT'S NOT THREAD SAFE
		The PowerShell Development section on MSDN even states this! So, we have to make it thread safe.

		EndProcessing is always called last
		*/

		virtual void BeginProcessing(void) override;
		/*Used for One-time optional pre-processing
			-In this case begin processing will determine the input senerio
			-It will validate the arguments with a single input
				-HashAlg String
				-HashAlg Key

		*/

		virtual void ProcessRecord(void) override;
		//Used to provide record by record processing can be called more than one depending on input, if ValueFromPipeline is present

		virtual void EndProcessing(void) override;
		//Used for One-time optional post processing

		virtual void StopProcessing(void) override;
		//Used for if the user Terminates the command( CTRL+C )

	private:

		static array<System::String^>^ HashArgArray = { "SHA1", "SHA256", "SHA384", "SHA512", "MD5", "RIPEMD160" };

		//Helper Functions
		System::IO::FileStream^ Get_Open_File_Stream(System::String^);
		//get random number
		//get key


		//Normal Hash Algs
		static array<System::Byte>^ MD5_Hash_File(System::IO::FileStream^% FiletoHash);
		static array<System::Byte>^ RIPEMD160_Hash_File(System::IO::FileStream^% FiletoHash);
		static array<System::Byte>^ SHA1_Hash_File(System::IO::FileStream^% FiletoHash);
		static array<System::Byte>^ SHA256_Hash_File(System::IO::FileStream^% FiletoHash);
		static array<System::Byte>^ SHA384_Hash_File(System::IO::FileStream^% FiletoHash);
		static array<System::Byte>^ SHA512_Hash_File(System::IO::FileStream^% FiletoHash);


		//HMAC Hash Algs


		System::String^ Format_Hash_to_String(array<System::Byte>^%);

		//Error Strings
		static ref class ErrorStrings
		{
		public:
			//-- Class Constructors and Destructors
			ErrorStrings(); //let the compiler generate them, we only care about the types in the class
			~ErrorStrings();

			//-- Function Names for Internal use
			System::String^ const BeginProcessingName = "Begin Processing";
			System::String^ const ProcessRecordName = "Process Record";
			System::String^ const EndProcessingName = "End Processing";
			System::String^ const StopProcessingName = "Stop Processing";

			//-- Argument Name Strings for interal use
			System::String^ FilePath = "FilePath";
			System::String^ HashAlgArg = "HashAlg";

			//-- Possible Error Condition Implemented in this Class
			System::String^ IsNull = ": Is Null";
			System::String^ IsWhiteSpace = ": Is White Space";
			System::String^ IsNotaNumber = ": Is not a Number: ";
			System::String^ IsLessthanZero = ": Is less than or equal to Zero";

			//-- Error Result Conditions
			System::String^ StackTrace = "\n\nStack Trace: ";
			System::String^ BaseStackTrace = "\n\nBase Stack Trace: ";
			System::String^ Failed = " Failed, ";
			System::String^ UnexpectedManaged = ": Unexpected exception, Managed type thrown";
		};


		//This is the wrapper we use for copying data between beginprocessing and ProcessRecord
		ref struct ArgumentWrapper
		{
			System::String^ HashAlg;
		};

		//Global Structures/Classes
		ArgumentWrapper^ InputArgs = gcnew ArgumentWrapper;
	};
}
