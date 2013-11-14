#include "program.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "../../log/log.h"
#include "../../util/filetools.h"
#include "../../util/strings.h"
#include "../../util/error.h"

namespace openage {
namespace engine {
namespace shader {

Program::Program() {
	this->id = glCreateProgram();
}

Program::~Program() {
	glDeleteProgram(this->id);
}

void Program::attach_shader(Shader *s) {
	glAttachShader(this->id, s->id);
}

void Program::link() {
	glLinkProgram(this->id);
	this->check(GL_LINK_STATUS);
	glValidateProgram(this->id);
	this->check(GL_VALIDATE_STATUS);
}

void Program::check(GLenum what_to_check) {
	GLint status;
	glGetProgramiv(this->id, what_to_check, &status);

	if (status != GL_TRUE) {
		GLint loglen;
		glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &loglen);
		char *infolog = new char[loglen];
		glGetProgramInfoLog(this->id, loglen, NULL, infolog);

		const char *what_str;
		switch(what_to_check) {
		case GL_LINK_STATUS:
			what_str = "linking";
			break;
		case GL_VALIDATE_STATUS:
			what_str = "validation";
			break;
		case GL_COMPILE_STATUS:
			what_str = "compiliation";
			break;
		default:
			what_str = "<unknown task>";
			break;
		}

		util::Error e("Program %s failed\n%s", what_str, infolog);
		delete[] infolog;
		throw e;
	}
}

void Program::use() {
	glUseProgram(this->id);
}

void Program::stopusing() {
	glUseProgram((GLuint) 0);
}

GLint Program::get_uniform_id(const char *name) {
	return glGetUniformLocation(this->id, name);
}

} //namespace shader
} //namespace engine
} //namespace openage
