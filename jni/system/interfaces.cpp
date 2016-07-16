#include "../common.h"
#include "interfaces.h"

namespace pegas
{
		const KeyCode IKeyboardController::k_keyCodeESCAPE		= 0x1B;
		const KeyCode IKeyboardController::k_keyCodeBACKSPACE	= 0x08;
		const KeyCode IKeyboardController::k_keyCodeTAB			= 0x09;
		const KeyCode IKeyboardController::k_keyCodeENTER		= 0x0D;
		const KeyCode IKeyboardController::k_keyCodeSPACE		= 0x20;

		const KeyCode IKeyboardController::k_keyCodeSHIFT		= 0x10;
		const KeyCode IKeyboardController::k_keyCodeCTRL		= 0x11;
		const KeyCode IKeyboardController::k_keyCodeALT			= 0x12;

		const KeyCode IKeyboardController::k_keyCodeLWIN		= 0x5B;
		const KeyCode IKeyboardController::k_keyCodeRWIN		= 0x5C;
		const KeyCode IKeyboardController::k_keyCodeAPPS		= 0x5D;

		const KeyCode IKeyboardController::k_keyCodePAUSE		= 0x13;
		const KeyCode IKeyboardController::k_keyCodeCAPSLOCK	= 0x14;
		const KeyCode IKeyboardController::k_keyCodeNUMLOCK		= 0x90;
		const KeyCode IKeyboardController::k_keyCodeSCROLLLOCK	= 0x91;

		const KeyCode IKeyboardController::k_keyCodePGUP		= 0x21;
		const KeyCode IKeyboardController::k_keyCodePGDN		= 0x22;
		const KeyCode IKeyboardController::k_keyCodeHOME		= 0x24;
		const KeyCode IKeyboardController::k_keyCodeEND			= 0x23;
		const KeyCode IKeyboardController::k_keyCodeINSERT		= 0x2D;
		const KeyCode IKeyboardController::k_keyCodeDELETE		= 0x2E;

		const KeyCode IKeyboardController::k_keyCodeLEFT		= 0x25;
		const KeyCode IKeyboardController::k_keyCodeUP			= 0x26;
		const KeyCode IKeyboardController::k_keyCodeRIGHT		= 0x27;
		const KeyCode IKeyboardController::k_keyCodeDOWN		= 0x28;

		const KeyCode IKeyboardController::k_keyCode_0			= 0x30;
		const KeyCode IKeyboardController::k_keyCode_1			= 0x31;
		const KeyCode IKeyboardController::k_keyCode_2			= 0x32;
		const KeyCode IKeyboardController::k_keyCode_3			= 0x33;
		const KeyCode IKeyboardController::k_keyCode_4			= 0x34;
		const KeyCode IKeyboardController::k_keyCode_5			= 0x35;
		const KeyCode IKeyboardController::k_keyCode_6			= 0x36;
		const KeyCode IKeyboardController::k_keyCode_7			= 0x37;
		const KeyCode IKeyboardController::k_keyCode_8			= 0x38;
		const KeyCode IKeyboardController::k_keyCode_9			= 0x39;

		const KeyCode IKeyboardController::k_keyCode_A			= 0x41;
		const KeyCode IKeyboardController::k_keyCode_B			= 0x42;
		const KeyCode IKeyboardController::k_keyCode_C			= 0x43;
		const KeyCode IKeyboardController::k_keyCode_D			= 0x44;
		const KeyCode IKeyboardController::k_keyCode_E			= 0x45;
		const KeyCode IKeyboardController::k_keyCode_F			= 0x46;
		const KeyCode IKeyboardController::k_keyCode_G			= 0x47;
		const KeyCode IKeyboardController::k_keyCode_H			= 0x48;
		const KeyCode IKeyboardController::k_keyCode_I			= 0x49;
		const KeyCode IKeyboardController::k_keyCode_J			= 0x4A;
		const KeyCode IKeyboardController::k_keyCode_K			= 0x4B;
		const KeyCode IKeyboardController::k_keyCode_L			= 0x4C;
		const KeyCode IKeyboardController::k_keyCode_M			= 0x4D;
		const KeyCode IKeyboardController::k_keyCode_N			= 0x4E;
		const KeyCode IKeyboardController::k_keyCode_O			= 0x4F;
		const KeyCode IKeyboardController::k_keyCode_P			= 0x50;
		const KeyCode IKeyboardController::k_keyCode_Q			= 0x51;
		const KeyCode IKeyboardController::k_keyCode_R			= 0x52;
		const KeyCode IKeyboardController::k_keyCode_S			= 0x53;
		const KeyCode IKeyboardController::k_keyCode_T			= 0x54;
		const KeyCode IKeyboardController::k_keyCode_U			= 0x55;
		const KeyCode IKeyboardController::k_keyCode_V			= 0x56;
		const KeyCode IKeyboardController::k_keyCode_W			= 0x57;
		const KeyCode IKeyboardController::k_keyCode_X			= 0x58;
		const KeyCode IKeyboardController::k_keyCode_Y			= 0x59;
		const KeyCode IKeyboardController::k_keyCode_Z			= 0x5A;

		const KeyCode IKeyboardController::k_keyCodeGRAVE		= 0xC0;
		const KeyCode IKeyboardController::k_keyCodeMINUS		= 0xBD;
		const KeyCode IKeyboardController::k_keyCodeEQUALS		= 0xBB;
		const KeyCode IKeyboardController::k_keyCodeBACKSLASH	= 0xDC;
		const KeyCode IKeyboardController::k_keyCodeLBRACKET	= 0xDB;
		const KeyCode IKeyboardController::k_keyCodeRBRACKET	= 0xDD;
		const KeyCode IKeyboardController::k_keyCodeSEMICOLON	= 0xBA;
		const KeyCode IKeyboardController::k_keyCodeAPOSTROPHE	= 0xDE;
		const KeyCode IKeyboardController::k_keyCodeCOMMA		= 0xBC;
		const KeyCode IKeyboardController::k_keyCodePERIOD		= 0xBE;
		const KeyCode IKeyboardController::k_keyCodeSLASH		= 0xBF;

		const KeyCode IKeyboardController::k_keyCodeNUMPAD0		= 0x60;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD1		= 0x61;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD2		= 0x62;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD3		= 0x63;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD4		= 0x64;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD5		= 0x65;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD6		= 0x66;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD7		= 0x67;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD8		= 0x68;
		const KeyCode IKeyboardController::k_keyCodeNUMPAD9		= 0x69;

		const KeyCode IKeyboardController::k_keyCodeMULTIPLY	= 0x6A;
		const KeyCode IKeyboardController::k_keyCodeDIVIDE		= 0x6F;
		const KeyCode IKeyboardController::k_keyCodeADD			= 0x6B;
		const KeyCode IKeyboardController::k_keyCodeSUBTRACT	= 0x6D;
		const KeyCode IKeyboardController::k_keyCodeDECIMAL		= 0x6E;

		const KeyCode IKeyboardController::k_keyCode_F1			= 0x70;
		const KeyCode IKeyboardController::k_keyCode_F2			= 0x71;
		const KeyCode IKeyboardController::k_keyCode_F3			= 0x72;
		const KeyCode IKeyboardController::k_keyCode_F4			= 0x73;
		const KeyCode IKeyboardController::k_keyCode_F5			= 0x74;
		const KeyCode IKeyboardController::k_keyCode_F6			= 0x75;
		const KeyCode IKeyboardController::k_keyCode_F7			= 0x76;
		const KeyCode IKeyboardController::k_keyCode_F8			= 0x77;
		const KeyCode IKeyboardController::k_keyCode_F9			= 0x78;
		const KeyCode IKeyboardController::k_keyCode_F10		= 0x79;
		const KeyCode IKeyboardController::k_keyCode_F11		= 0x7A;
		const KeyCode IKeyboardController::k_keyCode_F12		= 0x7B;

}
