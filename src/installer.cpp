#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#include <fstream>
#include <iostream>
#include <string>

#pragma comment(lib, "ole32.lib")

// 由 IExpress 解压到临时目录后运行：复制文件、建快捷方式、写卸载注册表项
static std::string dirOf(const std::string& path) {
    size_t p = path.find_last_of("\\/");
    return p == std::string::npos ? std::string(".") : path.substr(0, p);
}

static bool ensureDir(const std::string& path) {
    return CreateDirectoryA(path.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS;
}

static std::string expandEnv(const std::string& in) {
    char buf[4096];
    DWORD n = ExpandEnvironmentStringsA(in.c_str(), buf, sizeof(buf));
    return n > 0 && n < sizeof(buf) ? std::string(buf) : in;
}

static std::string specialFolder(int csidl) {
    char path[MAX_PATH] = {0};
    if (SUCCEEDED(SHGetFolderPathA(nullptr, csidl, nullptr, 0, path))) return std::string(path);
    return std::string();
}

static void createShortcut(const std::string& linkPath, const std::string& target,
                           const std::string& args, const std::string& workDir,
                           const std::string& desc) {
    IShellLinkA* link = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
                                  IID_IShellLinkA, reinterpret_cast<void**>(&link));
    if (FAILED(hr) || !link) return;
    link->SetPath(target.c_str());
    if (!args.empty()) link->SetArguments(args.c_str());
    if (!workDir.empty()) link->SetWorkingDirectory(workDir.c_str());
    if (!desc.empty()) link->SetDescription(desc.c_str());
    IPersistFile* persist = nullptr;
    if (SUCCEEDED(link->QueryInterface(IID_IPersistFile, reinterpret_cast<void**>(&persist)))) {
        WCHAR wide[MAX_PATH] = {0};
        MultiByteToWideChar(CP_ACP, 0, linkPath.c_str(), -1, wide, MAX_PATH);
        persist->Save(wide, TRUE);
        persist->Release();
    }
    link->Release();
}

static void writeRegString(HKEY key, const std::string& subKey, const std::string& name,
                           const std::string& value) {
    HKEY h = nullptr;
    if (RegCreateKeyA(key, subKey.c_str(), &h) == ERROR_SUCCESS) {
        RegSetValueExA(h, name.c_str(), 0, REG_SZ,
                       reinterpret_cast<const BYTE*>(value.c_str()),
                       static_cast<DWORD>(value.size() + 1));
        RegCloseKey(h);
    }
}

static void writeUtf8Bom(const std::string& path, const std::string& content) {
    std::ofstream f(path, std::ios::binary);
    f.put('\xEF'); f.put('\xBB'); f.put('\xBF');
    f.write(content.data(), content.size());
}

int main() {
    CoInitialize(nullptr);

    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    std::string srcDir = dirOf(exePath);

    std::string appDir = expandEnv("%LOCALAPPDATA%\\Programs\\VocabHero");
    ensureDir(appDir);

    // 复制主程序与源码（从 IExpress 解压目录）
    CopyFileA((srcDir + "\\VocabHero.exe").c_str(), (appDir + "\\VocabHero.exe").c_str(), FALSE);
    CopyFileA((srcDir + "\\VocabHero.cpp").c_str(), (appDir + "\\VocabHero.cpp").c_str(), FALSE);

    std::string uninstallBat = "@echo off\n"
        "setlocal\n"
        "set \"APP=%LOCALAPPDATA%\\Programs\\VocabHero\"\n"
        "echo Uninstalling VocabHero...\n"
        "if exist \"%APP%\" rmdir /s /q \"%APP%\"\n"
        "del /q \"%USERPROFILE%\\Desktop\\Vocab Hero.lnk\" >nul 2>&1\n"
        "del /q \"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Vocab Hero.lnk\" >nul 2>&1\n"
        "del /q \"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\Uninstall Vocab Hero.lnk\" >nul 2>&1\n"
        "reg delete \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\VocabHero\" /f >nul 2>&1\n"
        "echo Done. VocabHero has been uninstalled.\n"
        "pause\n";
    writeUtf8Bom(appDir + "\\uninstall.bat", uninstallBat);

    std::string desktop = specialFolder(CSIDL_DESKTOPDIRECTORY);
    std::string startMenu = specialFolder(CSIDL_PROGRAMS);
    std::string exeTarget = appDir + "\\VocabHero.exe";

    createShortcut(desktop + "\\Vocab Hero.lnk", exeTarget, "", appDir, "Vocab Hero - CET Word Game");
    createShortcut(startMenu + "\\Vocab Hero.lnk", exeTarget, "", appDir, "Vocab Hero - CET Word Game");
    createShortcut(startMenu + "\\Uninstall Vocab Hero.lnk", "cmd.exe",
                   "/c \"" + appDir + "\\uninstall.bat\"", appDir, "Uninstall Vocab Hero");

    std::string regKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\VocabHero";
    writeRegString(HKEY_CURRENT_USER, regKey, "DisplayName", "Vocab Hero - CET Word Game");
    writeRegString(HKEY_CURRENT_USER, regKey, "DisplayVersion", "1.0");
    writeRegString(HKEY_CURRENT_USER, regKey, "Publisher", "Vocab Hero");
    writeRegString(HKEY_CURRENT_USER, regKey, "InstallLocation", appDir);
    writeRegString(HKEY_CURRENT_USER, regKey, "UninstallString",
                   appDir + "\\uninstall.bat");

    CoUninitialize();
    std::cout << "INSTALL_OK" << std::endl;
    return 0;
}
