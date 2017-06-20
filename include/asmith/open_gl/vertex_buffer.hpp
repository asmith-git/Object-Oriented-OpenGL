//	Copyright 2017 Adam Smith
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
// 
//	http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.

#ifndef ASMITH_OPENGL_VERTEX_BUFFER_HPP
#define ASMITH_OPENGL_VERTEX_BUFFER_HPP

#include "object.hpp"

namespace asmith { namespace gl {
	
	/*!
		\brief Base class for OpenGL shader objects
		\author Adam Smith
		\date Created : 4th November 2015 Modified 20th June 2017
		\version 2.0
	*/
	class vertex_buffer : public object {
	private:
		GLenum mTarget;
		GLsizeiptr mSize;
		GLenum mUsage;
	public:
		vertex_buffer();
		vertex_buffer(GLenum);
		~vertex_buffer();

		void set_data(const GLvoid*, GLsizeiptr);
		bool set_usage(GLenum) throw();
		GLenum get_usage() const throw();

		bool bind(GLenum) throw();
		bool unbind(GLenum) throw();

		GLsizeiptr size() const throw();

		// Inherited from object 
		
		void create() override;
		void destroy() override;
	};
}}

#endif