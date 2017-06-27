using System;
using System.IO;
using System.Net;
using System.Reflection;
using System.CodeDom;
using System.CodeDom.Compiler;
using System.Web.Services;
using System.Web.Services.Description;
using System.Web.Services.Protocols;
using System.Xml.Serialization;

using System.Data;

namespace csWebSrv2
{
    class Program
    {
        static void Main(string[] args)
        {

            string paraURL = "http://www.webxml.com.cn/WebServices/TrainTimeWebService.asmx";
            string paraService = "TrainTimeWebService";
            string paraMethod = "getStationAndTimeByStationName";
            object[] paraObjs = new object[3];
            paraObjs[0] = "上海";
            paraObjs[1] = "北京";
            paraObjs[2] = "";

            object objResult;

            // 1. 使用 WebClient 下载 WSDL 信息。
            WebClient web = new WebClient();
            Stream stream = web.OpenRead(paraURL + "?WSDL");

            // 2. 创建和格式化 WSDL 文档。
            ServiceDescription description = ServiceDescription.Read(stream);

            // 3. 创建客户端代理代理类。
            ServiceDescriptionImporter importer = new ServiceDescriptionImporter();

            importer.ProtocolName = "Soap"; // 指定访问协议。
            importer.Style = ServiceDescriptionImportStyle.Client; // 生成客户端代理。
            importer.CodeGenerationOptions = CodeGenerationOptions.GenerateProperties | CodeGenerationOptions.GenerateNewAsync;

            importer.AddServiceDescription(description, null, null); // 添加 WSDL 文档。

            // 4. 使用 CodeDom 编译客户端代理类。
            CodeNamespace nmspace = new CodeNamespace(); // 为代理类添加命名空间，缺省为全局空间。
            CodeCompileUnit unit = new CodeCompileUnit();
            unit.Namespaces.Add(nmspace);

            ServiceDescriptionImportWarnings warning = importer.Import(nmspace, unit);
            CodeDomProvider provider = CodeDomProvider.CreateProvider("CSharp");

            CompilerParameters parameter = new CompilerParameters();
            parameter.GenerateExecutable = false;
            parameter.GenerateInMemory = true;
            parameter.ReferencedAssemblies.Add("System.dll");
            parameter.ReferencedAssemblies.Add("System.XML.dll");
            parameter.ReferencedAssemblies.Add("System.Web.Services.dll");
            parameter.ReferencedAssemblies.Add("System.Data.dll");

            CompilerResults result = provider.CompileAssemblyFromDom(parameter, unit);

            // 5. 使用 Reflection 调用 WebService。
            if (!result.Errors.HasErrors)
            {
                Assembly asm = result.CompiledAssembly;
                Type t = asm.GetType(paraService); // 如果在前面为代理类添加了命名空间，此处需要将命名空间添加到类型前面。

                object o = Activator.CreateInstance(t);
                MethodInfo method = t.GetMethod(paraMethod);

                objResult = method.Invoke(o, paraObjs);
                DataSet ds = (DataSet)objResult;
                string str = ds.GetXml();

                Console.WriteLine("sa");
            }

        }
    }
}
