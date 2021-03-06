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
#include "asmith/open_gl/context_state.hpp"

namespace asmith { namespace gl {
	
	// light

	std::shared_ptr<light> light::get_light(context& aContext, GLenum aID) throw() {
		if(aID < GL_LIGHT0 || aID >= (GL_LIGHT0 + GL_MAX_LIGHTS)) return std::shared_ptr<light>();
		std::shared_ptr<light>& l = aContext.state->lights[aID - GL_LIGHT0];
		if(! l) l.reset(new light(aContext, aID));
		return l;
	}

	void light::enable_lighting(context& aContext) throw() {
		if(aContext.state->lighting_enabled) return;
		aContext.state->lighting_enabled = true;
		glEnable(GL_LIGHTING);
	}

	void light::disable_lighting(context& aContext) throw() {
		if(!aContext.state->lighting_enabled) return;
		aContext.state->lighting_enabled = false;
		glDisable(GL_LIGHTING);
	}

	bool light::is_lighting_enabled(context& aContext) throw() {
		return aContext.state->lighting_enabled;
	}

	void light::set_scene_ambient(context& aContext, const vec4f& aValue) throw() {
		aContext.state->ambient_scene_colour = aValue;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &aContext.state->ambient_scene_colour[0]);
	}

	const vec4f& light::get_scene_ambient(context& aContext) throw() {
		return aContext.state->ambient_scene_colour;
	}

	light::light(context& aContext, GLenum aID) throw() :
		mContext(aContext),
		mPosition(0.f, 0.f, 0.f, 1.f),
		mAmbient(0.f, 0.f, 0.f, 1.f),
		mDiffuse(1.f, 1.f, 1.f, 1.f),
		mSpecular(1.f, 1.f, 1.f, 1.f),
		mDirection(1.f, -1.f, -1.f),
		mConstantAttenuation(1.f),
		mLinearAttenuation(0.f),
		mQuadraticAttenuation(0.f),
		mExponent(30),
		mCutoff(180),
		mID(aID),
		mEnabled(false)
	{}

	light::~light() throw() {

	}

	void light::enable() throw() {
		if(mEnabled) return;
		glEnable(mID);
		set_position(get_position());
		set_ambient(get_ambient());
		set_diffuse(get_diffuse());
		set_specular(get_specular());
		set_spot_direction(get_spot_direction());
		set_spot_exponent(get_spot_exponent());
		set_spot_cutoff(get_spot_cutoff());
		set_constant_attenuation(get_constant_attenuation());
		set_linear_attenuation(get_linear_attenuation());
		set_quadratic_attenuation(get_quadratic_attenuation());
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

	GLint light::get_spot_exponent() const throw() {
		return mExponent;
	}

	GLint light::get_spot_cutoff() const throw() {
		return mCutoff;
	}

	GLfloat light::get_constant_attenuation() const throw() {
		return mConstantAttenuation;
	}

	GLfloat light::get_linear_attenuation() const throw() {
		return mLinearAttenuation;
	}

	GLfloat light::get_quadratic_attenuation() const throw() {
		return mQuadraticAttenuation;
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

	void light::set_spot_exponent(GLint aValue) throw() {
		mExponent = aValue;
		glLighti(mID, GL_SPOT_EXPONENT, mExponent);
	}

	void light::set_spot_cutoff(GLint aValue) throw() {
		mCutoff = aValue;
		glLighti(mID, GL_SPOT_CUTOFF, mCutoff);
	}

	void light::set_constant_attenuation(GLfloat aValue) throw() {
		mConstantAttenuation = aValue;
		glLightf(mID, GL_CONSTANT_ATTENUATION, mConstantAttenuation);
	}

	void light::set_linear_attenuation(GLfloat aValue) throw() {
		mLinearAttenuation = aValue;
		glLightf(mID, GL_LINEAR_ATTENUATION, mLinearAttenuation);
	}

	void light::set_quadratic_attenuation(GLfloat aValue) throw() {
		mQuadraticAttenuation = aValue;
		glLightf(mID, GL_QUADRATIC_ATTENUATION, mQuadraticAttenuation);
	}

}}