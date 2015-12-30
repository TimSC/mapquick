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
	QVector<QVector3D> vertexColours;
	QVector<GLfloat> lineWidth;
	int vertexAttr1;
	int unitNormalAttr;
	int lineWidthAttr;
	int colourAttr;
};

#endif // LINERENDERER_H
