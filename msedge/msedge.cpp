// msedge.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "msedge.h"

#include <easy/easy.h>
using namespace easy;

//安装卸载
void install(bool isInstall)
{
	eString err_message;

	jsoncpp json;
	if (!json.readConfig("config.json"))
	{
		process.exit("缺少config.json文件");
	}

	eString edge_path = json["edge_path"].asString();
	eString edge_dir = edge_path.to_file_dir();
	edge_dir.pop_back();

	auto my_path = process.exe_path();
	auto my_dir = process.exe_dir();
	my_dir.pop_back();

	json["reg_paths"].forEach([&](jsoncpp& item)
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

			if (!util::write_reg_string(path,name,value))
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
		auto urlsv = cmdstr.Mid("&url=", "&", true);
		if (urlsv.empty()) urlsv = cmdstr.Mid("&url=", "", true);
		if (urlsv.empty()) break;
		eString url = util::url_decode(urlsv);
		auto keysv = url.get_url_paramsv("q");

		if (!keysv.empty())
		{
			urlStr = "https://www.google.com.hk/search?q=";
			urlStr += keysv;
		}
		else {
			urlStr = url;
		}

	} while (false);
	
	if (!urlStr.empty())
	{
		process.run_app(urlStr);
	}
	else {
		box.MessageInfo(cmdstr, "未知参数");
	}
	return 0;
}