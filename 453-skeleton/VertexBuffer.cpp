#include "VertexBuffer.h"

#include <utility>
#include <glm\ext\vector_float4.hpp>

VertexBuffer::VertexBuffer(GLuint index, GLint size, GLenum dataType, GLuint divisor)
	: bufferID{}
{
	bind();
	glVertexAttribPointer(index, size, dataType, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(index);
	glVertexAttribDivisor(index, divisor);
}

VertexBuffer::VertexBuffer(GLuint beginIndex, GLuint vec4Size, GLuint matrixSize, GLint size, GLenum dataType)
	: bufferID{}
{
	bind();

	for (unsigned int column = 0; column < matrixSize; column++)
	{
		glEnableVertexAttribArray(beginIndex + column);
		glVertexAttribPointer(beginIndex + column, size, dataType, GL_FALSE, 4 * vec4Size, (void*)(column * vec4Size));
		glVertexAttribDivisor(beginIndex + column, 1);
	}

}



void VertexBuffer::uploadData(GLsizeiptr size, const void* data, GLenum usage) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
