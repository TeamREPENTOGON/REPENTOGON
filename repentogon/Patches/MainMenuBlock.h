#pragma once
namespace MainMenuInputBlock{
    extern unsigned int _inputmask;	//pass all inputs
    extern bool _enabled;
    void SetInputMask(unsigned int mask);
	unsigned int GetInputMask();
	void ClearInputMask();
};