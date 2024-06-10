#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h> //<> - поиск по системным каталогам VStudio
#include <CommCtrl.h>
#include <cstdio>
#include "resource.h" //"" - поиск по каталогу проекта, а после по системным каталогам VStudio 
#include <cmath>

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

CHAR* ParseAddress(CHAR sz_address[], CONST CHAR sz_description[], DWORD dw_address)
{
	sprintf(
		sz_address,
		"%s%i.%i.%i.%i", sz_description,
		FIRST_IPADDRESS(dw_address),
		SECOND_IPADDRESS(dw_address),
		THIRD_IPADDRESS(dw_address),
		FOURTH_IPADDRESS(dw_address)
	);
	return sz_address;
}

CHAR* ParseAddress(CHAR sz_address[], DWORD dw_address)
{
	sprintf(
		sz_address,
		"%i.%i.%i.%i",
		FIRST_IPADDRESS(dw_address),
		SECOND_IPADDRESS(dw_address),
		THIRD_IPADDRESS(dw_address),
		FOURTH_IPADDRESS(dw_address)
	);
	return sz_address;
}

/*BOOL CheckMask(DWORD mask)
{
	DWORD pattern = 1 << 32;
	for (int i = 0; i < 32; i++)
	{
		
	}
	
	return TRUE;
}

INT CountOnes(DWORD mask)
{
	INT zero_bits = 0;
	DWORD power;
	for (int i = 1; i; i <<= 1, zero_bits++)
	{
		if (mask & i)
		{
			power = i;
			break;
		}
	}
	return 32 - zero_bits;
}*/

//Процедура окна - это самая обычная функция, которая вызывается при запуске окна
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Все принимаемые параметры - это числовые значения длиной 4 Байта
	//HWND (Handler to Window) - обработчик окна - это число, которое идентифицирует окно
	//Любые обращения к окну можно выполнить только через его обработчик
	//uMsg - Message - Сообщение, которое отправляется окну
	//wParam, lParam - параметры сообщения. Они делятся на LOWORD и HIWORD
	//LOWORD - младшее слово
	//HIWORD - старшее слово
	//WORD (слово) - это два Байта
	
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//WM - Window Message
		HWND hUpDown = GetDlgItem(hwnd, IDC_SPIN_PREFIX);
		//Функция GetDlgItem() получает HWND элемента интерфейса по его ID
		SendMessage(hUpDown, UDM_SETRANGE32, 0, 32);

		HWND hIpAddress = GetDlgItem(hwnd, IDC_IPADDRESS);
		SetFocus(hIpAddress);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{ //wParam contains control ID (IDOK, IDCANCEL...)
		case IDC_IPADDRESS:
		{
			CONST INT SIZE = 256;
			CHAR sz_info[SIZE]{};
			HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			//EN - Edit Notification (уведомление)
			if (HIWORD(wParam) == EN_CHANGE)
			{
				//BYTE = 8 bit - CHAR
				//WORD = 2 Byte (16 bit) - SHORT
				//DWORD (Double Word) = 4 Byte (32 bit) - INT
				//QWORD (Quad Word) = 8 Byte (64 bit) - LONG LONG
				//TBYTE (Ten Bytes) = 10 Bytes (80 bit);
				//DQWORD (Double Quad Word) = 16 Bytes (128 bit);
				DWORD dw_address;
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				INT first = FIRST_IPADDRESS(dw_address);
				INT second = SECOND_IPADDRESS(dw_address);
				INT third = THIRD_IPADDRESS(dw_address);
				INT fourth = FOURTH_IPADDRESS(dw_address);

				sprintf_s(sz_info, SIZE, "Info:\nFirst: %i, Second %i, Third %i, Fourth %i", first, second, third, fourth);
				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);
				if (first < 128)
					SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"8");
				else if (first < 192)
					SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"16");
				else if (first < 224)
					SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)"24");
			}		
		}
		break;
		//case IDC_IPMASK:
		//{
		//	if (HIWORD(wParam) == EN_CHANGE)
		//	{
		//		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		//		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
		//		DWORD dw_mask = 0;
		//		SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
		//		INT prefix = CountOnes(dw_mask);
		//		CHAR sz_prefix[3] = {}; //sz_ - string zero (строка заканчивающаяся нулём)
		//		sprintf(sz_prefix, "%i", prefix);
		//		SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
		//		//SendMessage(hIPmask, IPM_SETADDRESS, 0, dw_mask >> (32 - prefix) << (32 - prefix));
		//	}
		//}
		break;
		case IDC_EDIT_PREFIX:
		{
			HWND hIPAddress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			HWND hIpMask = GetDlgItem(hwnd, IDC_IPMASK);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			DWORD dw_mask = ~0;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				CONST INT SIZE_PREFIX = 8;
				CHAR sz_buffer[SIZE_PREFIX];
				SendMessage(hEditPrefix, WM_GETTEXT, SIZE_PREFIX, (LPARAM)sz_buffer);
				//MessageBox(hwnd, sz_buffer, "Prefix", MB_OK | MB_ICONINFORMATION);
				INT shift = atoi(sz_buffer);
				//dw_mask >>= (32 - shift); //Функция atoi() преобразует ASCII-строку в значение типа 'int'
				//dw_mask <<= (32 - shift);
				//SendMessage(hIpMask, IPM_SETADDRESS, 0, (LPARAM)dw_mask);
				SendMessage(hIpMask, IPM_SETADDRESS, 0, dw_mask /*>> (32 - shift)*/ <<= (32 - shift));





				CONST INT SIZE = 256;
				CHAR sz_info[SIZE]{};
				CHAR sz_network_address[SIZE]{};
				CHAR sz_broadcast_address[SIZE]{};
				CHAR sz_number_of_hosts[SIZE]{};
				CHAR sz_first_host[SIZE]{};
				CHAR sz_last_host[SIZE]{};
				DWORD dw_address = 0;
				SendMessage(hIPAddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
				DWORD dw_network_address = dw_address & dw_mask;
				ParseAddress(sz_network_address, "Адрес сети:\t\t\t", dw_network_address);

				DWORD dw_broadcast_address = ~dw_mask | dw_network_address;
				ParseAddress(sz_broadcast_address, "Широковещательный адрес:\t", dw_broadcast_address);

				DWORD dw_number_of_hosts = dw_broadcast_address - dw_network_address - 1;
				sprintf(sz_number_of_hosts, "Количество узлов:\t\t%i", dw_number_of_hosts);

				ParseAddress(sz_first_host, "Начальный IP-адрес:\t\t", dw_network_address + 1);
				ParseAddress(sz_last_host, "Последний IP-адрес:\t\t", dw_broadcast_address - 1);
					

				sprintf(
					sz_info,
					"Info:\n%s\n%s\n%s\n%s\n%s",
					sz_network_address,
					sz_broadcast_address,
					sz_number_of_hosts,
					sz_first_host,
					sz_last_host
				);
				SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)sz_info);

			}
		}
		break;
		case IDOK: 
		{
			CHAR result[256];
			CONST INT SIZE = 256;
			CHAR nwads_info[SIZE]{};
			CHAR bc_info[SIZE]{};
			CHAR adr_count[SIZE]{};
			CHAR first_host[SIZE]{};
			CHAR last_host[SIZE]{};

			HWND hIPaddress = GetDlgItem(hwnd, IDC_IPADDRESS);
			HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);
			HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
			HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
			DWORD dw_address;
			SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dw_address);
			INT IP_first = FIRST_IPADDRESS(dw_address);
			INT IP_second = SECOND_IPADDRESS(dw_address);
			INT IP_third = THIRD_IPADDRESS(dw_address);
			INT IP_fourth = FOURTH_IPADDRESS(dw_address);

			DWORD dw_mask;
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
			INT MASK_first = FIRST_IPADDRESS(dw_mask);
			INT MASK_second = SECOND_IPADDRESS(dw_mask);
			INT MASK_third = THIRD_IPADDRESS(dw_mask);
			INT MASK_fourth = FOURTH_IPADDRESS(dw_mask);

			
			DWORD dw_mask_addresses = dw_mask;
			int i = 32;
			for (; dw_mask_addresses & 1 ^ 1; i--)
				dw_mask_addresses >>= 1;

			sprintf_s(nwads_info, SIZE, "Info:\nАдрес сети:\t\t\t%i.%i.%i.%i", IP_first & MASK_first, IP_second & MASK_second, IP_third & MASK_third, IP_fourth & MASK_fourth);
			sprintf_s(bc_info, SIZE, "\nШироковещательный адрес:\t%i.%i.%i.%i", (IP_first& MASK_first) | (255 - MASK_first), (IP_second& MASK_second) | (255 - MASK_second), (IP_third& MASK_third) | (255 - MASK_third), (IP_fourth& MASK_fourth) | (255 - MASK_fourth));
			sprintf_s(adr_count, SIZE, "\nКоличество IP-адресов:\t%i", (int)pow(2, 32-i) - 2);
			sprintf_s(first_host, SIZE, "\nIP-адрес первого хоста:\t%i.%i.%i.%i", IP_first& MASK_first, IP_second& MASK_second, IP_third& MASK_third, (IP_fourth& MASK_fourth) | 1);
			sprintf_s(last_host, SIZE, "\nIP-адрес последнего хоста:\t%i.%i.%i.%i", (IP_first& MASK_first) | (255 - MASK_first), (IP_second& MASK_second) | (255 - MASK_second), (IP_third& MASK_third) | (255 - MASK_third), ((IP_fourth& MASK_fourth) | (255 - MASK_fourth)) ^ 1);

			strcpy(result, nwads_info);
			strcat(result, bc_info);
			strcat(result, adr_count);
			strcat(result, first_host);
			strcat(result, last_host);

			SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)result);


		}
			 
			break;
		
		case IDCANCEL: EndDialog(hwnd, 0); break;
		}
		break;
	case WM_NOTIFY:
	{
		HWND hIPmask = GetDlgItem(hwnd, IDC_IPMASK);
		HWND hEditPrefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);

		switch (wParam)
		{
			case IDC_IPMASK:
			{
				DWORD dw_mask = 0;
				SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dw_mask);
				int i = 32;
				for (; dw_mask & 1 ^ 1; i--)
					dw_mask >>= 1;
				CHAR sz_prefix[3]{};
				sprintf(sz_prefix, "%i", i);
				SendMessage(hEditPrefix, WM_SETTEXT, 0, (LPARAM)sz_prefix);
			}
			break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}