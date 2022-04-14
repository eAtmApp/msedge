// msedge.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "msedge.h"

#include <easy/easy.h>
using namespace easy;

jsoncpp jsonConfig;

//安装卸载
void install(bool isInstall)
{
	eString err_message;

	eString edge_path = jsonConfig["edge_path"].asString();
	eString edge_dir = edge_path.to_file_dir();
	edge_dir.pop_back();

	auto my_path = process.exe_path();
	auto my_dir = process.exe_dir();
	my_dir.pop_back();

	jsonConfig["reg_paths"].forEach([&](jsoncpp& item)
		{
			eString path = item["path"].asString();
			eString name = item["name"].asString();
			eString value = item["value"].asString();

			if (isInstall)
			{
				value.replaceAll("[EXE]", my_path);
				value.replaceAll("[DIR]", my_dir);
			}
			else {
				value.replaceAll("[EXE]", edge_path);
				value.replaceAll("[DIR]", edge_dir);
			}

			if (!util::write_reg_string(path, name, value))
			{
				if (!err_message.empty()) err_message += "\r\n";
				err_message += util::Format("与入路径失败:{}", path);
			}
		});

	if (!err_message.empty())
	{
		box.MessageError(err_message, util::Format("{}失败!", isInstall ? "安装" : "卸载"));
	}
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	eString cmdstr(lpCmdLine);

	if (!jsonConfig.readConfig("config.json"))
	{
		process.exit("缺少config.json文件");
	}

	console.set_logfile();
	process.set_exception_dump(false);

	bool isInstall = cmdstr.compare_icase("/install");
	bool isUninstall = cmdstr.compare_icase("/uninstall");
	if (isInstall || isUninstall)
	{
		install(isInstall);
		return 0;
	}

	eString urlStr;

	do
	{
		console.log("{}", cmdstr);
		eString url = cmdstr.get_url_paramsv("url");
		if (url.empty()) break;

		if (url.find("://") == std::string::npos) url = util::url_decode(url);

		eStringV keysv = url.get_url_paramsv("q");

		if (!keysv.empty())
		{
			console.log("key:{}", keysv);

			//搜索类型 %3A为:的编码
			eStringV subType = keysv.Mid("", "%3A");
			
			//匹配配置文件
			if (!subType.empty() && jsonConfig["urls"][subType].isString())
			{
				urlStr = jsonConfig["urls"][subType].asString();
				keysv = keysv.substr(subType.size() + 3);
			}

			//如果没有则使用缺省的搜索引擎
			if (urlStr.empty() && jsonConfig["urls"]["default"].isString())
			{
				urlStr = jsonConfig["urls"]["default"].asString();
			}
			
			//如果再没有就使用google
			if (urlStr.empty())
			{
				urlStr = "https://www.google.com.hk/search?q=[KEY]";
			}

			urlStr.replaceAll("[KEY]", keysv);
		}
		else {
			urlStr = url;
			if (urlStr.find("://") == std::string::npos)
			{
				urlStr = util::url_decode(urlStr);
			}
		}
		console.log("url:{}", urlStr);
	} while (false);

	if (!urlStr.empty())
	{
		process.run_app(urlStr);
	}
	else {
		console.log("未知参数:{}", lpCmdLine);
		eString edge_path = jsonConfig["edge_path"].asString();
		if (!edge_path.empty())
		{
			process.run_app(edge_path, lpCmdLine);
		}
	}
	return 0;
}