#include <Mesh/PointCloud.h>
#include <Algorithms/PoissonDiscSampling.h>

namespace GLR {
	PointCloud::PointCloud() {
		m_Points = PDS::GeneratePoints(glm::vec2(1.0f, 1.0f), 1.0f, 30);
		LOG_TRACE(m_Points.size());
	}
}