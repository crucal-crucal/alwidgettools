#pragma once

#include <QtGlobal>

namespace AL {
#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>

static inline void setShadow(HWND hwnd) {
	constexpr MARGINS shadow = { 1, 0, 0, 0 };
	typedef HRESULT (WINAPI *DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS* pMarInset);
	if (const HMODULE module = LoadLibraryW(L"dwmapi.dll")) {
		if (const auto dwm_extendframe_into_client_area_ = reinterpret_cast<DwmExtendFrameIntoClientAreaPtr>(GetProcAddress(module, "DwmExtendFrameIntoClientArea"))) {
			dwm_extendframe_into_client_area_(hwnd, &shadow);
		}
	}
}
#endif
} // namespace AL
