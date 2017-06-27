using System;

namespace csWebServiceStatic
{
    class Program
    {
        static void Main(string[] args)
        {
            WeatherWebService.WeatherWebService w = new WeatherWebService.WeatherWebService();
            //把webservice当做一个类来操作  
            string[] s = new string[23];//声明string数组存放返回结果  
            string city = "上海";//获得文本框录入的查询城市  
            s = w.getWeatherbyCityName(city);
            //以文本框内容为变量实现方法getWeatherbyCityName  
            if (s[8] == "")
            {
            }
            else
            {
                for (int ii = 0; ii < 23; ii++)
                {
                    if (s[ii].Length > 0)
                        Console.WriteLine(ii.ToString() + " : " + s[ii]);
                }
            }  
        }
    }
}
