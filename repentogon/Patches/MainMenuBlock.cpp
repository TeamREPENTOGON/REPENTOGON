#include "MainMenuBlock.h"
namespace MainMenuInputBlock{
    unsigned int _inputmask=-1;
    bool _enabled=0;
    void SetInputMask(unsigned int mask) { _enabled=true; _inputmask = mask; };
	unsigned int GetInputMask() { return _inputmask; };
	void ClearInputMask() { _enabled=false; _inputmask = -1; };
};