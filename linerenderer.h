#ifndef LINERENDERER_H
#define LINERENDERER_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>

class LineRenderer
{
public:
	LineRenderer();
	~LineRenderer();

	void render();
	void initialize();

protected:
	QOpenGLShaderProgram program1;
	QVector<QVector2D> vertices;
	QVector<QVector2D> unitNormal;
	QVector<QVector2D> extrusion;
	QVector<QVector3D> vertexColours;
	QVector<GLfloat> lineWidth;
	int vertexAttr1;
	int unitNormalAttr;
	int lineWidthAttr;
	int colourAttr;
	int extrusionAttr;

	void addLineSegment(
			float x0, float x1, float x2, float x3,
			float y0, float y1, float y2, float y3,
			float w0, float w1, float w2, float w3);
};

#endif // LINERENDERER_H
