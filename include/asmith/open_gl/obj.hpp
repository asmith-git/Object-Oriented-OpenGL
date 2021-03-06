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

#ifndef ASMITH_OPENGL_OBJ_HPP
#define ASMITH_OPENGL_OBJ_HPP

#include <iostream>
#include <vector>
#include "vertex_array.hpp"

namespace asmith { namespace gl {
	
	/*!
		\brief 
		\author Adam Smith
		\date Created : ? Modified 5th July 2017
		\version 3.4
	*/
	struct obj {
		struct face {
			GLuint vertex;
			GLuint texture_coordinate;
			GLuint normal;
		};

		enum {
			MAX_FACE_POINTS = 8
		};

		struct primative {
			face faces[MAX_FACE_POINTS];
			struct {
				uint8_t count : 7;
				uint8_t smooth_shading : 1;
			};
		};

		struct group {
			std::string name;
			std::vector<primative> faces;
		};

		struct object {
			std::string name;
			std::vector<group> groups;
		};
		
		std::vector<vec3f> vertices;
		std::vector<vec3f> normals;
		std::vector<vec2f> texture_coordinates;
		std::vector<object> objects;

		void load(std::istream&);
		std::shared_ptr<vertex_array> create_vao(context&, GLsizei&) const;
	};
}}

#endif