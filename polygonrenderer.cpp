#include "polygonrenderer.h"
#include <iostream>
#include <cmath>
#include <GL/gl.h>
using namespace std;

// Vertex shader
const char vs[] = \
"#version 100\n"\
"// Attribute variable that contains coordinates of the vertices.\n" \
"\n" \
"attribute highp vec4 vertex;\n"
"// Main function, which needs to set `gl_Position`.\n" \
"void main()\n" \
"{\n" \
"    gl_Position = vec4(vertex.x, vertex.y, 0., 1.);\n" \
"}\n";

// Fragment shader
const char fs[] = \
"#version 100\n"\
"// Main fragment shader function.\n" \
"void main()\n" \
"{\n" \
"    // We simply set the pixel color to yellow.\n" \
"    gl_FragColor = vec4(1., 1., 0., 1.);\n" \
"}\n" \
"\n";

PolygonRenderer::PolygonRenderer()
{
	vertexAttr1 = 0;
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

	vertices.clear();
	vertices << QVector2D(-0.2, -0.2);
	vertices << QVector2D(0.0, 0.0);
	vertices << QVector2D(0.2, -0.2);
	vertices << QVector2D(0.2, 0.2);
	vertices << QVector2D(-0.2, 0.2);

	/*GLUtesselatorObj *tess = gluNewTess();
	gluTessBeginPolygon(tess, this);
	gluTessBeginContour(tess);

	GLdouble cords[3];
	cords[2] = 0.0;
	for(size_t i=0;vertices.size();i++) {
		cords[0] = vertices[i][0];
		cords[1] = vertices[i][1];
		gluTessVertex(tess, cords, NULL);
	}
	gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	gluDeleteTess(tess);*/

}

void PolygonRenderer::render()
{
	program1.bind();
	program1.enableAttributeArray(vertexAttr1);
	program1.setAttributeArray(vertexAttr1, vertices.constData());

	glDrawArrays(GL_POLYGON, 0, vertices.size());

	program1.disableAttributeArray(vertexAttr1);
	program1.release();
}
