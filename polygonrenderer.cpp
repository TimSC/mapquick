#include "polygonrenderer.h"

// Vertex shader
const char vs[] = \
"#version 330\n" \
"// Attribute variable that contains coordinates of the vertices.\n" \
"layout(location = 0) in vec2 position;\n" \
"\n" \
"// Main function, which needs to set `gl_Position`.\n" \
"void main()\n" \
"{\n" \
"    // The final position is transformed from a null signal to a sinewave here.\n" \
"    // We pass the position to gl_Position, by converting it into\n" \
"    // a 4D vector. The last coordinate should be 0 when rendering 2D figures.\n" \
"    gl_Position = vec4(position.x, .2 * sin(20 * position.x), 0., 1.);\n" \
"}\n";

// Fragment shader
const char fs[] = \
"#version 330\n" \
"// Output variable of the fragment shader, which is a 4D vector containing the\n" \
"// RGBA components of the pixel color.\n" \
"out vec4 out_color;\n" \
"\n" \
"// Main fragment shader function.\n" \
"void main()\n" \
"{\n" \
"    // We simply set the pixel color to yellow.\n" \
"    out_color = vec4(1., 1., 0., 1.);\n" \
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
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

	QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &program1);
	vshader1->compileSourceCode(vs);

	QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &program1);
	fshader1->compileSourceCode(fs);

	program1.addShader(vshader1);
	program1.addShader(fshader1);
	program1.link();

	vertexAttr1 = program1.attributeLocation("vertex");
}

void PolygonRenderer::render()
{
	//This time consuming function should not normally be in the main loop,
	//but it is here for simplicity
	for(unsigned i=0;i<10000;i++)
		vertices << QVector3D(rand() % 1000, rand() % 1000, 0.0f);

	//clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);

	program1.enableAttributeArray(vertexAttr1);
	program1.setAttributeArray(vertexAttr1, vertices.constData());

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	program1.disableAttributeArray(vertexAttr1);
}
