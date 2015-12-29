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
	int vertexAttr1;
};

#endif // LINERENDERER_H