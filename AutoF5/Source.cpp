/*
* Read the description)
* All another is simple
	by operok 022020
*/

#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>

class dShort
{
private:
	short size;
	HWND* sarr;
public:
	dShort(const short s) {
		size = s;
		sarr = new HWND[size];
	}
	dShort& operator += (HWND* nN) {
		HWND* nsarr = new HWND[size + 1];
		for (short i = 0; i < size; i++) {
			nsarr[i] = sarr[i];
		}
		nsarr[size] = *nN;
		delete[] sarr;
		sarr = nsarr;
		size++;
		return *this;
	}
	HWND& operator [] (short index) {
		return sarr[index];
	}
	short getSize() {
		return size;
	}
	~dShort() {
		delete[] sarr;
	}
};

dShort st(0);

bool GetFWTime(WCHAR *path, FILETIME *ret)
{
	HANDLE hFile;
	FILETIME ftCreate, ftAccess;
	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE || !GetFileTime(hFile, &ftCreate, &ftAccess, ret))
	{
		std::cout << "Îøèáêà! Íåò äîñòóïà ê ôàéëó!\n";
		return 0;
	}
	GetFileTime(hFile, &ftCreate, &ftAccess, ret);
	CloseHandle(hFile);
	return 1;
}

BOOL CALLBACK speichereFenster(HWND hwnd, LPARAM lParam) {
	const DWORD TITLE_SIZE = 1024;
	WCHAR windowTitle[TITLE_SIZE];

	GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

	int length = ::GetWindowTextLength(hwnd);
	std::wstring title(&windowTitle[0]);
	if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
		return 1;
	}

	st += &hwnd;

	std::vector<std::wstring>& titles =
	*reinterpret_cast<std::vector<std::wstring>*>(lParam);
	titles.push_back(title);
	return 1;
}

bool EnterNum(HWND *ret) {
	std::vector<std::wstring> titles;
	EnumWindows(speichereFenster, reinterpret_cast<LPARAM>(&titles));
	std::cout << "Îòêðûòû îêíà: \n";

	unsigned short i = 0;
	unsigned short lasti;

	for (const auto& title : titles) {
		std::wcout << i << L": " << title << std::endl;
		i++;
	}

	lasti = --i;
	std::cout << "Ââåäèòå íîìåð íóæíîãî îêíà\n";
	std::cin >> i;

	if (i > lasti || i < 0) 
	{
		i = 0;
	}
	std::cout << "Èñïîëüçóåòñÿ íîìåð " << i << '\n';
	*ret = st[i];
	return 1;
}

bool EnterPath(WCHAR *ret){
		std::cout << "Âåäèòå àäðåñ ôàéëà\n";
		std::wstring cin;
		std::wcin >> cin;

		if (cin.length() < MAX_PATH && cin.length() > 0)
		{
			for (int i = 0; i < (int)cin.length() + 1; i++)
			ret[i] = cin[i];
			return 1;
		}

		std::cout << "Ââåä¸í íåïðàâèëüíûé ïóòü\n";
		return 0;

}

int main() 
{
	setlocale(LC_ALL, "ru");
	std::cout << "Made by operok\n";
	HWND hCurrentWindow;
	EnterNum(&hCurrentWindow);

	WCHAR path[MAX_PATH];
	FILETIME LastFT;

	do 
	{
		while (!EnterPath(path));
	}
	while (!GetFWTime(path, &LastFT));
	
	std::cout << "\t Íà÷àëîñü îòñëåæèâàíèå èçìåíåíèé\n";

		/*
			SYSTEMTIME stUTC, stLocal;
			FileTimeToSystemTime(&NewFT, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			std::cout << '\t' << stLocal.wDay << ' ' << stLocal.wMonth << ' ' << stLocal.wYear << ' ' << stLocal.wHour << ':' << stLocal.wMinute << '.' << stLocal.wMilliseconds << '\n';
		*/

	while (1) {

				FILETIME NewFT;
				if (!GetFWTime(path, &NewFT)) 
					std::cout << "Îøèáêà! Ïðîïàë äîñòóï ê ôàéëó!\n";

				if (NewFT.dwHighDateTime != LastFT.dwHighDateTime || NewFT.dwLowDateTime != LastFT.dwLowDateTime)
				{
					HWND oldhwnd = GetForegroundWindow();
					SetForegroundWindow(hCurrentWindow);

					SendMessage(hCurrentWindow, WM_KEYDOWN, VK_CONTROL, 0x0167027F);
					SendMessage(hCurrentWindow, WM_KEYDOWN, VK_F5, 0x0167027F);
					Sleep(rand() % 75 + 50);
					SendMessage(hCurrentWindow, WM_KEYUP, VK_F5, 0x0167027F);
					SendMessage(hCurrentWindow, WM_KEYUP, VK_CONTROL, 0x0167027F);

					SetForegroundWindow(oldhwnd);
					std::cout << "Çàìå÷åíî èçìåíåíèå ôàéëà.\n";
					LastFT = NewFT;
				}
				Sleep(350);
			}
	system("pause");
	return 0;
}
