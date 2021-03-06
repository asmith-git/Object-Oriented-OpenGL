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

#ifndef ASMITH_OPENGL_CONTEXT_HPP
#define ASMITH_OPENGL_CONTEXT_HPP

#include "core.hpp"

namespace asmith { namespace gl {

	namespace implementation {
		class context_state;
	}
	
	/*!
		\brief
		\author Adam Smith
		\date Created : 30th June 2017 Modified 30th June 2017
		\version 1.0
	*/
	class context {
	private:
		context(const context&) = delete;
		context(context&&) = delete;
		context& operator=(const context&) = delete;
		context& operator=(context&&) = delete;
	public:
		context();
		~context();

		implementation::context_state* const state;
	};

}}

#endif