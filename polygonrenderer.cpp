#include "polygonrenderer.h"
#include <iostream>
using namespace std;

// Vertex shader
const char vs[] = \
"#version 120\n"\
"// Attribute variable that contains coordinates of the vertices.\n" \
"\n" \
"attribute highp vec4 vertex;\n"
"// Main function, which needs to set `gl_Position`.\n" \
"void main()\n" \
"{\n" \
"    // The final position is transformed from a null signal to a sinewave here.\n" \
"    // We pass the position to gl_Position, by converting it into\n" \
"    // a 4D vector. The last coordinate should be 0 when rendering 2D figures.\n" \
"    gl_Position = vec4(vertex.x, .2 * sin(20 * vertex.x), 0., 1.);\n" \
"}\n";

// Fragment shader
const char fs[] = \
"#version 120\n"\
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
	cout << glGetString(GL_VERSION) << endl;

	QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &program1);
	vshader1->compileSourceCode(vs);

	QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &program1);
	fshader1->compileSourceCode(fs);

	program1.addShader(vshader1);
	program1.addShader(fshader1);
	program1.link();

	vertexAttr1 = program1.attributeLocation("vertex");
	cout << "vertex attribute location: " << vertexAttr1 << endl;
}

void PolygonRenderer::render()
{
	//This time consuming function should not normally be in the main loop,
	//but it is here for simplicity
	vertices.clear();
	for(unsigned i=0;i<10000;i++) {
		float x = ((float)i / 10000.0f) * 2.0f - 1.0;
		vertices << QVector2D(x, 0.0f);
	}

	glDepthMask(false);

	glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program1.bind();
	program1.enableAttributeArray(vertexAttr1);
	program1.setAttributeArray(vertexAttr1, vertices.constData());

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	program1.disableAttributeArray(vertexAttr1);
	program1.release();
}
