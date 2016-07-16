#pragma once

#include "types.h"

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <wchar.h>

#ifndef STD_STRING_H
#define STD_STRING_H
#pragma warning(disable : 4786)
#include <string>
#endif

//	Name Space declaration
namespace pegas
{
	template<typename _Char>
	class TString: public std::basic_string<_Char, std::char_traits<_Char> >
	{
	public:
		typedef std::basic_string<_Char, std::char_traits<_Char> > TEXT_BASE_CLASS;
		
		// Creators...
		inline TString(){};
		TString(const TString<_Char>& rhs);
		TString(const _Char* text, ...);
		explicit TString(const TEXT_BASE_CLASS& rhs);
		explicit TString(_Char c);
		~TString(){};

		// cast the string directly to a _Char pointer
		inline operator const _Char*()const;

		// copy data from various source types
		TString<_Char>& operator=(const TString<_Char>& rhs);
		TString<_Char>& operator=(const TEXT_BASE_CLASS& rhs);
		TString<_Char>& operator=(const _Char *s);
		TString<_Char>& operator=(_Char c);

		// append data from various sources
		TString<_Char>& operator+=(const TString<_Char>& rhs);
		TString<_Char>& operator+=(const TEXT_BASE_CLASS& rhs);
		TString<_Char>& operator+=(const _Char *s); 
		TString<_Char>& operator+=(_Char c);

		// test equality and inequality (case sensitive)
		bool operator==(const TString<_Char>& rhs);
		bool operator!=(const TString<_Char>& rhs);

		TString<_Char>& trim(_Char c = ' ');
		TString<_Char>& trimStart(_Char c = ' ');
		TString<_Char>& trimEnd(_Char c = ' ');

		//specialization

		// test without case sensitivity
		bool compareWithoutCase(const TString<_Char>& rhs);

		// operator that allows TString<_Char> to be used as an STL map key value
		bool operator<(const TString<_Char>& rhs);

		// format the string from optional parameters
		void format(const _Char* text, ...);
		
		//additional functions
		TString<_Char>& toLower();
		TString<_Char>& toUpper();
	};

	//- Inline Functions ---------------------------------------------------------------------

	//. Creators .............................................................................

	template<typename _Char>
	inline TString<_Char>::TString(const TString<_Char>& rhs)
		:TEXT_BASE_CLASS(rhs)
	{

	}

	template<typename _Char>
	inline TString<_Char>::TString(const TEXT_BASE_CLASS& rhs)
		:TEXT_BASE_CLASS(rhs)
	{

	}

	template<typename _Char>
	inline TString<_Char>::TString(_Char c)
	{
		assign(1,c); 
	}

	//. Operators ............................................................................
	template<typename _Char>
	inline bool TString<_Char>::operator==(const TString<_Char>& rhs)
	{
		return (compare(rhs) == 0);
	}

	template<typename _Char>
	inline bool TString<_Char>::operator!=(const TString<_Char>& rhs)
	{
		return (compare(rhs) != 0);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(const TString<_Char>& rhs) 
	{
		this->assign((const TEXT_BASE_CLASS)rhs);
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(const TEXT_BASE_CLASS& rhs) 
	{
		this->assign((const TEXT_BASE_CLASS) rhs);
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(const _Char *s)
	{
		this->assign(s);
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator=(_Char c)
	{
		this->assign(1,c);
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(const TString<_Char>& rhs)
	{
		append((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(const TEXT_BASE_CLASS& rhs)
	{
		append((const TEXT_BASE_CLASS) rhs); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(const _Char *s)
	{
		append(s); 
		return (*this);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::operator+=(_Char c)
	{
		append(1,c); 
		return (*this);
	}


	//. Helper Functions .....................................................................


	template<typename _Char>
	inline TString<_Char> operator+(const TString<_Char>& lhs, const TString<_Char>& rhs)
	{
		TString<_Char> NewText(lhs);
		NewText+=rhs;
		return(NewText);
	}

	template<typename _Char>
	inline TString<_Char>::operator const _Char*()const
	{
		return (this->c_str());
	}

	/*************************************************************************************
		additional operations
	**************************************************************************************/
	template<typename _Char>
	inline TString<_Char>& TString<_Char>::trim(_Char c)
	{
		return trimStart(c).trimEnd(c);
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::trimStart(_Char c)
	{
		size_t found_pos = find_first_not_of(c);
		if(found_pos != this->npos)
		{
			TEXT_BASE_CLASS trimmedStr(this->begin() + found_pos, this->end());
			assign(trimmedStr);
		}

		return *this;
	}

	template<typename _Char>
	inline TString<_Char>& TString<_Char>::trimEnd(_Char c)
	{
		size_t new_length = this->length();
		for(size_t i = this->length() - 1; i >= 0; --i)
		{
			if((this->at(i) != c) && (this->at(i) != _Char(0)))
				break;
			--new_length;
		}

		if(new_length != this->length())
		{
			this->resize(new_length);
		}

		return *this;
	}

	/*************************************************************************************************
		ANSIString
	*************************************************************************************************/

	template<>
	inline TString<char>::TString(const char* text, ...)
	{
		va_list args;
		int len;
		char* buffer;

		va_start( args, text );

		len = vsnprintf(0, 0, text, args) + 1;
		buffer = new char[len];
		vsnprintf(buffer, len, text, args);
		
		assign(buffer);
		delete[] buffer;
		
		va_end(args);
	}

	// test without case sensitivity
	template<>
	inline bool TString<char>::compareWithoutCase(const TString<char>& rhs)
	{
		char c1, c2;
		size_t l = std::min<size_t>(length(), rhs.length());

		for (size_t i = 0; i < l; i++)
		{

			c1 = (char)::towlower(at(i));
		    c2 = (char)::towlower(rhs.at(i));
		    if (c1 != c2)
		    {
		    	return false;
		    }
		}

	   return true;
	}

	// operator that allows TString<_Char> to be used as an STL map key value
	template<>
	inline bool TString<char>::operator<(const TString<char>& rhs)
	{
		return(strcmp(c_str(), rhs.c_str()) < 0);
	}

	// format the string from optional parameters
	template<>
	inline void TString<char>::format(const char* text, ...)
	{
		va_list args;
		int len;
		char* buffer;

		va_start( args, text );
	
		len = vsnprintf(0, 0, text, args) + 1;
		buffer = new char[len];
		vsnprintf(buffer, len, text, args);

		assign(buffer);
		delete[] buffer;

		va_end(args);
	}

	//additional functions
	template<>
	inline TString<char>& TString<char>::toLower()
	{
		for(size_t i = 0; i < length(); i++)
		{
			at(i) = (char)::towlower(at(i));
		}

		return *this;
	}

	template<>
	inline TString<char>& TString<char>::toUpper()
	{
		for(size_t i = 0; i < length(); i++)
		{
			at(i) = (char)::towupper(at(i));
		}

		return *this;
	}

	/****************************************************************************************
		WideString
	*****************************************************************************************/
	template<>
	inline TString<wchar_t>::TString(const wchar_t* text, ...)
	{
		va_list args;
		int len;
		wchar_t* buffer;

		va_start( args, text );

		len = vswprintf(0, 0, text, args ) + 1;
		buffer = new wchar_t[len];
		vswprintf(buffer, len, text, args );

		assign(buffer);
		delete[] buffer;
		
		va_end(args);
	}

	// test without case sensitivity
	template<>
	inline bool TString<wchar_t>::compareWithoutCase(const TString<wchar_t>& rhs)
	{
		wchar_t c1, c2;
		size_t l = std::min<size_t>(length(), rhs.length());

		for (size_t i = 0; i < l; i++)
		{
			c1 = towlower(at(i));
		    c2 = towlower(rhs.at(i));
		    if (c1 != c2)
		    {
		    	return false;
		    }
		}

	   return true;
	}

	// operator that allows TString<_Char> to be used as an STL map key value
	template<>
	inline bool TString<wchar_t>::operator<(const TString<wchar_t>& rhs)
	{
		return(wcscmp(c_str(), rhs.c_str()) < 0);
	}

	// format the string from optional parameters
	template<>
	inline void TString<wchar_t>::format(const wchar_t* text, ...)
	{
		va_list args;
		int len;
		wchar_t* buffer;

		va_start( args, text );

		len = vswprintf(0, 0, text, args ) + 1;
		buffer = new wchar_t[len];
		vswprintf(buffer, len, text, args );

		assign(buffer);
		delete[] buffer;

		va_end(args);
	}

	//additional functions
	template<>
	inline TString<wchar_t>& TString<wchar_t>::toLower()
	{
		for(size_t i = 0; i < length(); i++)
		{
			at(i) = towlower(at(i));
		}

		return *this;
	}

	template<>
	inline TString<wchar_t>& TString<wchar_t>::toUpper()
	{
		for(size_t i = 0; i < length(); i++)
		{
			at(i) = towupper(at(i));
		}

		return *this;
	}

	typedef TString<char> AString;
	typedef TString<wchar_t> WString;
	typedef TString<tchar> String;
} 

