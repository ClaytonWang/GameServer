#include "Stdafx.h"
#include "AfxDllx.h"
#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
static AFX_EXTENSION_MODULE SkinControlDLL={NULL,NULL};


//��������
static INT(WINAPI * GetScrollPosT)(HWND, INT) = GetScrollPos;
static INT(WINAPI * SetScrollPosT)(HWND, INT, INT, BOOL) = SetScrollPos;
static BOOL(WINAPI * GetScrollInfoT)(HWND, INT, LPSCROLLINFO) = GetScrollInfo;
static INT(WINAPI *SetScrollInfoT)(HWND, INT, LPCSCROLLINFO, BOOL) = SetScrollInfo;
static BOOL(WINAPI *GetScrollRangeT)(HWND, INT, LPINT, LPINT) = GetScrollRange;
static BOOL(WINAPI *SetScrollRangeT)(HWND, INT, INT, INT, BOOL) = SetScrollRange;
static BOOL(WINAPI *ShowScrollBarT)(HWND, INT, BOOL) = ShowScrollBar;
static BOOL(WINAPI *EnableScrollBarT)(HWND, UINT, UINT) = EnableScrollBar;

//////////////////////////////////////////////////////////////////////////////////
//��������
/*
DETOUR_TRAMPOLINE(INT   WINAPI GetScrollPosT(HWND, INT), GetScrollPos)
DETOUR_TRAMPOLINE(INT   WINAPI SetScrollPosT(HWND, INT, INT, BOOL), SetScrollPos)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollInfoT(HWND, INT, LPSCROLLINFO), GetScrollInfo)
DETOUR_TRAMPOLINE(INT   WINAPI SetScrollInfoT(HWND, INT, LPCSCROLLINFO, BOOL), SetScrollInfo)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollRangeT(HWND, INT, LPINT, LPINT), GetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI SetScrollRangeT(HWND, INT, INT, INT, BOOL), SetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI ShowScrollBarT(HWND, INT, BOOL), ShowScrollBar)
DETOUR_TRAMPOLINE(BOOL  WINAPI EnableScrollBarT(HWND, UINT, UINT), EnableScrollBar)
*/
//////////////////////////////////////////////////////////////////////////////////

//���ù���
INT WINAPI SetScrollInfoD(HWND hWnd, INT fnBar, LPCSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_SetScrollInfo(hWnd,fnBar,lpScrollInfo,bRedraw);
	}
	else
	{
		return SetScrollInfoT(hWnd,fnBar,lpScrollInfo,bRedraw);
	}
}

//��ȡ����
BOOL WINAPI GetScrollInfoD(HWND hWnd, INT fnBar, LPSCROLLINFO lpScrollInfo)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_GetScrollInfo(hWnd, fnBar, lpScrollInfo);
	}
	else
	{
		return GetScrollInfoT(hWnd, fnBar, lpScrollInfo);
	}
}

//���ù���
INT WINAPI SetScrollPosD(HWND hWnd, INT nBar, INT nPos, BOOL bRedraw)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_SetScrollPos(hWnd,nBar, nPos,bRedraw);
	}
	else
	{
		return SetScrollPosT(hWnd,nBar, nPos,bRedraw);
	}
}

//��ȡ����
INT WINAPI GetScrollPosD(HWND hWnd, INT nBar)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_GetScrollPos(hWnd,nBar);
	}
	else
	{
		return GetScrollPosT(hWnd,nBar);
	}
}

//���ù���
BOOL WINAPI SetScrollRangeD(HWND hWnd, INT nBar, INT nMinPos, INT nMaxPos, BOOL bRedraw)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_SetScrollRange(hWnd,nBar,nMinPos,nMaxPos,bRedraw);
	}
	else
	{
		return SetScrollRangeT(hWnd,nBar,nMinPos,nMaxPos,bRedraw);
	}
}

//��ȡ����
BOOL WINAPI GetScrollRangeD(HWND hWnd, INT nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_GetScrollRange(hWnd,nBar,lpMinPos,lpMaxPos);
	}
	else
	{
		return GetScrollRangeT(hWnd,nBar,lpMinPos,lpMaxPos);
	}
}

//��ʾ����
BOOL WINAPI ShowScrollBarD(HWND hWnd, INT nBar, BOOL bShow)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_ShowScrollBar(hWnd,nBar,bShow);
	}
	else
	{
		return ShowScrollBarT(hWnd,nBar,bShow);
	}
}

//���ù���
BOOL WINAPI EnableScrollBarD(HWND hWnd, UINT wSBflags, UINT wArrows)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_EnableScrollBar(hWnd,wSBflags,wArrows);
	}
	else
	{
		return EnableScrollBarT(hWnd,wSBflags,wArrows);
	}
}

//////////////////////////////////////////////////////////////////////////////////

//��������
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (DetourIsHelperProcess()) {
		return TRUE;
	}
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(SkinControlDLL, hInstance)) return 0;
		new CDynLinkLibrary(SkinControlDLL);

		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)SetScrollPosT, SetScrollPosD);
		DetourAttach(&(PVOID&)GetScrollPosT, GetScrollPosD);
		DetourAttach(&(PVOID&)SetScrollInfoT, SetScrollInfoD);
		DetourAttach(&(PVOID&)GetScrollInfoT, GetScrollInfoD);
		DetourAttach(&(PVOID&)ShowScrollBarT, ShowScrollBarD);
		DetourAttach(&(PVOID&)SetScrollRangeT, SetScrollRangeD);
		DetourAttach(&(PVOID&)GetScrollRangeT, GetScrollRangeD);
		DetourAttach(&(PVOID&)EnableScrollBarT, EnableScrollBarD);
		DetourTransactionCommit();
		//���ع���
		/*
		DetourFunctionWithTrampoline((PBYTE)SetScrollPosT,(PBYTE)SetScrollPosD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollPosT,(PBYTE)GetScrollPosD);
		DetourFunctionWithTrampoline((PBYTE)SetScrollInfoT,(PBYTE)SetScrollInfoD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollInfoT,(PBYTE)GetScrollInfoD);
		DetourFunctionWithTrampoline((PBYTE)ShowScrollBarT,(PBYTE)ShowScrollBarD);
		DetourFunctionWithTrampoline((PBYTE)SetScrollRangeT,(PBYTE)SetScrollRangeD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollRangeT,(PBYTE)GetScrollRangeD);
		DetourFunctionWithTrampoline((PBYTE)EnableScrollBarT,(PBYTE)EnableScrollBarD);
		*/
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(SkinControlDLL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)SetScrollPosT, SetScrollPosD);
		DetourDetach(&(PVOID&)GetScrollPosT, GetScrollPosD);
		DetourDetach(&(PVOID&)SetScrollInfoT, SetScrollInfoD);
		DetourDetach(&(PVOID&)GetScrollInfoT, GetScrollInfoD);
		DetourDetach(&(PVOID&)ShowScrollBarT, ShowScrollBarD);
		DetourDetach(&(PVOID&)SetScrollRangeT, SetScrollRangeD);
		DetourDetach(&(PVOID&)GetScrollRangeT, GetScrollRangeD);
		DetourDetach(&(PVOID&)EnableScrollBarT, EnableScrollBarD);
		DetourTransactionCommit();
		//ж�ع���
		/*
		DetourRemove((PBYTE)SetScrollPosT,(PBYTE)SetScrollPosD);
		DetourRemove((PBYTE)GetScrollPosT,(PBYTE)GetScrollPosD);
		DetourRemove((PBYTE)SetScrollInfoT,(PBYTE)SetScrollInfoD);
		DetourRemove((PBYTE)GetScrollInfoT,(PBYTE)GetScrollInfoD);
		DetourRemove((PBYTE)ShowScrollBarT,(PBYTE)ShowScrollBarD);
		DetourRemove((PBYTE)SetScrollRangeT,(PBYTE)SetScrollRangeD);
		DetourRemove((PBYTE)GetScrollRangeT,(PBYTE)GetScrollRangeD);
		DetourRemove((PBYTE)EnableScrollBarT,(PBYTE)EnableScrollBarD);
		*/
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
