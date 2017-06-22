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

#include <vector>
#include <stdexcept>
#include "asmith/open_gl/vertex_buffer.hpp"

//! \todo Implement binding at multiple targets
//! \todo Implement global binding stack
//! \todo Implement named buffers

namespace asmith { namespace gl {

	enum {
		BUFFER_TARGET_COUNT = 14
	};

	static uint8_t buffer_target_to_index(const GLenum aTarget) throw() {
		switch (aTarget) {
		case GL_ARRAY_BUFFER				: return 0;
		case GL_DRAW_INDIRECT_BUFFER		: return 1;
		case GL_ELEMENT_ARRAY_BUFFER		: return 2;
		case GL_PIXEL_PACK_BUFFER			: return 3;
		case GL_PIXEL_UNPACK_BUFFER			: return 4;
		case GL_TEXTURE_BUFFER				: return 5;
		case GL_TRANSFORM_FEEDBACK_BUFFER	: return 6;
#if ASMITH_GL_VERSION_GE(3,1)
		case GL_COPY_READ_BUFFER			: return 7;
		case GL_COPY_WRITE_BUFFER			: return 8;
		case GL_UNIFORM_BUFFER				: return 9;
#endif
#if ASMITH_GL_VERSION_GE(4,2)
		case GL_ATOMIC_COUNTER_BUFFER		: return 10;
#endif
#if ASMITH_GL_VERSION_GE(4,3)
		case GL_DISPATCH_INDIRECT_BUFFER	: return 11;
		case GL_SHADER_STORAGE_BUFFER		: return 12;
#endif
#if ASMITH_GL_VERSION_GE(4,4)
		case GL_QUERY_BUFFER				: return 13;
#endif
		default								: return BUFFER_TARGET_COUNT;
		}
	}

	static uint8_t index_to_buffer_targer(const GLenum aTarget) throw() {
		switch (aTarget) {
		case 0: return GL_ARRAY_BUFFER;
		case 1: return GL_DRAW_INDIRECT_BUFFER;
		case 2: return GL_ELEMENT_ARRAY_BUFFER;
		case 3: return GL_PIXEL_PACK_BUFFER;
		case 4: return GL_PIXEL_UNPACK_BUFFER;
		case 5: return GL_TEXTURE_BUFFER;
		case 6: return GL_TRANSFORM_FEEDBACK_BUFFER;
#if ASMITH_GL_VERSION_GE(3,1)
		case 7: return GL_COPY_READ_BUFFER;
		case 8: return GL_COPY_WRITE_BUFFER;
		case 9: return GL_UNIFORM_BUFFER;
#endif
#if ASMITH_GL_VERSION_GE(4,2)
		case 10: return GL_ATOMIC_COUNTER_BUFFER;
#endif
#if ASMITH_GL_VERSION_GE(4,3)
		case 11: return GL_DISPATCH_INDIRECT_BUFFER;
		case 12: return GL_SHADER_STORAGE_BUFFER;
#endif
#if ASMITH_GL_VERSION_GE(4,4)
		case 13: return GL_QUERY_BUFFER;
#endif
		default: return GL_INVALID_ENUM;
		}
	}

	enum : GLenum {
		DEFAULT_BUFFER_TARGET =
#if ASMITH_GL_VERSION_GE(3,1)
		GL_ARRAY_BUFFER
#else
		GL_COPY_WRITE_BUFFER
#endif
	};

	static std::weak_ptr<vertex_buffer> VERTEX_BUFFER_TARGETS[BUFFER_TARGET_COUNT];
	
	// vertex_buffer

	vertex_buffer::vertex_buffer() :
		mTarget(GL_INVALID_ENUM),
		mSize(0),
		mUsage(GL_INVALID_ENUM),
		mIsMapped(false)
	{}

	vertex_buffer::vertex_buffer(GLenum aUsage) :
		mTarget(GL_INVALID_ENUM),
		mSize(0),
		mUsage(aUsage),
		mIsMapped(false)
	{}

	vertex_buffer::~vertex_buffer() {
		if(is_mapped()) unmap();
		if(is_bound()) unbind();
	}

	GLsizeiptr vertex_buffer::size() const throw() {
		return mSize;
	}

	bool vertex_buffer::set_usage(GLenum aUsage) throw() {
		if(is_created()) return false;
		mUsage = aUsage;
		return true;
	}

	GLenum vertex_buffer::get_usage() const throw() {
		return mUsage;
	}

	void vertex_buffer::buffer(const GLvoid* aData, GLsizeiptr aSize) {
		if(! is_created()) throw std::runtime_error("asmith::gl::vertex_buffer::set_data : Buffer does not exist");
		if(mUsage == GL_INVALID_ENUM) throw std::runtime_error("asmith::gl::vertex_buffer::set_data : Usage not set");

		bool autoBind = false;
		if(! is_bound()) {
			bind(DEFAULT_BUFFER_TARGET);
			autoBind = true;
		}

		mSize = aSize;
#if ASMITH_GL_VERSION_LE(2, 1)		
		glBufferDataARB(mTarget, aSize, aData, mUsage);
#else
		glBufferData(mTarget, aSize, aData, mUsage);
#endif
		if(autoBind) unbind();
	}

	void vertex_buffer::sub_buffer(GLsizeiptr aOffset, const GLvoid* aData, GLsizeiptr aSize) {
		if(!is_created()) throw std::runtime_error("asmith::gl::vertex_buffer::set_data : Buffer does not exist");

		bool autoBind = false;
		if(! is_bound()) {
			bind(DEFAULT_BUFFER_TARGET);
			autoBind = true;
		}

		mSize = aSize;
#if ASMITH_GL_VERSION_LE(2, 1)		
		glBufferSubDataARB(mTarget, aOffset, aSize, aData);
#else
		glBufferSubData(mTarget, aOffset, aSize, aData);
#endif
		if(autoBind) unbind();
	}

	bool vertex_buffer::bind(GLenum aTarget) throw() {
		if(is_bound()) return false;

#if ASMITH_GL_VERSION_LE(2,1)
		glBindBufferARB(aTarget, mID);
#else
		glBindBuffer(aTarget, mID);
#endif

		const std::shared_ptr<vertex_buffer> ptr = std::dynamic_pointer_cast<vertex_buffer>(shared_from_this());
		const uint8_t i = buffer_target_to_index(aTarget);
		mPreviousBinding = VERTEX_BUFFER_TARGETS[i];
		VERTEX_BUFFER_TARGETS[i] = ptr;
		mTarget = aTarget;
		return true;
	}

	bool vertex_buffer::unbind() throw() {
		if(! is_bound()) return false;
		if(is_mapped()) unmap();

		const uint8_t i = buffer_target_to_index(mTarget); 
		std::shared_ptr<vertex_buffer> prev = mPreviousBinding.lock();
#if ASMITH_GL_VERSION_LE(2,1)
		glBindBufferARB(mTarget, prev ? prev->get_id() : 0);
#else
		glBindBuffer(mTarget, prev ? prev->get_id() : 0);
#endif
		VERTEX_BUFFER_TARGETS[i] = mPreviousBinding;
		mTarget = GL_INVALID_ENUM;
		return true;
	}

	bool vertex_buffer::is_currently_bound() const throw() {
		if(mTarget == GL_INVALID_ENUM) return false;
		const uint8_t i = buffer_target_to_index(mTarget);
		std::shared_ptr<vertex_buffer> prev = VERTEX_BUFFER_TARGETS[i].lock();
		if(! prev) return false;
		return prev->get_id() == mID;
	}

	bool vertex_buffer::is_bound() const throw() {
		return mTarget != GL_INVALID_ENUM;
	}

	GLenum vertex_buffer::get_bind_target() const throw() {
		return mTarget;
	}

	void vertex_buffer::create() {
		if(is_created()) destroy();

#if ASMITH_GL_VERSION_LE(2,1)
		glGenBuffersARB(1, &mID);
#else
		glGenBuffers(1, &mID);
#endif
		mSize = 0;
		if(mID == object::INVALID_ID) throw std::runtime_error("asmith::gl::vertex_buffer::create : glGenBuffers returned 0");
	}

	void vertex_buffer::destroy() {
		if(! is_created()) return;
#if ASMITH_GL_VERSION_LE(2,1)
		glDeleteBuffersARB(1, &mID);
#else
		glDeleteBuffers(1, &mID);
#endif
		mID = 0;
		mSize = 0;
	}

	void* vertex_buffer::map(GLenum aAccess) throw() {
		if(is_mapped() || ! is_currently_bound()) return nullptr;

		mIsMapped = true;
#if ASMITH_GL_VERSION_LE(2,1)
		return glMapBufferARB(mTarget, aAccess);
#else
		return glMapBuffer(mTarget, aAccess);
#endif
	}

	void* vertex_buffer::map_range(GLsizeiptr aOffset, GLsizeiptr aLength, GLenum aAccess) throw() {
		if(is_mapped() || ! is_currently_bound()) return nullptr;

		mIsMapped = true;
#if ASMITH_GL_VERSION_LE(2,1)
		return glMapBufferRangeARB(mTarget, aOffset, aLength, aAccess);
#else
		return glMapBufferRange(mTarget, aOffset, aLength, aAccess);
#endif
	}

	bool vertex_buffer::unmap() throw() {
		if(! (is_mapped() && is_currently_bound())) return false;
#if ASMITH_GL_VERSION_LE(2,1)
		return glUnmapBufferARB(mTarget);
#else
		return glUnmapBuffer(mTarget);
#endif
		return true;
	}

	bool vertex_buffer::is_mapped() const throw() {
		return mIsMapped;
	}

}}