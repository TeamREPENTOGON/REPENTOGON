#include "IsaacRepentance.h"

MOD_EXPORT void L_GenericPrompt_Init(GenericPrompt* prompt) {
	new (prompt) GenericPrompt();
}

MOD_EXPORT void L_GenericPrompt_Destroy(GenericPrompt* prompt) {
	prompt->~GenericPrompt();
	memset(prompt, 0, sizeof(GenericPrompt));
}

MOD_EXPORT void L_GenericPrompt_Initialize(GenericPrompt* prompt, bool smallPrompt) {
	prompt->Initialize(smallPrompt);
}

MOD_EXPORT void L_GenericPrompt_Show(GenericPrompt* prompt) {
	prompt->Show(false);
}

MOD_EXPORT bool L_GenericPrompt_IsActive(GenericPrompt* prompt) {
	return prompt->IsActive();
}

MOD_EXPORT void L_GenericPrompt_SetImageToVictoryRun(GenericPrompt* prompt) {
	prompt->SetImageToVictoryRun(nullptr);
}

MOD_EXPORT void L_GenericPrompt_Update(GenericPrompt* prompt, bool processInput) {
	prompt->Update(processInput);
}

MOD_EXPORT void L_GenericPrompt_Render(GenericPrompt* prompt) {
	prompt->Render();
}

MOD_EXPORT void L_GenericPrompt_SetText(GenericPrompt* prompt, const char* text1, const char* text2, const char* text3, const char* text4, const char* text5) {
	prompt->SetText(text1, text2, text3, text4, text5);
}
