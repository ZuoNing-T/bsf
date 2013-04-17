/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __Matrix3_H__
#define __Matrix3_H__

#include "CmPrerequisitesUtil.h"

#include "CmVector3.h"

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

namespace CamelotFramework
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/
	/** A 3x3 matrix which can represent rotations around axes.
        @note
            <b>All the code is adapted from the Wild Magic 0.2 Matrix
            library (http://www.geometrictools.com/).</b>
        @par
            The coordinate system is assumed to be <b>right-handed</b>.
    */
    class CM_UTILITY_EXPORT Matrix3
    {
    public:
        /** Default constructor.
            @note
                It does <b>NOT</b> initialize the matrix for efficiency.
        */
		inline Matrix3 () {}
        inline explicit Matrix3 (const float arr[3][3])
		{
			memcpy(m,arr,9*sizeof(float));
		}
        inline Matrix3 (const Matrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(float));
		}
        Matrix3 (float fEntry00, float fEntry01, float fEntry02,
                    float fEntry10, float fEntry11, float fEntry12,
                    float fEntry20, float fEntry21, float fEntry22)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
		}

		/** Exchange the contents of this matrix with another. 
		*/
		inline void swap(Matrix3& other)
		{
			std::swap(m[0][0], other.m[0][0]);
			std::swap(m[0][1], other.m[0][1]);
			std::swap(m[0][2], other.m[0][2]);
			std::swap(m[1][0], other.m[1][0]);
			std::swap(m[1][1], other.m[1][1]);
			std::swap(m[1][2], other.m[1][2]);
			std::swap(m[2][0], other.m[2][0]);
			std::swap(m[2][1], other.m[2][1]);
			std::swap(m[2][2], other.m[2][2]);
		}

        // member access, allows use of construct mat[r][c]
        inline float* operator[] (size_t iRow) const
		{
			return (float*)m[iRow];
		}
        /*inline operator float* ()
		{
			return (float*)m[0];
		}*/
        Vector3 GetColumn (size_t iCol) const;
        void SetColumn(size_t iCol, const Vector3& vec);
        void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

        // assignment and comparison
        inline Matrix3& operator= (const Matrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(float));
			return *this;
		}
        bool operator== (const Matrix3& rkMatrix) const;
        inline bool operator!= (const Matrix3& rkMatrix) const
		{
			return !operator==(rkMatrix);
		}

        // arithmetic operations
        Matrix3 operator+ (const Matrix3& rkMatrix) const;
        Matrix3 operator- (const Matrix3& rkMatrix) const;
        Matrix3 operator* (const Matrix3& rkMatrix) const;
        Matrix3 operator- () const;

        // matrix * vector [3x3 * 3x1 = 3x1]
        Vector3 operator* (const Vector3& rkVector) const;

        // vector * matrix [1x3 * 3x3 = 1x3]
        CM_UTILITY_EXPORT friend Vector3 operator* (const Vector3& rkVector,
            const Matrix3& rkMatrix);

        // matrix * scalar
        Matrix3 operator* (float fScalar) const;

        // scalar * matrix
        CM_UTILITY_EXPORT friend Matrix3 operator* (float fScalar, const Matrix3& rkMatrix);

        // utilities
        Matrix3 transpose () const;
        bool Inverse (Matrix3& rkInverse, float fTolerance = 1e-06) const;
        Matrix3 Inverse (float fTolerance = 1e-06) const;
        float Determinant () const;

        // singular value decomposition
        void SingularValueDecomposition (Matrix3& rkL, Vector3& rkS,
            Matrix3& rkR) const;
        void SingularValueComposition (const Matrix3& rkL,
            const Vector3& rkS, const Matrix3& rkR);

        // Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
        void Orthonormalize ();

        // orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
        void QDUDecomposition (Matrix3& rkQ, Vector3& rkD,
            Vector3& rkU) const;

        float SpectralNorm () const;

        // matrix must be orthonormal
        void ToAxisAngle (Vector3& rkAxis, Radian& rfAngle) const;
		inline void ToAxisAngle (Vector3& rkAxis, Degree& rfAngle) const {
			Radian r;
			ToAxisAngle ( rkAxis, r );
			rfAngle = r;
		}
        void FromAxisAngle (const Vector3& rkAxis, const Radian& fRadians);

        // The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
        // where yaw is rotation about the Up vector, pitch is rotation about the
        // Right axis, and roll is rotation about the Direction axis.
        bool ToEulerAnglesXYZ (Radian& rfYAngle, Radian& rfPAngle,
            Radian& rfRAngle) const;
        bool ToEulerAnglesXZY (Radian& rfYAngle, Radian& rfPAngle,
            Radian& rfRAngle) const;
        bool ToEulerAnglesYXZ (Radian& rfYAngle, Radian& rfPAngle,
            Radian& rfRAngle) const;
        bool ToEulerAnglesYZX (Radian& rfYAngle, Radian& rfPAngle,
            Radian& rfRAngle) const;
        bool ToEulerAnglesZXY (Radian& rfYAngle, Radian& rfPAngle,
            Radian& rfRAngle) const;
        bool ToEulerAnglesZYX (Radian& rfYAngle, Radian& rfPAngle,
            Radian& rfRAngle) const;
        void FromEulerAnglesXYZ (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
        void FromEulerAnglesXZY (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
        void FromEulerAnglesYXZ (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
        void FromEulerAnglesYZX (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
        void FromEulerAnglesZXY (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
        void FromEulerAnglesZYX (const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
        // eigensolver, matrix must be symmetric
        void EigenSolveSymmetric (float afEigenvalue[3],
            Vector3 akEigenvector[3]) const;

        static void TensorProduct (const Vector3& rkU, const Vector3& rkV,
            Matrix3& rkProduct);

		/** Determines if this matrix involves a scaling. */
		inline bool hasScale() const
		{
			// check magnitude of column vectors (==local axes)
			float t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
			if (!Math::RealEqual(t, 1.0, (float)1e-04))
				return true;
			t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
			if (!Math::RealEqual(t, 1.0, (float)1e-04))
				return true;
			t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
			if (!Math::RealEqual(t, 1.0, (float)1e-04))
				return true;

			return false;
		}

		/** Function for writing to a stream.
		*/
		inline CM_UTILITY_EXPORT friend std::ostream& operator <<
			( std::ostream& o, const Matrix3& mat )
		{
			o << "Matrix3(" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " 
                            << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " 
                            << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ")";
			return o;
		}

        static const float EPSILON;
        static const Matrix3 ZERO;
        static const Matrix3 IDENTITY;

    protected:
        // support for eigensolver
        void Tridiagonal (float afDiag[3], float afSubDiag[3]);
        bool QLAlgorithm (float afDiag[3], float afSubDiag[3]);

        // support for singular value decomposition
        static const float ms_fSvdEpsilon;
        static const unsigned int ms_iSvdMaxIterations;
        static void Bidiagonalize (Matrix3& kA, Matrix3& kL,
            Matrix3& kR);
        static void GolubKahanStep (Matrix3& kA, Matrix3& kL,
            Matrix3& kR);

        // support for spectral norm
        static float MaxCubicRoot (float afCoeff[3]);

        float m[3][3];

        // for faster access
        friend class Matrix4;
    };
	/** @} */
	/** @} */

	CM_ALLOW_MEMCPY_SERIALIZATION(Matrix3);
}
#endif
