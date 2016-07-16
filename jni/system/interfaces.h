#ifndef PEGAS_SYSTEM_INTERFACES_H_
#define PEGAS_SYSTEM_INTERFACES_H_

#include "../core/types.h"
#include "../core/smart_pointer.h"

#ifdef ANDROID
#include <android/input.h>
#endif

namespace pegas
{

		enum eKeyFlags
		{
				k_keyFlagShift = 1,
				k_keyFlagCtrl = 2,
				k_keyFlagAlt = 4,
				k_keyFlagCapsLock = 8,
				k_keyFlagNumLock = 16,
				k_keyFlagScrollLock = 32,
				k_keyFlagRepeat = 64
		};

		enum eMouseButton
		{
				k_mouseButtonLeft = 1,
				k_mouseButtonRight = 2,
				k_mouseButtonMiddle = 4
		};

		typedef int32 KeyCode;
		typedef int32 KeyFlags;
		typedef int32 MouseButton;
		typedef int32 MouseFlags;
		typedef int32 NumNothes;

		class IKeyboardController
		{
		public:
			static const KeyCode k_keyCodeESCAPE;//		0x1B
			static const KeyCode k_keyCodeBACKSPACE;//	0x08
			static const KeyCode k_keyCodeTAB;//		0x09
			static const KeyCode k_keyCodeENTER;//		0x0D
			static const KeyCode k_keyCodeSPACE;//		0x20

			static const KeyCode k_keyCodeSHIFT;//		0x10
			static const KeyCode k_keyCodeCTRL;//		0x11
			static const KeyCode k_keyCodeALT;//		0x12

			static const KeyCode k_keyCodeLWIN;//		0x5B
			static const KeyCode k_keyCodeRWIN;//		0x5C
			static const KeyCode k_keyCodeAPPS;//		0x5D

			static const KeyCode k_keyCodePAUSE;//		0x13
			static const KeyCode k_keyCodeCAPSLOCK;//	0x14
			static const KeyCode k_keyCodeNUMLOCK;//	0x90
			static const KeyCode k_keyCodeSCROLLLOCK;//	0x91

			static const KeyCode k_keyCodePGUP;//		0x21
			static const KeyCode k_keyCodePGDN;//		0x22
			static const KeyCode k_keyCodeHOME;//		0x24
			static const KeyCode k_keyCodeEND;//		0x23
			static const KeyCode k_keyCodeINSERT;//		0x2D
			static const KeyCode k_keyCodeDELETE;//		0x2E

			static const KeyCode k_keyCodeLEFT;//		0x25
			static const KeyCode k_keyCodeUP;//			0x26
			static const KeyCode k_keyCodeRIGHT;//		0x27
			static const KeyCode k_keyCodeDOWN;//		0x28

			static const KeyCode k_keyCode_0;//			0x30
			static const KeyCode k_keyCode_1;//			0x31
			static const KeyCode k_keyCode_2;//			0x32
			static const KeyCode k_keyCode_3;//			0x33
			static const KeyCode k_keyCode_4;//			0x34
			static const KeyCode k_keyCode_5;//			0x35
			static const KeyCode k_keyCode_6;//			0x36
			static const KeyCode k_keyCode_7;//			0x37
			static const KeyCode k_keyCode_8;//			0x38
			static const KeyCode k_keyCode_9;//			0x39

			static const KeyCode k_keyCode_A;//			0x41
			static const KeyCode k_keyCode_B;//			0x42
			static const KeyCode k_keyCode_C;//			0x43
			static const KeyCode k_keyCode_D;//			0x44
			static const KeyCode k_keyCode_E;//			0x45
			static const KeyCode k_keyCode_F;//			0x46
			static const KeyCode k_keyCode_G;//			0x47
			static const KeyCode k_keyCode_H;//			0x48
			static const KeyCode k_keyCode_I;//			0x49
			static const KeyCode k_keyCode_J;//			0x4A
			static const KeyCode k_keyCode_K;//			0x4B
			static const KeyCode k_keyCode_L;//			0x4C
			static const KeyCode k_keyCode_M;//			0x4D
			static const KeyCode k_keyCode_N;//			0x4E
			static const KeyCode k_keyCode_O;//			0x4F
			static const KeyCode k_keyCode_P;//			0x50
			static const KeyCode k_keyCode_Q;//			0x51
			static const KeyCode k_keyCode_R;//			0x52
			static const KeyCode k_keyCode_S;//			0x53
			static const KeyCode k_keyCode_T;//			0x54
			static const KeyCode k_keyCode_U;//			0x55
			static const KeyCode k_keyCode_V;//			0x56
			static const KeyCode k_keyCode_W;//			0x57
			static const KeyCode k_keyCode_X;//			0x58
			static const KeyCode k_keyCode_Y;//			0x59
			static const KeyCode k_keyCode_Z;//			0x5A

			static const KeyCode k_keyCodeGRAVE;//		0xC0
			static const KeyCode k_keyCodeMINUS;//		0xBD
			static const KeyCode k_keyCodeEQUALS;//		0xBB
			static const KeyCode k_keyCodeBACKSLASH;//	0xDC
			static const KeyCode k_keyCodeLBRACKET;//	0xDB
			static const KeyCode k_keyCodeRBRACKET;//	0xDD
			static const KeyCode k_keyCodeSEMICOLON;//	0xBA
			static const KeyCode k_keyCodeAPOSTROPHE;//	0xDE
			static const KeyCode k_keyCodeCOMMA;//		0xBC
			static const KeyCode k_keyCodePERIOD;//		0xBE
			static const KeyCode k_keyCodeSLASH;//		0xBF

			static const KeyCode k_keyCodeNUMPAD0;//	0x60
			static const KeyCode k_keyCodeNUMPAD1;//	0x61
			static const KeyCode k_keyCodeNUMPAD2;//	0x62
			static const KeyCode k_keyCodeNUMPAD3;//	0x63
			static const KeyCode k_keyCodeNUMPAD4;//	0x64
			static const KeyCode k_keyCodeNUMPAD5;//	0x65
			static const KeyCode k_keyCodeNUMPAD6;//	0x66
			static const KeyCode k_keyCodeNUMPAD7;//	0x67
			static const KeyCode k_keyCodeNUMPAD8;//	0x68
			static const KeyCode k_keyCodeNUMPAD9;//	0x69

			static const KeyCode k_keyCodeMULTIPLY;//	0x6A
			static const KeyCode k_keyCodeDIVIDE;//		0x6F
			static const KeyCode k_keyCodeADD;//		0x6B
			static const KeyCode k_keyCodeSUBTRACT;//	0x6D
			static const KeyCode k_keyCodeDECIMAL;//	0x6E

			static const KeyCode k_keyCode_F1;//		0x70
			static const KeyCode k_keyCode_F2;//			0x71
			static const KeyCode k_keyCode_F3;//			0x72
			static const KeyCode k_keyCode_F4;//			0x73
			static const KeyCode k_keyCode_F5;//			0x74
			static const KeyCode k_keyCode_F6;//			0x75
			static const KeyCode k_keyCode_F7;//			0x76
			static const KeyCode k_keyCode_F8;//			0x77
			static const KeyCode k_keyCode_F9;//			0x78
			static const KeyCode k_keyCode_F10;//		0x79
			static const KeyCode k_keyCode_F11;//		0x7A
			static const KeyCode k_keyCode_F12;//		0x7B

		public:
			IKeyboardController() {}
			virtual ~IKeyboardController() {}

			virtual void onKeyDown(KeyCode key, KeyFlags flags) = 0;
			virtual void onKeyUp(KeyCode key, KeyFlags flags) = 0;
			virtual void onChar(tchar ch) = 0;
		};

		class IMouseController
		{
		public:
			IMouseController() {}
			virtual ~IMouseController() {}

			virtual void onMouseButtonDown(MouseButton button, float x, float y, MouseFlags flags) = 0;
			virtual void onMouseButtonUp(MouseButton button, float x, float y, MouseFlags flags) = 0;
			virtual void onMouseMove(float x, float y, MouseFlags flags) = 0;
			virtual void onMouseWheel(NumNothes wheel, MouseFlags flags) = 0;
		};

#ifdef ANDROID
		class IActivityHandler
		{
		public:
			virtual ~IActivityHandler() {}

			virtual status onActivate() = 0;
			virtual void onDeactivate() = 0;
			virtual status onStep() = 0;

			virtual void onStart() {}
			virtual void onResume() {}
			virtual void onPause() {}
			virtual void onStop() {}
			virtual void onDestroy() {}

			virtual void onSaveState(void** data, size_t* size) {}
			virtual void onConfigurationChanged() {}
			virtual void onLowMemory() {}

			virtual void onCreateWindow() {}
			virtual void onDestroyWindow() {}
			virtual void onGainFocus() {}
			virtual void onLostFocus() {}
		};

		class IInputHandler
		{
		public:
			virtual ~IInputHandler() {}
			virtual bool onTouchEvent(AInputEvent* event) = 0;
			virtual bool onKeyEvent(AInputEvent* event) = 0;
		};
#endif

		//------------------------------------------------------------------------------------------
		//  ISerializer
		//------------------------------------------------------------------------------------------
		class ISerializer
		{
		public:
			virtual ~ISerializer() {}

			virtual ISerializer& operator>>(int8& data) = 0;
			virtual ISerializer& operator>>(uint8& data) = 0;
			virtual ISerializer& operator>>(int16& data) = 0;
			virtual ISerializer& operator>>(uint16& data) = 0;
			virtual ISerializer& operator>>(int32& data) = 0;
			virtual ISerializer& operator>>(uint32& data) = 0;
			virtual ISerializer& operator>>(float& data) = 0;
			virtual ISerializer& operator>>(bool& data) = 0;
			virtual ISerializer& operator>>(String& data) = 0;

			virtual ISerializer& operator<<(int8 data) = 0;
			virtual ISerializer& operator<<(uint8 data) = 0;
			virtual ISerializer& operator<<(int16 data) = 0;
			virtual ISerializer& operator<<(uint16 data) = 0;
			virtual ISerializer& operator<<(int32 data) = 0;
			virtual ISerializer& operator<<(uint32 data) = 0;
			virtual ISerializer& operator<<(float data) = 0;
			virtual ISerializer& operator<<(bool data) = 0;
			virtual ISerializer& operator<<(String& data) = 0;
		};

		typedef SmartPointer<ISerializer> SerializerPtr;

		enum FileOpenMode
		{
			k_readOnly,
			k_writeOnly,
			k_readWrite,
			k_appendOnly,
			k_readAndAppend
		};

		class IFileSystem
		{
		public:
			virtual ~IFileSystem() {};

			virtual ISerializer* createFile(const String& fileName) = 0;
			virtual ISerializer* openFile(const String& fileName, uint32 mode) = 0;
			virtual void closeFile(ISerializer* file) = 0;
		};
}

#endif /* SYSTEM_INTERFACES_H_ */
