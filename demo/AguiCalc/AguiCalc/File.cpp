/*   _____                           
* /\  _  \                     __    
* \ \ \_\ \      __    __  __ /\_\   
*  \ \  __ \   /'_ `\ /\ \/\ \\/\ \  
*   \ \ \/\ \ /\ \_\ \\ \ \_\ \\ \ \ 
*    \ \_\ \_\\ \____ \\ \____/ \ \_\
*     \/_/\/_/ \/____\ \\/___/   \/_/
*                /\____/             
*                \_/__/              
*
* Copyright (c) 2011 Joshua Larouche
* 
*
* License: (BSD)
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name of Agui nor the names of its contributors may
*    be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "File.hpp"
namespace cge
{
	File::File(void)
		: m_file(NULL)
	{
	}

	File::File( const std::string& fileName, const std::string& openMode /*= "r"*/ )
		: m_file(NULL)
	{
		open(fileName,openMode);
	}

	File::~File(void)
	{
	}

	bool File::isOpen() const
	{
		return m_file != NULL;
	}

	bool File::open( const std::string& fileName, const std::string& openMode )
	{
		if(isOpen())
		{
			close();
		}

		m_file = al_fopen(fileName.c_str(),openMode.c_str());

		return m_file != NULL;
	}

	bool File::close()
	{
		if(isOpen())
		{
			al_fclose(m_file);
			m_file = NULL;
			return true;
		}
		return false;
	}

	bool File::eof() const
	{
		if(!isOpen())
		{
			return true;
		}
		else
		{
			return al_feof(m_file);
		}
	}

	bool File::readLine( std::string& buff )
	{
		buff = "";
		if(eof() || !isOpen())
		{
			return false;
		}

		
		int c = 0;
		while((c = al_fgetc(m_file)) != EOF && c != '\n')
		{
			if(c > 13)
			buff += (char)c;
		}

		return true;
	}

	bool File::readAll( std::string& buff )
	{
		buff = "";
		if(eof() || !isOpen())
		{
			return false;
		}


		int c = 0;
		while((c = al_fgetc(m_file)) != EOF)
		{
			if(c > 13 || c == '\n')
				buff += (char)c;
		}

		return true;
	}

}
