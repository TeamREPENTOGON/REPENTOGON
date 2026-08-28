#include "IsaacRepentance.h"

MOD_EXPORT KColor* L_KColor_Constructor(KColor* color, float r, float g, float b, float a)
{
    color->_red = r;
    color->_green = g;
    color->_blue = b;
    color->_alpha = a;
    color->_unk = 0;

    return color;
};