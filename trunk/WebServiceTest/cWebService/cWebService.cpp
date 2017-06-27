#using <System.dll>
#using <System.XML.dll>
#using <System.Web.Services.dll>
#using <System.Data.dll>
using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Reflection;
using namespace System::CodeDom;
using namespace System::CodeDom::Compiler;
using namespace System::Web::Services;
using namespace System::Web::Services::Description;
using namespace System::Web::Services::Protocols;
using namespace System::Xml::Serialization;

using namespace System::Data;

void main(void)
{


	String^ paraURL = gcnew String("http://www.webxml.com.cn/WebServices/TrainTimeWebService.asmx");
	String^ paraService = gcnew String("TrainTimeWebService");
	String^ paraMethod = gcnew String("getStationAndTimeByStationName");

	array<Object^>^ paraObjs = gcnew array<Object^>(3); 

	paraObjs[0] = "上海";
	paraObjs[1] = "北京";
	paraObjs[2] = "";

	Object^ objResult;
	// 1. 使用 WebClient 下载 WSDL 信息。
	WebClient^ web = gcnew WebClient();
	Stream^ stream = web->OpenRead(paraURL + "?WSDL");

	// 2. 创建和格式化 WSDL 文档。
	ServiceDescription^ description = ServiceDescription::Read(stream);

	// 3. 创建客户端代理代理类。
	ServiceDescriptionImporter^ importer = gcnew ServiceDescriptionImporter();

	importer->ProtocolName = "Soap"; // 指定访问协议。
	importer->Style = ServiceDescriptionImportStyle::Client; // 生成客户端代理。
	importer->CodeGenerationOptions = 
		CodeGenerationOptions::GenerateProperties | CodeGenerationOptions::GenerateNewAsync;

	importer->AddServiceDescription(description, nullptr, nullptr); // 添加 WSDL 文档。

	// 4. 使用 CodeDom 编译客户端代理类。
	CodeNamespace^ nmspace = gcnew CodeNamespace(); // 为代理类添加命名空间，缺省为全局空间。
	CodeCompileUnit^ unit = gcnew CodeCompileUnit();
	unit->Namespaces->Add(nmspace);

	ServiceDescriptionImportWarnings^ warning = importer->Import(nmspace, unit);
	CodeDomProvider^ provider = CodeDomProvider::CreateProvider("CSharp");

	CompilerParameters^ parameter = gcnew CompilerParameters();
	parameter->GenerateExecutable = false;
	parameter->GenerateInMemory = true;
	parameter->ReferencedAssemblies->Add("System.dll");
	parameter->ReferencedAssemblies->Add("System.XML.dll");
	parameter->ReferencedAssemblies->Add("System.Web.Services.dll");
	parameter->ReferencedAssemblies->Add("System.Data.dll");

	CompilerResults^ result = provider->CompileAssemblyFromDom(parameter, unit);

	// 5. 使用 Reflection 调用 WebService。
	if (!result->Errors->HasErrors)
	{
		Assembly^ asmb = result->CompiledAssembly;
		Type^ t = asmb->GetType(paraService); // 如果在前面为代理类添加了命名空间，此处需要将命名空间添加到类型前面。

		Object^ o = Activator::CreateInstance(t);
		MethodInfo^ method = t->GetMethod(paraMethod);

		objResult = method->Invoke(o, paraObjs);
			DataSet^ ds = (DataSet^)objResult;
		String^ str = ds->GetXml();
/**/
		Console::WriteLine("sa");
	}


}
