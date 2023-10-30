#include "console.h"


void game_console::Render() {
	if (szBuffer[0]) {
		float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float outer_color[4] = { 0.12f, 0.12f, 0.10f, 1.0f };
		float inner_color[4] = { 0.25f, 0.25f, 0.20f, 1.0f };

		uintptr_t shader = Material_RegisterHandle(0x4ED973885856E206, 0, -1); //*reinterpret_cast<uintptr_t*>(0x15024F278_g); // 

		R_AddCmdDrawStretchPic(32, 40, 1852, 2, 0, 0, 1, 1, outer_color, shader); //top bar
		R_AddCmdDrawStretchPic(32, 67, 1852, 2, 0, 0, 1, 1, outer_color, shader); //bottom bar
		
		R_AddCmdDrawStretchPic(30, 40, 2, 29, 0, 0, 1, 1, outer_color, shader); //left bar
		R_AddCmdDrawStretchPic(1882, 42, 2, 27, 0, 0, 1, 1, outer_color, shader); //right bar
		
		R_AddCmdDrawStretchPic(32, 42, 1850, 25, 0, 0, 1, 1, inner_color, shader);
		float scale = 1.0f;

		uintptr_t font = UI_GetFontHandle(ScrPlace_GetViewUIContext(0), 7, scale);
		float width = R_TextWidth(0, szBuffer, 0x7FFFFFFF, font);

		CL_DrawTextPhysical(szBuffer, 0x7FFFFFFF, font, 35, 67, scale, scale, 0.0f, white, 0, 0);
		if (edit_tick > 20 && edit_tick < 40) {
			CL_DrawTextPhysical("|", 0x7FFFFFFF, font, 35 + width, 67, scale, scale, 0.0f, white, 0, 0);
		}
		edit_tick++;

		if (edit_tick > 41) {
			edit_tick = 0;
		}
		//for (int i = 0; i < 8;i++) {
		//	float scale = 1.0f;
		//	CL_DrawTextPhysical(szBuffer, 0x7FFFFFFF, UI_GetFontHandle(ScrPlace_GetViewUIContext(0), 7, scale), 35, 65 + (i * 50), scale, scale, 0.0f, white, 0, 0);
		//}
	}
}

void game_console::KeyboardHandle(WPARAM wParam, int key) {
	//printf("%i\n", key);
	if (key == VK_RETURN) {


		auto Cbuf_AddText = reinterpret_cast<void(*)(int, const char*)>(0x1492E2B50_g);

		Cbuf_AddText(0, &g_console.szBuffer[g_console.start]);

		g_console.bopen = false;
		memset(szBuffer, 0, 4096);
		return;
	}

	if (key == VK_BACK) {
		if (szBuffer[start]) {
			szBuffer[strlen(szBuffer) - 1] = 0;
		}
		return;
	}

	if (key >= 0x30 && key <= 0x5A || key == VK_SPACE || key == VK_OEM_MINUS || key == VK_OEM_PLUS || key == 0xBF) {
		if (strlen(szBuffer) < 4096) {
			int vkey = key;
			if (!GetAsyncKeyState(VK_RSHIFT) && key != VK_SPACE) {
				vkey = tolower(key);
				switch (key) {
					case VK_OEM_PLUS: vkey = '='; break;
					case VK_OEM_MINUS: vkey = '-'; break;
					case 0xBF: vkey = '/'; break;
				}
			}
			if (GetAsyncKeyState(VK_RSHIFT)) {
				switch (key) {
					case VK_OEM_PLUS: vkey = '+'; break;
					case VK_OEM_MINUS: vkey = '_'; break;
					case 0x30: vkey = ')'; break;
					case 0x31: vkey = '!'; break;
					case 0x32: vkey = '@'; break;
					case 0x33: vkey = '#'; break;
					case 0x34: vkey = '$'; break;
					case 0x35: vkey = '%'; break;
					case 0x36: vkey = '^'; break;
					case 0x37: vkey = '&'; break;
					case 0x38: vkey = '*'; break;
					case 0x39: vkey = '('; break;
					case 0xBF: vkey = '?'; break;
				}
			}
			strcat(szBuffer, (const char*)&vkey);
		}
	}
	

}

game_console g_console;