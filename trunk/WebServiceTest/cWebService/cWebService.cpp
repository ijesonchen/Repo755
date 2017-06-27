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

	paraObjs[0] = "�Ϻ�";
	paraObjs[1] = "����";
	paraObjs[2] = "";

	Object^ objResult;
	// 1. ʹ�� WebClient ���� WSDL ��Ϣ��
	WebClient^ web = gcnew WebClient();
	Stream^ stream = web->OpenRead(paraURL + "?WSDL");

	// 2. �����͸�ʽ�� WSDL �ĵ���
	ServiceDescription^ description = ServiceDescription::Read(stream);

	// 3. �����ͻ��˴�������ࡣ
	ServiceDescriptionImporter^ importer = gcnew ServiceDescriptionImporter();

	importer->ProtocolName = "Soap"; // ָ������Э�顣
	importer->Style = ServiceDescriptionImportStyle::Client; // ���ɿͻ��˴���
	importer->CodeGenerationOptions = 
		CodeGenerationOptions::GenerateProperties | CodeGenerationOptions::GenerateNewAsync;

	importer->AddServiceDescription(description, nullptr, nullptr); // ��� WSDL �ĵ���

	// 4. ʹ�� CodeDom ����ͻ��˴����ࡣ
	CodeNamespace^ nmspace = gcnew CodeNamespace(); // Ϊ��������������ռ䣬ȱʡΪȫ�ֿռ䡣
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

	// 5. ʹ�� Reflection ���� WebService��
	if (!result->Errors->HasErrors)
	{
		Assembly^ asmb = result->CompiledAssembly;
		Type^ t = asmb->GetType(paraService); // �����ǰ��Ϊ����������������ռ䣬�˴���Ҫ�������ռ���ӵ�����ǰ�档

		Object^ o = Activator::CreateInstance(t);
		MethodInfo^ method = t->GetMethod(paraMethod);

		objResult = method->Invoke(o, paraObjs);
			DataSet^ ds = (DataSet^)objResult;
		String^ str = ds->GetXml();
/**/
		Console::WriteLine("sa");
	}


}
