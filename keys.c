#include "oslib.h"

int (*readButtonsFunctions)(SceCtrlData *pad_data, int count);

OSL_CONTROLLER osl_pad;

OSL_CONTROLLER *oslReadKeys()
{
	SceCtrlData ctl;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(1);

    if (readButtonsFunctions != NULL)
        readButtonsFunctions(&ctl, 1);
    else
    	sceCtrlPeekBufferPositive( &ctl, 1 );

	//R�cup�re les valeurs du stick
    if (osl_keys->holdAffectsAnalog && (ctl.Buttons & OSL_KEYMASK_HOLD)){
        osl_keys->analogX = 0;
        osl_keys->analogY = 0;
    }else{
        osl_keys->analogX = (signed)ctl.Lx-128;
        osl_keys->analogY = (signed)ctl.Ly-128;
    }

	if (osl_keys->analogToDPadSensivity > 0)		{
		if (osl_keys->analogY >= osl_keys->analogToDPadSensivity)
			ctl.Buttons |= OSL_KEYMASK_DOWN;
		if (osl_keys->analogY <= -osl_keys->analogToDPadSensivity)
			ctl.Buttons |= OSL_KEYMASK_UP;
		if (osl_keys->analogX >= osl_keys->analogToDPadSensivity)
			ctl.Buttons |= OSL_KEYMASK_RIGHT;
		if (osl_keys->analogX <= -osl_keys->analogToDPadSensivity)
			ctl.Buttons |= OSL_KEYMASK_LEFT;
	}

	/*
		CODE AUTO-REPEAT
	*/
	if (osl_keys->autoRepeatInterval > 0)			{					//Auto repeat activ�?
		//Si �a change -> compteur � z�ro
		if (osl_keys->lastHeld.value != ctl.Buttons)
			osl_keys->autoRepeatCounter=0;
		else			{
			osl_keys->autoRepeatCounter++;
			if (osl_keys->autoRepeatCounter >= osl_keys->autoRepeatInit)			{
				//AutoRepeat d�clench� -> d�clenchement toutes les autoRepeatInterval coups
				if ((osl_keys->autoRepeatCounter - osl_keys->autoRepeatInit) % osl_keys->autoRepeatInterval == 0)
					osl_keys->lastHeld.value &= ~osl_keys->autoRepeatMask;
			}
		}
	}

	osl_keys->pressed.value = ~osl_keys->lastHeld.value & ctl.Buttons;
	osl_keys->released.value = osl_keys->lastHeld.value & ~ctl.Buttons;

	osl_keys->held.value = ctl.Buttons;
	osl_keys->lastHeld.value = ctl.Buttons;

	return osl_keys;
}

void oslFlushKey()
{
	oslReadKeys();
}

int oslKbhit()
{
	OSL_CONTROLLER *keys;
	int i, j, k;
	keys = oslReadKeys();
	k = 0;
	j = 0;
	for (i=0;i<24;i++)		{
		if (keys->pressed.value & (1<<i))			{
			k |= (i+1)<<j;
			j+=8;
		}
	}
	return k;
}

int oslWaitKey()
{
	int i;
	while(!(i=oslKbhit()) && !osl_quit)
		oslWaitVSync();
	return i;
}


int oslSetReadKeysFunction(int (*sceCtrlReadBufferPositive)(SceCtrlData *pad_data, int count))
{
    readButtonsFunctions = sceCtrlReadBufferPositive;
    return 0;
}


int oslUnsetReadKeysFunction(){
    readButtonsFunctions = NULL;
    return 0;
}


int oslSetHoldForAnalog(int holdForAnalog){
    osl_keys->holdAffectsAnalog = holdForAnalog;
    return 0;
}
