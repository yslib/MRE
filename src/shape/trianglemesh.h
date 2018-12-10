#ifndef TRIANGLE_MESH_H_
#define TRIANGLE_MESH_H_

#include  "../mathematics/basetype.h"
//#include "../mathematics/arithmetic.h"
#include "../mathematics/transformation.h"
#include "../core/shape.h"

namespace  ysl
{
	class TriangleMesh {
		std::unique_ptr<Point3f[]> m_vertices;
		int m_nVertex;
		std::unique_ptr<int[]> m_vertexIndices;
		int m_nIndex;
		std::unique_ptr<Vector3f[]> m_normals;
	public:
		TriangleMesh(const Point3f * vertices,
			const Vector3f * normals,

			int nVertex,
			const int * vertexIndices,
			int nIndex,
			const Transform & trans)noexcept :
			m_nVertex(nIndex),
			m_nIndex(nIndex)
		{
			m_vertices.reset(new Point3f[nIndex]);
			m_vertexIndices.reset(new int[nIndex]);
			m_normals.reset(new Vector3f[nIndex]);
			for (int i = 0; i < nIndex; i++)
			{
				m_vertices[i] = trans * vertices[vertexIndices[i]];
			}
			for (int i = 0; i < nIndex; i++)
			{
				m_vertexIndices[i] = vertexIndices[i];
			}
			//create normals vertices
			for (int i = 0; i < nIndex; i++) {
				m_normals[i] = normals[vertexIndices[i]];
			}
		}
		const Point3f * getVerticesArray()const
		{
			return m_vertices.get();
		}
		int getVertexCount()const
		{
			return m_nVertex;
		}
		const int * getIndicesArray()const
		{
			return m_vertexIndices.get();
		}
		int getIndexCount()const
		{
			return m_nIndex;
		}
		const Vector3f *getNormalsArray()const {
			return m_normals.get();
		}
		void transform(const Transform & trans)
		{
			for (int i = 0; i < m_nIndex; i++)
			{
				m_vertices[i] = trans * m_vertices[i];
			}
		}
		friend class Triangle;
	};


	//class Triangle : public Shape 
	//{
	//	std::shared_ptr<TriangleMesh> m_sharedTriangles;
	//	const int * m_vertexIndices;
	//public:
	//	Triangle(std::shared_ptr<TriangleMesh> mesh, int indexOffset, std::shared_ptr<Material> m = nullptr) :Shape(m)
	//	{
	//		m_sharedTriangles = mesh;
	//		m_vertexIndices = &(mesh->m_vertexIndices[indexOffset * 3]);
	//	}
	//	AABB bound()const override
	//	{
	//		return AABB(m_sharedTriangles->m_vertices[m_vertexIndices[0]],
	//			m_sharedTriangles->m_vertices[m_vertexIndices[1]]).unionWith(
	//				m_sharedTriangles->m_vertices[m_vertexIndices[2]]);
	//	}
	//	Float area()const override
	//	{
	//		Vector3f v1 = m_sharedTriangles->m_vertices[m_vertexIndices[1]]
	//			- m_sharedTriangles->m_vertices[m_vertexIndices[0]];
	//		Vector3f v2 = m_sharedTriangles->m_vertices[m_vertexIndices[2]]
	//			- m_sharedTriangles->m_vertices[m_vertexIndices[0]];
	//		return 0.5*Vector3f::Dot(v1, v2);

	//	}
	//	Interaction sample(const Point2f &u) const override {
	//		Point2f s = uniformSampleTriangle(u);
	//		Point3f p0 = m_sharedTriangles->m_vertices[m_vertexIndices[0]];
	//		Point3f p1 = m_sharedTriangles->m_vertices[m_vertexIndices[1]];
	//		Point3f p2 = m_sharedTriangles->m_vertices[m_vertexIndices[2]];

	//		Point3f sp = p0 * s[0] + p1 * s[1] + (1 - s[0] - s[1])*p2;

	//		Vector3f p0Norm = m_sharedTriangles->m_normals[m_vertexIndices[0]];
	//		Vector3f p1Norm = m_sharedTriangles->m_normals[m_vertexIndices[1]];
	//		Vector3f p2Norm = m_sharedTriangles->m_normals[m_vertexIndices[2]];

	//		Interaction isect;
	//		//parameter of the triangle
	//		isect.m_u = u[0];
	//		isect.m_v = u[1];

	//		///TODO::
	//		//surface normal
	//		isect.m_norm = (u[0] * p1Norm + u[1] * p2Norm + (1 - u[0] - u[1])*p0Norm);
	//		//shape
	//		isect.m_pShape = this;
	//		//point
	//		isect.m_p = sp;

	//		//error bound

	//		return isect;
	//	}

	//	bool intersect(const Ray & ray, Float * t, Interaction * interac)const override;
	//	static std::vector<std::shared_ptr<Shape>>
	//		createTriangleMesh(const Point3f * vertices, const Point3f * normals, int nVertex,
	//			const int * vertexIndices, int nIndex, std::unordered_map<int, std::string> & mtlName, MaterialReader & mtlReader,
	//			std::vector<std::shared_ptr<Shape>>* lightShapes,
	//			const Transform & trans)
	//	{
	//		assert(nIndex % 3 == 0);
	//		if (lightShapes != nullptr)
	//			lightShapes->clear();
	//		std::shared_ptr<TriangleMesh> mesh(new TriangleMesh(vertices, normals, nVertex, vertexIndices, nIndex, trans));
	//		std::vector<std::shared_ptr<Shape>> tris;
	//		for (int i = 0; i < nIndex / 3; i++)
	//		{
	//			tris.push_back(std::make_shared<Triangle>(mesh, i));
	//			std::string name = mtlName[i];
	//			//qDebug() << QString::fromStdString(name);
	//			MaterialType type;
	//			if (name == "sphere_transmission") {
	//				type = MaterialType::Glass;
	//			}
	//			else if (name == "sphere_mirror") {
	//				type = MaterialType::Mirror;
	//			}
	//			else {
	//				type = MaterialType::Metal;
	//			}
	//			Color kd = mtlReader[name]["Kd"];
	//			Color ks = mtlReader[name]["Ks"];
	//			Color ka = mtlReader[name]["Ka"];
	//			Color tf = mtlReader[name]["Tf"];
	//			Color niV = mtlReader[name]["Ni"];
	//			Color nsV = mtlReader[name]["Ns"];
	//			Color ke = mtlReader[name]["Ke"];
	//			if (kd.isNull()) {
	//				kd = Spectrum(0);
	//			}
	//			if (ks.isNull()) {
	//				ks = Color(0, 0, 0);
	//			}
	//			if (ka.isNull()) {
	//				ka = Color(0, 0, 0);
	//			}
	//			if (tf.isNull()) {
	//				tf = Color(0, 0, 0);
	//			}
	//			if (ke.isNull()) {
	//				ke = Color(0, 0, 0);
	//			}
	//			else {
	//				//light source
	//				if (lightShapes != nullptr)
	//					lightShapes->push_back(tris.back());
	//			}
	//			Float ni;
	//			if (niV.isNull()) {
	//				ni = 0;
	//			}
	//			else {
	//				ni = niV[0];
	//			}
	//			Float ns;
	//			if (nsV.isNull()) {
	//				ns = 0;
	//			}
	//			else {
	//				ns = nsV[0];
	//			}

	//			///TODO: distiguish different material by MaterialType
	//			tris.back()->setMaterial(std::make_shared<Material>(kd, ks, ka, ke, tf, ni, ns, type));
	//		}

	//		return tris;
	//	}
	//};
}

#endif