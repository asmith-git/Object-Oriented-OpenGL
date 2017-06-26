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

#include "asmith/open_gl/light.hpp"

namespace asmith { namespace gl {

	static std::shared_ptr<light> LIGHTS[GL_MAX_LIGHTS];
	
	// light

	std::shared_ptr<light> light::get_light(GLenum aID) throw() {
		if(aID < GL_LIGHT0 || aID >= (GL_LIGHT0 + GL_MAX_LIGHTS)) return std::shared_ptr<light>();
		if(! LIGHTS[aID]) LIGHTS[aID].reset(new light(aID));
		return LIGHTS[aID];
	}


	light::light(GLenum aID) :
		mID(aID),
		mEnabled(false)
	{}

	light::~light() {

	}

	void light::enable() throw() {
		if(mEnabled) return;
		glEnable(mID);
		mEnabled = true;
	}

	void light::disable() throw() {
		if(! mEnabled) return;
		glDisable(mID);
		mEnabled = false;
	}

	bool light::is_enabled() const throw() {
		return mEnabled;
	}

	const vec4f& light::get_position() const throw() {
		return mPosition;
	}

	const vec4f& light::get_ambient() const throw() {
		return mAmbient;
	}

	const vec4f& light::get_diffuse() const throw() {
		return mDiffuse;
	}

	const vec4f& light::get_specular() const throw() {
		return mSpecular;
	}

	const vec3f& light::get_spot_direction() const throw() {
		return mDirection;
	}

	GLfloat light::get_spot_exponent() const throw() {
		return mExponent;
	}

	GLfloat light::get_spot_cutoff() const throw() {
		return mCutoff;
	}

	void light::set_position(const vec4f& aValue) throw() {
		mPosition = aValue;
		glLightfv(mID, GL_POSITION, &mPosition[0]);
	}

	void light::set_ambient(const vec4f& aValue) throw() {
		mAmbient = aValue;
		glLightfv(mID, GL_AMBIENT, &mAmbient[0]);
	}

	void light::set_diffuse(const vec4f& aValue) throw() {
		mDiffuse = aValue;
		glLightfv(mID, GL_DIFFUSE, &mDiffuse[0]);
	}

	void light::set_specular(const vec4f& aValue) throw() {
		mSpecular = aValue;
		glLightfv(mID, GL_SPECULAR, &mSpecular[0]);
	}

	void light::set_spot_direction(const vec3f& aValue) throw() {
		mDirection = aValue;
		glLightfv(mID, GL_SPOT_DIRECTION, &mDirection[0]);
	}

	void light::set_spot_exponent(GLfloat aValue) throw() {
		mExponent = aValue;
		glLightfv(mID, GL_SPOT_EXPONENT, &mExponent);
	}

	void light::set_spot_cutoff(GLfloat aValue) throw() {
		mCutoff = aValue;
		glLightfv(mID, GL_SPOT_CUTOFF, &mCutoff);
	}

}}