#include "polygonrenderer.h"
#include <iostream>
#include <cmath>
#include "glues/source/glu.h"

using namespace std;

TessResult::TessResult()
{
	currentType = GL_INVALID_ENUM;
}

TessResult::~TessResult()
{

}

void TessResult::BeginCB(GLenum type, void *data)
{
	class TessResult *self = (class TessResult *)data;
	cout << "BeginCB: ";
	if (type == GL_TRIANGLES) cout << "GL_TRIANGLES";
	if (type == GL_TRIANGLE_STRIP) cout << "GL_TRIANGLE_STRIP";
	if (type == GL_TRIANGLE_FAN) cout << "GL_TRIANGLE_FAN";
	cout << endl;
	self->currentType = type;
}

void TessResult::EndCB(void *data)
{
	class TessResult *self = (class TessResult *)data;
	self->currentType = GL_INVALID_ENUM;
	self->rxIndices.clear();
}

void TessResult::VertexCB( void *vertex_data, void *data)
{
	class TessResult *self = (class TessResult *)data;
	size_t vertexIndex = (size_t)vertex_data;
	cout << vertexIndex << endl;
	self->rxIndices.append(vertexIndex);

	if (self->currentType == GL_TRIANGLES) {
		self->triIndices.append(vertexIndex);
	}

	//Convert to simple triangles
	if (self->currentType == GL_TRIANGLE_FAN) {
		if(self->rxIndices.size() == 3) {
			self->triIndices.append(self->rxIndices[0]);
			self->triIndices.append(self->rxIndices[1]);
			self->triIndices.append(self->rxIndices[2]);
		}
		else if(self->rxIndices.size() > 3) {
			self->triIndices.append(self->rxIndices[0]);
			self->triIndices.append(self->rxIndices[self->rxIndices.size()-2]);
			self->triIndices.append(self->rxIndices[self->rxIndices.size()-1]);
		}
	}

	if (self->currentType == GL_TRIANGLE_STRIP) {
		if(self->rxIndices.size() == 3) {
			self->triIndices.append(self->rxIndices[0]);
			self->triIndices.append(self->rxIndices[1]);
			self->triIndices.append(self->rxIndices[2]);
		}
		else if(self->rxIndices.size() > 3) {
			self->triIndices.append(self->rxIndices[self->rxIndices.size()-3]);
			self->triIndices.append(self->rxIndices[self->rxIndices.size()-2]);
			self->triIndices.append(self->rxIndices[self->rxIndices.size()-1]);
		}
	}
}

void TessResult::CombineCB( GLdouble coords[3], void *vertex_data[4],
	GLfloat weight[4], void **outData, void *data)
{
	class TessResult *self = (class TessResult *)data;
}

void TessResult::ErrorCB( GLenum errno, void *data)
{
	class TessResult *self = (class TessResult *)data;
}

// ***********************************************************************

// Vertex shader
const char vs[] = \
"#version 100\n"\
"// Attribute variable that contains coordinates of the vertices.\n" \
"\n" \
"attribute highp vec4 vertex;\n"
"// Main function, which needs to set `gl_Position`.\n" \
"    uniform vec2 offset;\n"
"    uniform mediump float blurAlpha;\n"
"    varying mediump float blurAlpha2;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(vertex.x + offset[0], vertex.y + offset[1], 0., 1.);\n"
"    blurAlpha2 = blurAlpha;\n"
"}\n";

// Fragment shader
const char fs[] =
"#version 100\n"
"// Main fragment shader function.\n"
"#undef lowp\n" //https://forum.qt.io/topic/48001/qglshaderprogram-glsl-es-2-shaders-not-compiling
"#undef mediump\n"
"#undef highp\n"
"    varying mediump vec4 blurAlpha2;\n"
"void main()\n"
"{\n"
"    // We simply set the pixel color to yellow.\n"
"    gl_FragColor = vec4(1., 1., 0., blurAlpha2);\n"
"}\n"
"\n";

// ***********************************************************************

PolygonRenderer::PolygonRenderer():
	m_indexBuffer(QOpenGLBuffer::IndexBuffer),
	m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
	vertexAttr1 = -1;
	offsetUnif = -1;
}

PolygonRenderer::~PolygonRenderer()
{

}

void PolygonRenderer::initialize()
{

	QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &program1);
	vshader1->compileSourceCode(vs);

	QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &program1);
	fshader1->compileSourceCode(fs);

	program1.addShader(vshader1);
	program1.addShader(fshader1);
	program1.link();

	vertexAttr1 = program1.attributeLocation("vertex");
	offsetUnif = program1.uniformLocation("offset");
	blurAlphaUnif = program1.uniformLocation("blurAlpha");

	vertices.clear();
	vertices << QVector2D(-0.2, -0.2);
	vertices << QVector2D(0.0, 0.0);
	vertices << QVector2D(0.3, -0.2);
	vertices << QVector2D(0.2, 0.2);
	vertices << QVector2D(-0.2, 0.2);

	m_vertexBuffer.create();
	m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
	m_vertexBuffer.bind();
	m_vertexBuffer.allocate( vertices.constData(), vertices.size() * sizeof(QVector2D) );

	GLUtesselatorObj *tess = gluNewTess();
	class TessResult tessResult;
	gluTessCallback(tess, GLU_TESS_BEGIN_DATA,   (_GLUfuncptr)tessResult.BeginCB);
	gluTessCallback(tess, GLU_TESS_END_DATA,     (_GLUfuncptr)tessResult.EndCB);
	gluTessCallback(tess, GLU_TESS_VERTEX_DATA,  (_GLUfuncptr)tessResult.VertexCB);
	gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (_GLUfuncptr)tessResult.CombineCB);
	gluTessCallback(tess, GLU_TESS_ERROR_DATA,   (_GLUfuncptr)tessResult.ErrorCB);

	gluTessBeginPolygon(tess, (GLvoid*)&tessResult);
	gluTessBeginContour(tess);

	GLdouble cords[3];
	cords[2] = 0.0;
	for(size_t i=0;i<vertices.size();i++) {
		cords[0] = vertices[i][0];
		cords[1] = vertices[i][1];
		gluTessVertex(tess, cords, (GLvoid*)i);
	}
	gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	gluDeleteTess(tess);

	QVector<unsigned int> indices;
	m_indexBuffer.create();
	m_indexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
	m_indexBuffer.bind();
	m_indexBuffer.allocate( tessResult.triIndices.constData(), tessResult.triIndices.size() * sizeof( unsigned int ) );
}

void PolygonRenderer::render()
{
	program1.bind();

	m_indexBuffer.bind();

	m_vertexBuffer.bind();
	program1.setAttributeBuffer(vertexAttr1, GL_FLOAT, 0, 2, 0);
	program1.enableAttributeArray(vertexAttr1);

	//Sharp version
	QVector2D offset(0.0, 0.0);
	program1.setUniformValueArray(offsetUnif, &offset, 1);
	program1.setUniformValue(blurAlphaUnif, 1.0f);
	glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

	double blurSize = 0.01;

	//Blurry versions
	offset = QVector2D(blurSize, 0.0);
	program1.setUniformValueArray(offsetUnif, &offset, 1);
	program1.setUniformValue(blurAlphaUnif, 0.5f);
	glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

	offset = QVector2D(-blurSize, 0.0);
	program1.setUniformValueArray(offsetUnif, &offset, 1);
	program1.setUniformValue(blurAlphaUnif, 0.5f);
	glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

	offset = QVector2D(0.0, blurSize);
	program1.setUniformValueArray(offsetUnif, &offset, 1);
	program1.setUniformValue(blurAlphaUnif, 0.5f);
	glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

	offset = QVector2D(0.0, -blurSize);
	program1.setUniformValueArray(offsetUnif, &offset, 1);
	program1.setUniformValue(blurAlphaUnif, 0.5f);
	glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

	program1.disableAttributeArray(vertexAttr1);
	program1.release();
}
