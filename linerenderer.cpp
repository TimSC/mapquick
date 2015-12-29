#include "linerenderer.h"
#include <iostream>
#include <cmath>
using namespace std;

// Vertex shader
const char vs[] = \
"#version 100\n"\
"// Attribute variable that contains coordinates of the vertices.\n" \
"\n" \
"attribute highp vec4 vertex;\n"
"attribute highp vec4 unitNormal;\n"
"attribute float lineWidth;\n"
"// Main function, which needs to set `gl_Position`.\n" \
"void main()\n" \
"{\n" \
"    gl_Position = vec4(vertex.x + (unitNormal.x * lineWidth), vertex.y + (unitNormal.y * lineWidth), 0., 1.);\n" \
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

LineRenderer::LineRenderer()
{
	vertexAttr1 = -1;
	unitNormalAttr = -1;
	lineWidthAttr = -1;
}

LineRenderer::~LineRenderer()
{

}

void LineRenderer::initialize()
{
	cout << glGetString(GL_VERSION) << endl;

	QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &program1);
	vshader1->compileSourceCode(vs);

	QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &program1);
	fshader1->compileSourceCode(fs);

	program1.addShader(vshader1);
	program1.addShader(fshader1);
	program1.link();

	vertexAttr1 = program1.attributeLocation("vertex");
	unitNormalAttr = program1.attributeLocation("unitNormal");
	lineWidthAttr = program1.attributeLocation("lineWidth");
	cout << "vertex attribute location: " << vertexAttr1 << endl;
	cout << "unitNormal attribute location: " << unitNormalAttr << endl;
	cout << "lineWidth attribute location: " << lineWidthAttr << endl;

	vertices.clear();
	unitNormal.clear();
	lineWidth.clear();

	for(unsigned i=0;i<100;i++) {
		//https://www.mapbox.com/blog/drawing-antialiased-lines/

		float x1 = ((float)i / 100.0f) * 2.0f - 1.0f;
		float x2 = ((float)(i+1) / 100.0f) * 2.0f - 1.0f;
		float y1 = .2 * sin(20.0f * x1);
		float y2 = .2 * sin(20.0f * x2);
		float dx = x2 - x1;
		float dy = y2 - y1;
		float nx = 0.0;
		float ny = 1.0;
		float magn = pow(pow(nx, 2.0f)+pow(ny, 2.0f),0.5f);
		if(magn > 0.0f) {
			nx /= magn;
			ny /= magn;
		}
		vertices << QVector2D(x1, y1);
		vertices << QVector2D(x1, y1);
		vertices << QVector2D(x2, y2);
		unitNormal << QVector2D(nx, ny);
		unitNormal << QVector2D(nx, -ny);
		unitNormal << QVector2D(nx, ny);
		lineWidth.append(0.05f);
		lineWidth.append(0.05f);
		lineWidth.append(0.05f);

		vertices << QVector2D(x2, y2);
		vertices << QVector2D(x2, y2);
		vertices << QVector2D(x1, y1);
		unitNormal << QVector2D(nx, ny);
		unitNormal << QVector2D(nx, -ny);
		unitNormal << QVector2D(nx, -ny);
		lineWidth.append(0.05f);
		lineWidth.append(0.05f);
		lineWidth.append(0.05f);
	}
}

void LineRenderer::render()
{
	glDisable(GL_DEPTH_TEST);

	glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program1.bind();
	program1.setAttributeArray(vertexAttr1, vertices.constData());
	program1.setAttributeArray(unitNormalAttr, unitNormal.constData());
	program1.setAttributeArray(lineWidthAttr, lineWidth.constData(), 1);

	program1.enableAttributeArray(vertexAttr1);
	program1.enableAttributeArray(unitNormalAttr);
	program1.enableAttributeArray(lineWidthAttr);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	program1.disableAttributeArray(vertexAttr1);
	program1.disableAttributeArray(unitNormalAttr);
	program1.disableAttributeArray(lineWidthAttr);

	program1.release();
}
