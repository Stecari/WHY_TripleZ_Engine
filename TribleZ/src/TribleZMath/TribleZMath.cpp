#include "tzpch.h"

#include "TribleZMath.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

namespace TribleZ::Math
{
	/*
	* 参考(抄)gtx/matrix_decompose.inl中的glm::decompose的
	* 就是删掉了一些复杂的东西，比如xyz复合方向上的缩放
	* 作用就是将transform分解成translation,rotation,scale三个向量
	*/
	bool Decomposed(glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
	{
		using namespace glm;
		using T = float;		//只是懒得一个个改而已，也方便后面就一起改

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;
		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		outTranslation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		//Scale
		outScale.x = length(Row[0]);// v3Length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		outScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		outScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

#ifdef DELETE_CODE
		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				outScale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif // DELETE_CODE

		// Now, get the rotations out, as described in the gem.
		//这套是原来库里面被注释掉的，好像也更好理解
		 outRotation.y = asin(-Row[0][2]);
		 if (cos(outRotation.y) != 0) {
			 outRotation.x = atan2(Row[1][2], Row[2][2]);
			 outRotation.z = atan2(Row[0][1], Row[0][0]);
		 } else {
			 outRotation.x = atan2(-Row[2][0], Row[1][1]);
			 outRotation.z = 0;
		 }

		return true;
	}
}