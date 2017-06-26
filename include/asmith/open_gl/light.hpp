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

#ifndef ASMITH_OPENGL_LIGHT_HPP
#define ASMITH_OPENGL_LIGHT_HPP

#include <memory>
#include "core.hpp"

namespace asmith { namespace gl {
	
	/*!
		\brief Wrapper class for OpenGL lighting
		\author Adam Smith
		\date Created : 26th June 2017 Modified 26th June 2017
		\version 1.0
	*/
	class light : public std::enable_shared_from_this<light> {
	private:
		const GLenum mID;
		bool mEnabled;
	private:
		light(const light&) = delete;
		light(light&&) = delete;
		light& operator=(const light&) = delete;
		light& operator=(light&&) = delete;

		light(GLenum);
	public:
		static std::shared_ptr<light> get_light(GLenum) throw();

		~light();

		void enable() throw();
		void disable() throw();
		bool is_enabled() const throw();
	};

}}

#endif