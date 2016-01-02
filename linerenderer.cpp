#include "linerenderer.h"
#include <iostream>
#include <cmath>
using namespace std;

// Vertex shader
const char vs[] = \
"#version 100\n"\
"// Attribute variable that contains coordinates of the vertices.\n" \
"\n" \
"attribute highp vec2 vertex;\n"
"attribute highp vec2 unitNormal;\n"
"attribute highp vec3 colour;\n"
"attribute highp vec2 extrusion;\n"
"attribute float lineWidth;\n"
"varying vec2 interpNorm;\n"
"varying vec2 interpLinePos;\n"
"varying vec2 interpVertex;\n"
"varying vec3 interpColour;\n"
"// Main function, which needs to set `gl_Position`.\n"
"void main()\n"
"{\n"
"    interpVertex = vec2(vertex.x + (extrusion.x * lineWidth), vertex.y + (extrusion.y * lineWidth));\n"
"    gl_Position = vec4(interpVertex.x, interpVertex.y, 0., 1.);\n"
"    interpNorm = vec2(unitNormal.x, unitNormal.y);\n"
"    interpLinePos = vec2(vertex.x, vertex.y);\n"
"    interpColour = colour;\n"
"}\n";

// Fragment shader
const char fs[] = \
"#version 100\n"
"// Main fragment shader function.\n"
"#undef lowp\n" //https://forum.qt.io/topic/48001/qglshaderprogram-glsl-es-2-shaders-not-compiling
"#undef mediump\n"
"#undef highp\n"
"varying mediump vec2 interpNorm;\n"
"varying mediump vec2 interpLinePos;\n"
"varying mediump vec2 interpVertex;\n"
"varying mediump vec3 interpColour;\n"
"void main()\n"
"{\n"
"	 mediump float awayFromLine = dot(interpNorm, interpVertex-interpLinePos);\n"
"    mediump float alpha = 1.0 - awayFromLine / 0.05;\n"
"    if(alpha < 0.0) alpha = 0.0;\n"
"    gl_FragColor = vec4(interpColour.x, interpColour.y, interpColour.z, alpha);\n"
"}\n"
"\n";

LineRenderer::LineRenderer()
{
	vertexAttr1 = -1;
	unitNormalAttr = -1;
	lineWidthAttr = -1;
	colourAttr = -1;
	extrusionAttr = -1;
}

LineRenderer::~LineRenderer()
{

}

inline float Mag(float &x, float &y)
{
	return pow(pow(x, 2.0f)+pow(y, 2.0f),0.5f);
}

void Normalize(float &x, float &y)
{
	float magn = Mag(x, y);
	if(magn > 0.0f) {
		x /= magn;
		y /= magn;
	}
}

void CalcBisection(float x0, float y0, float x1, float y1, float x2, float y2,
	float &extrusionxOut, float &extrusionyOut)
{	
	//Get bisection vector
	float v01x = x1 - x0;
	float v01y = y1 - y0;
	Normalize(v01x, v01y);
	float v12x = x2 - x1;
	float v12y = y2 - y1;
	Normalize(v12x, v12y);
	float bisectx = v01x - v12x;
	float bisecty = v01y - v12y;
	Normalize(bisectx, bisecty);

	float n01x = v01y;
	float n01y = -v01x;
	float n12x = v12y;
	float n12y = -v12x;

	float dotBisectWithLineNorm = bisectx * n12x + bisecty * n12y;
	if(dotBisectWithLineNorm != 0.0f)
	{
		if(abs(dotBisectWithLineNorm) > 1.0f) {
			if(dotBisectWithLineNorm > 0.0f)
				dotBisectWithLineNorm = 1.0f;
			else
				dotBisectWithLineNorm = -1.0f;
		}
		dotBisectWithLineNorm = abs(dotBisectWithLineNorm);

		extrusionxOut = n12x / dotBisectWithLineNorm;
		extrusionyOut = n12y / dotBisectWithLineNorm;
	}
	else
	{
		extrusionxOut = n12x;
		extrusionyOut = n12y;
	}
}


void LineRenderer::addLineSegment(
		float x0, float x1, float x2, float x3,
		float y0, float y1, float y2, float y3,
		float w0, float w1, float w2, float w3)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	float nx = dy;
	float ny = -dx;
	Normalize(nx, ny);

	float extrusion01x = 0.0f, extrusion01y = 0.0f;
	CalcBisection(x0, y0, x1, y1, x2, y2, extrusion01x, extrusion01y);
	float extrusion12x = 0.0f, extrusion12y = 0.0f;
	CalcBisection(x1, y1, x2, y2, x3, y3, extrusion12x, extrusion12y);

	vertices << QVector2D(x1, y1);
	vertices << QVector2D(x1, y1);
	vertices << QVector2D(x2, y2);
	unitNormal << QVector2D(nx, ny);
	unitNormal << QVector2D(-nx, -ny);
	unitNormal << QVector2D(nx, ny);

	vertexColours << QVector3D(0.5, 0.5, 0.5);
	vertexColours << QVector3D(0.5, 0.5, 0.5);
	vertexColours << QVector3D(0.5, 0.5, 0.5);
	lineWidth.append(w1);
	lineWidth.append(w1);
	lineWidth.append(w2);
	extrusion << QVector2D(extrusion01x, extrusion01y);
	extrusion << QVector2D(-extrusion01x, -extrusion01y);
	extrusion << QVector2D(extrusion12x, extrusion12y);

	vertices << QVector2D(x2, y2);
	vertices << QVector2D(x2, y2);
	vertices << QVector2D(x1, y1);
	unitNormal << QVector2D(nx, ny);
	unitNormal << QVector2D(-nx, -ny);
	unitNormal << QVector2D(-nx, -ny);

	vertexColours << QVector3D(0.5, 0.5, 0.5);
	vertexColours << QVector3D(0.5, 0.5, 0.5);
	vertexColours << QVector3D(0.5, 0.5, 0.5);
	lineWidth.append(w2);
	lineWidth.append(w2);
	lineWidth.append(w1);
	extrusion << QVector2D(extrusion12x, extrusion12y);
	extrusion << QVector2D(-extrusion12x, -extrusion12y);
	extrusion << QVector2D(-extrusion01x, -extrusion01y);

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
	colourAttr = program1.attributeLocation("colour");
	extrusionAttr = program1.attributeLocation("extrusion");

	vertices.clear();
	unitNormal.clear();
	lineWidth.clear();
	vertexColours.clear();
	extrusion.clear();




/*
	for(unsigned i=0;i<100;i++) {
		//https://www.mapbox.com/blog/drawing-antialiased-lines/

		float x0 = ((float)(i-1) / 100.0f) * 2.0f - 1.0f;
		float x1 = ((float)i / 100.0f) * 2.0f - 1.0f;
		float x2 = ((float)(i+1) / 100.0f) * 2.0f - 1.0f;
		float x3 = ((float)(i+2) / 100.0f) * 2.0f - 1.0f;
		float y0 = .2 * sin(20.0f * x0);
		float y1 = .2 * sin(20.0f * x1);
		float y2 = .2 * sin(20.0f * x2);
		float y3 = .2 * sin(20.0f * x3);
		float dx = x2 - x1;
		float dy = y2 - y1;
		float w0 = 0.05;
		float w1 = 0.05;
		float w2 = 0.05;
		float w3 = 0.05;

		addLineSegment(
			x0, x1, x2, x3,
			y0, y1, y2, y3,
			w0, w1, w2, w3);
	}*/

	addLineSegment(
		-1.0, -0.9, 0.4, 0.4,
		-0.9, -0.9, -0.9, 0.7,
		0.08, 0.08, 0.08, 0.08);

	addLineSegment(
		-0.9, 0.4, 0.4, 0.4,
		-0.9, -0.9, 0.7, 0.9,
		0.08, 0.08, 0.08, 0.08);

}

void LineRenderer::render()
{
	glDisable(GL_DEPTH_TEST);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_BLEND);

	glClearColor(0.5f, 0.5f, 0.7f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program1.bind();
	program1.setAttributeArray(vertexAttr1, vertices.constData());
	program1.setAttributeArray(unitNormalAttr, unitNormal.constData());
	program1.setAttributeArray(lineWidthAttr, lineWidth.constData(), 1);
	program1.setAttributeArray(colourAttr, vertexColours.constData());
	program1.setAttributeArray(extrusionAttr, extrusion.constData());

	program1.enableAttributeArray(vertexAttr1);
	program1.enableAttributeArray(unitNormalAttr);
	program1.enableAttributeArray(lineWidthAttr);
	program1.enableAttributeArray(colourAttr);
	program1.enableAttributeArray(extrusionAttr);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	program1.disableAttributeArray(vertexAttr1);
	program1.disableAttributeArray(unitNormalAttr);
	program1.disableAttributeArray(lineWidthAttr);
	program1.disableAttributeArray(colourAttr);
	program1.disableAttributeArray(extrusionAttr);

	program1.release();
}
