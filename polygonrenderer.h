#ifndef POLYGONRENDERER_H
#define POLYGONRENDERER_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <qopenglbuffer.h>

class TessResult
{
protected:
	QVector<size_t> rxIndices;
	GLenum currentType;
public:
	TessResult();
	virtual ~TessResult();

	static void BeginCB(GLenum type, void *data);
	static void EndCB(void *data);
	static void VertexCB( void *vertex_data, void *data);
	static void CombineCB( GLdouble coords[3], void *vertex_data[4],
		GLfloat weight[4], void **outData, void *data);
	static void ErrorCB( GLenum errno, void *data);

	QVector<unsigned int> triIndices;
};

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
	QOpenGLBuffer m_indexBuffer;
	QOpenGLBuffer m_vertexBuffer;
	int vertexAttr1;
	int offsetUnif;
	int blurAlphaUnif;
};

#endif // POLYGONRENDERER_H
