#ifndef POLYGONRENDERER_H
#define POLYGONRENDERER_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>

class PolygonRenderer
{
public:
	PolygonRenderer();
	~PolygonRenderer();

	void render();
	void initialize();

protected:
	QOpenGLShaderProgram program1;
	QVector<QVector2D> vertices;
	int vertexAttr1;
};

#endif // POLYGONRENDERER_H
