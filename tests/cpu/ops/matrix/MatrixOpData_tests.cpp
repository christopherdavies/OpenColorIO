// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenColorIO Project.

#include "ops/matrix/MatrixOpData.cpp"

#include "UnitTest.h"

namespace OCIO = OCIO_NAMESPACE;

OCIO_ADD_TEST(MatrixOpData, empty)
{
    OCIO::MatrixOpData m;
    OCIO_CHECK_ASSERT(m.isNoOp());
    OCIO_CHECK_ASSERT(m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_NO_THROW(m.validate());
    OCIO_CHECK_EQUAL(m.getType(), OCIO::OpData::MatrixType);

    OCIO_CHECK_EQUAL(m.getArray().getLength(), 4);
    OCIO_CHECK_EQUAL(m.getArray().getNumValues(), 16);
    OCIO_CHECK_EQUAL(m.getArray().getNumColorComponents(), 4);

    m.getArray().resize(3, 3);

    OCIO_CHECK_EQUAL(m.getArray().getNumValues(), 9);
    OCIO_CHECK_EQUAL(m.getArray().getLength(), 3);
    OCIO_CHECK_EQUAL(m.getArray().getNumColorComponents(), 3);
    OCIO_CHECK_NO_THROW(m.validate());
}

OCIO_ADD_TEST(MatrixOpData, accessors)
{
    OCIO::MatrixOpData m;
    OCIO_CHECK_ASSERT(m.isNoOp());
    OCIO_CHECK_ASSERT(m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_ASSERT(m.isIdentity());
    OCIO_CHECK_NO_THROW(m.validate());

    m.setArrayValue(15, 1 + 1e-5f);

    OCIO_CHECK_ASSERT(!m.isNoOp());
    OCIO_CHECK_ASSERT(!m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_ASSERT(!m.isIdentity());
    OCIO_CHECK_NO_THROW(m.validate());

    m.setArrayValue(1, 1e-5f);
    m.setArrayValue(15, 1.0f);

    OCIO_CHECK_ASSERT(!m.isNoOp());
    OCIO_CHECK_ASSERT(!m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(!m.isDiagonal());
    OCIO_CHECK_ASSERT(!m.isIdentity());
    OCIO_CHECK_NO_THROW(m.validate());

    OCIO_CHECK_EQUAL(m.getFileInputBitDepth(), OCIO::BIT_DEPTH_UNKNOWN);
    OCIO_CHECK_EQUAL(m.getFileOutputBitDepth(), OCIO::BIT_DEPTH_UNKNOWN);
    m.setFileInputBitDepth(OCIO::BIT_DEPTH_UINT10);
    m.setFileOutputBitDepth(OCIO::BIT_DEPTH_UINT8);
    OCIO_CHECK_EQUAL(m.getFileInputBitDepth(), OCIO::BIT_DEPTH_UINT10);
    OCIO_CHECK_EQUAL(m.getFileOutputBitDepth(), OCIO::BIT_DEPTH_UINT8);

    OCIO::MatrixOpData m1{ m };
    OCIO_CHECK_EQUAL(m1.getFileInputBitDepth(), OCIO::BIT_DEPTH_UINT10);
    OCIO_CHECK_EQUAL(m1.getFileOutputBitDepth(), OCIO::BIT_DEPTH_UINT8);

    OCIO::MatrixOpData m2;
    m2 = m;
    OCIO_CHECK_EQUAL(m2.getFileInputBitDepth(), OCIO::BIT_DEPTH_UINT10);
    OCIO_CHECK_EQUAL(m2.getFileOutputBitDepth(), OCIO::BIT_DEPTH_UINT8);
}

OCIO_ADD_TEST(MatrixOpData, offsets)
{
    OCIO::MatrixOpData m;
    OCIO_CHECK_ASSERT(m.isNoOp());
    OCIO_CHECK_ASSERT(m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_ASSERT(!m.hasOffsets());
    OCIO_CHECK_NO_THROW(m.validate());

    m.setOffsetValue(2, 1.0f);
    OCIO_CHECK_ASSERT(!m.isNoOp());
    OCIO_CHECK_ASSERT(m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_ASSERT(m.hasOffsets());
    OCIO_CHECK_NO_THROW(m.validate());
    OCIO_CHECK_EQUAL(m.getOffsets()[2], 1.0f);
}

OCIO_ADD_TEST(MatrixOpData, offsets4)
{
    OCIO::MatrixOpData m;
    OCIO_CHECK_ASSERT(m.isNoOp());
    OCIO_CHECK_ASSERT(m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_ASSERT(!m.hasOffsets());
    OCIO_CHECK_NO_THROW(m.validate());

    m.setOffsetValue(3, -1e-6f);
    OCIO_CHECK_ASSERT(!m.isNoOp());
    OCIO_CHECK_ASSERT(m.isUnityDiagonal());
    OCIO_CHECK_ASSERT(m.isDiagonal());
    OCIO_CHECK_ASSERT(m.hasOffsets());
    OCIO_CHECK_NO_THROW(m.validate());
    OCIO_CHECK_EQUAL(m.getOffsets()[3], -1e-6f);
}

OCIO_ADD_TEST(MatrixOpData, diagonal)
{
    OCIO::MatrixOpDataRcPtr pM = OCIO::MatrixOpData::CreateDiagonalMatrix(0.5);

    OCIO_CHECK_ASSERT(pM->isDiagonal());
    OCIO_CHECK_ASSERT(!pM->hasOffsets());
    OCIO_CHECK_NO_THROW(pM->validate());
    OCIO_CHECK_EQUAL(pM->getArray().getValues()[0], 0.5);
    OCIO_CHECK_EQUAL(pM->getArray().getValues()[5], 0.5);
    OCIO_CHECK_EQUAL(pM->getArray().getValues()[10], 0.5);
    OCIO_CHECK_EQUAL(pM->getArray().getValues()[15], 0.5);
}

#define MATRIX_TEST_HAS_ALPHA(id, val)  \
{                                       \
    mat.getArray()[id] = val + 0.001;   \
    OCIO_CHECK_ASSERT(mat.hasAlpha());  \
    mat.getArray()[id] = val;           \
    OCIO_CHECK_ASSERT(!mat.hasAlpha()); \
}

OCIO_ADD_TEST(MatrixOpData, has_alpha)
{
    OCIO::MatrixOpData mat;
    OCIO_CHECK_ASSERT(!mat.hasAlpha());

    MATRIX_TEST_HAS_ALPHA(3, 0.0);
    MATRIX_TEST_HAS_ALPHA(7, 0.0);
    MATRIX_TEST_HAS_ALPHA(11, 0.0);
    MATRIX_TEST_HAS_ALPHA(12, 0.0);
    MATRIX_TEST_HAS_ALPHA(13, 0.0);
    MATRIX_TEST_HAS_ALPHA(14, 0.0);
    MATRIX_TEST_HAS_ALPHA(15, 1.0);

    mat.getOffsets()[3] = 0.001;
    OCIO_CHECK_ASSERT(mat.hasAlpha());
    mat.getOffsets()[3] = 0.0;
    OCIO_CHECK_ASSERT(!mat.hasAlpha());
}

#undef MATRIX_TEST_HAS_ALPHA

OCIO_ADD_TEST(MatrixOpData, clone)
{
    OCIO::MatrixOpData ref;
    ref.setOffsetValue(2, 1.0f);
    ref.setArrayValue(0, 2.0f);

    OCIO::MatrixOpDataRcPtr pClone = ref.clone();

    OCIO_CHECK_ASSERT(pClone.get());
    OCIO_CHECK_ASSERT(!pClone->isNoOp());
    OCIO_CHECK_ASSERT(!pClone->isUnityDiagonal());
    OCIO_CHECK_ASSERT(pClone->isDiagonal());
    OCIO_CHECK_NO_THROW(pClone->validate());
    OCIO_CHECK_EQUAL(pClone->getType(), OCIO::OpData::MatrixType);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[0], 0.0f);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[1], 0.0f);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[2], 1.0f);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[3], 0.0f);
    OCIO_CHECK_ASSERT(pClone->getArray() == ref.getArray());
}

OCIO_ADD_TEST(MatrixOpData, clone_offsets4)
{
    OCIO::MatrixOpData ref;
    ref.setOffsetValue(0, 1.0f);
    ref.setOffsetValue(1, 2.0f);
    ref.setOffsetValue(2, 3.0f);
    ref.setOffsetValue(3, 4.0f);
    ref.setArrayValue(0, 2.0f);

    OCIO::MatrixOpDataRcPtr pClone = ref.clone();

    OCIO_CHECK_ASSERT(pClone.get());
    OCIO_CHECK_ASSERT(!pClone->isNoOp());
    OCIO_CHECK_ASSERT(!pClone->isUnityDiagonal());
    OCIO_CHECK_ASSERT(pClone->isDiagonal());
    OCIO_CHECK_NO_THROW(pClone->validate());
    OCIO_CHECK_EQUAL(pClone->getType(), OCIO::OpData::MatrixType);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[0], 1.0f);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[1], 2.0f);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[2], 3.0f);
    OCIO_CHECK_EQUAL(pClone->getOffsets()[3], 4.0f);
    OCIO_CHECK_ASSERT(pClone->getArray() == ref.getArray());
}

OCIO_ADD_TEST(MatrixOpData, test_construct)
{
    OCIO::MatrixOpData matOp;

    OCIO_CHECK_EQUAL(matOp.getID(), "");
    OCIO_CHECK_EQUAL(matOp.getType(), OCIO::OpData::MatrixType);
    OCIO_CHECK_ASSERT(matOp.getFormatMetadata().getChildrenElements().empty());
    OCIO_CHECK_EQUAL(matOp.getOffsets()[0], 0.0f);
    OCIO_CHECK_EQUAL(matOp.getOffsets()[1], 0.0f);
    OCIO_CHECK_EQUAL(matOp.getOffsets()[2], 0.0f);
    OCIO_CHECK_EQUAL(matOp.getOffsets()[3], 0.0f);
    OCIO_CHECK_EQUAL(matOp.getArray().getLength(), 4);
    OCIO_CHECK_EQUAL(matOp.getArray().getNumColorComponents(), 4);
    OCIO_CHECK_EQUAL(matOp.getArray().getNumValues(), 16);
    const OCIO::ArrayDouble::Values & val = matOp.getArray().getValues();
    OCIO_CHECK_EQUAL(val.size(), 16);
    OCIO_CHECK_EQUAL(val[0], 1.0f);
    OCIO_CHECK_EQUAL(val[1], 0.0f);
    OCIO_CHECK_EQUAL(val[2], 0.0f);
    OCIO_CHECK_EQUAL(val[3], 0.0f);

    OCIO_CHECK_EQUAL(val[4], 0.0f);
    OCIO_CHECK_EQUAL(val[5], 1.0f);
    OCIO_CHECK_EQUAL(val[6], 0.0f);
    OCIO_CHECK_EQUAL(val[7], 0.0f);

    OCIO_CHECK_EQUAL(val[8], 0.0f);
    OCIO_CHECK_EQUAL(val[9], 0.0f);
    OCIO_CHECK_EQUAL(val[10], 1.0f);
    OCIO_CHECK_EQUAL(val[11], 0.0f);

    OCIO_CHECK_EQUAL(val[12], 0.0f);
    OCIO_CHECK_EQUAL(val[13], 0.0f);
    OCIO_CHECK_EQUAL(val[14], 0.0f);
    OCIO_CHECK_EQUAL(val[15], 1.0f);

    OCIO_CHECK_NO_THROW(matOp.validate());

    matOp.getArray().resize(3, 3); // validate() will resize to 4x4

    OCIO_CHECK_EQUAL(matOp.getArray().getNumValues(), 9);
    OCIO_CHECK_EQUAL(matOp.getArray().getLength(), 3);
    OCIO_CHECK_EQUAL(matOp.getArray().getNumColorComponents(), 3);

    OCIO_CHECK_NO_THROW(matOp.validate());

    OCIO_CHECK_EQUAL(matOp.getArray().getNumValues(), 16);
    OCIO_CHECK_EQUAL(matOp.getArray().getLength(), 4);
    OCIO_CHECK_EQUAL(matOp.getArray().getNumColorComponents(), 4);
}

// Validate matrix composition.
OCIO_ADD_TEST(MatrixOpData, composition)
{
    // Create two test ops.
    const float mtxA[] = {  1, 2, 3, 4,
                            4, 5, 6, 7,
                            7, 8, 9, 10,
                            11, 12, 13, 14 };
    const float offsA[] = { 10, 11, 12, 13 };

    OCIO::MatrixOpData mA;
    mA.setFileInputBitDepth(OCIO::BIT_DEPTH_UINT8);
    mA.setFileOutputBitDepth(OCIO::BIT_DEPTH_F16);

    mA.setRGBA(mtxA);
    mA.setRGBAOffsets(offsA);

    const float mtxB[] = { 21, 22, 23, 24,
                           24, 25, 26, 27,
                           27, 28, 29, 30,
                           31, 32, 33, 34 };
    const float offsB[] = { 30, 31, 32, 33 };

    OCIO::MatrixOpDataRcPtr mB = std::make_shared<OCIO::MatrixOpData>();
    mB->setFileInputBitDepth(OCIO::BIT_DEPTH_F16);
    mB->setFileOutputBitDepth(OCIO::BIT_DEPTH_UINT10);

    mB->setRGBA(mtxB);
    mB->setRGBAOffsets(offsB);

    // Correct results.
    const float aim[] = { 534, 624, 714, 804,
                          603, 705, 807, 909,
                          672, 786, 900, 1014,
                          764, 894, 1024, 1154 };
    const float aim_offs[] = { 1040 + 30, 1178 + 31, 1316 + 32, 1500 + 33 };

    // Compose.
    OCIO::ConstMatrixOpDataRcPtr mBConst = mB;
    OCIO::MatrixOpDataRcPtr result(mA.compose(mBConst));

    // Check bit-depths copied correctly.
    OCIO_CHECK_EQUAL(result->getFileInputBitDepth(), OCIO::BIT_DEPTH_UINT8);
    OCIO_CHECK_EQUAL(result->getFileOutputBitDepth(), OCIO::BIT_DEPTH_UINT10);

    const OCIO::ArrayDouble::Values& newCoeff = result->getArray().getValues();

    // Sanity check on size.
    OCIO_CHECK_ASSERT(newCoeff.size() == 16);

    // Coefficient check.
    for (unsigned long i = 0; i < newCoeff.size(); i++)
    {
        OCIO_CHECK_EQUAL(aim[i], newCoeff[i]);
    }

    // Offset check.
    const unsigned long dim = result->getArray().getLength();
    for (unsigned long i = 0; i < dim; i++)
    {
        OCIO_CHECK_EQUAL(aim_offs[i], result->getOffsets()[i]);
    }
}

OCIO_ADD_TEST(MatrixOpData, equality)
{
    OCIO::MatrixOpData m1;
    m1.setArrayValue(0, 2);

    OCIO::MatrixOpData m2;
    m2.setID("invalid_u_id_test");
    m2.setArrayValue(0, 2);

    // id is part of metadata. FormatMetadataImpl is ignored for ==.
    OCIO_CHECK_ASSERT(m1 == m2);

    // File bit-depth is ignored for ==.
    m1.setFileInputBitDepth(OCIO::BIT_DEPTH_UINT8);
    OCIO_CHECK_ASSERT(m1 == m2);

    OCIO::MatrixOpData m3;
    m3.setArrayValue(0, 6);

    OCIO_CHECK_ASSERT(!(m1 == m3));

    OCIO::MatrixOpData m4;
    m4.setArrayValue(0, 2);

    OCIO_CHECK_ASSERT(m1 == m4);

    m4.setOffsetValue(3, 1e-5f);

    OCIO_CHECK_ASSERT(!(m1 == m4));
}

OCIO_ADD_TEST(MatrixOpData, rgb)
{
    OCIO::MatrixOpData m;

    const float rgb[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    m.setRGB(rgb);

    const OCIO::ArrayDouble::Values & v = m.getArray().getValues();
    OCIO_CHECK_EQUAL(v[0], rgb[0]);
    OCIO_CHECK_EQUAL(v[1], rgb[1]);
    OCIO_CHECK_EQUAL(v[2], rgb[2]);
    OCIO_CHECK_EQUAL(v[3], 0.0f);

    OCIO_CHECK_EQUAL(v[4], rgb[3]);
    OCIO_CHECK_EQUAL(v[5], rgb[4]);
    OCIO_CHECK_EQUAL(v[6], rgb[5]);
    OCIO_CHECK_EQUAL(v[7], 0.0f);

    OCIO_CHECK_EQUAL(v[8], rgb[6]);
    OCIO_CHECK_EQUAL(v[9], rgb[7]);
    OCIO_CHECK_EQUAL(v[10], rgb[8]);
    OCIO_CHECK_EQUAL(v[11], 0.0f);

    OCIO_CHECK_EQUAL(v[12], 0.0f);
    OCIO_CHECK_EQUAL(v[13], 0.0f);
    OCIO_CHECK_EQUAL(v[14], 0.0f);
    OCIO_CHECK_EQUAL(v[15], 1.0f);
}

OCIO_ADD_TEST(MatrixOpData, rgba)
{
    OCIO::MatrixOpData m;

    const float rgba[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15 };
    m.setRGBA(rgba);

    const OCIO::ArrayDouble::Values & v = m.getArray().getValues();
    for (unsigned long i = 0; i<16; ++i)
    {
        OCIO_CHECK_EQUAL(v[i], rgba[i]);
    }
}

OCIO_ADD_TEST(MatrixOpData, matrixInverse_identity)
{
    OCIO::MatrixOpData refMatrixOp;

    refMatrixOp.setFileInputBitDepth(OCIO::BIT_DEPTH_F32);
    refMatrixOp.setFileOutputBitDepth(OCIO::BIT_DEPTH_UINT12);

    OCIO_CHECK_ASSERT(refMatrixOp.isDiagonal());
    OCIO_CHECK_ASSERT(refMatrixOp.isIdentity());
    OCIO_CHECK_ASSERT(!refMatrixOp.hasOffsets());

    // Get inverse of reference matrix operation.
    OCIO::MatrixOpDataRcPtr invMatrixOp;
    OCIO_CHECK_ASSERT(!invMatrixOp);
    OCIO_CHECK_NO_THROW(invMatrixOp = refMatrixOp.inverse());
    OCIO_REQUIRE_ASSERT(invMatrixOp);

    // Inverse op should have its input/output bit-depth inverted.
    OCIO_CHECK_EQUAL(invMatrixOp->getFileInputBitDepth(),
                     refMatrixOp.getFileOutputBitDepth());
    OCIO_CHECK_EQUAL(invMatrixOp->getFileOutputBitDepth(),
                     refMatrixOp.getFileInputBitDepth());

    // But still be an identity matrix.
    OCIO_CHECK_ASSERT(invMatrixOp->isDiagonal());
    OCIO_CHECK_ASSERT(invMatrixOp->isIdentity());
    OCIO_CHECK_ASSERT(!invMatrixOp->hasOffsets());
}

OCIO_ADD_TEST(MatrixOpData, matrixInverse_singular)
{
    OCIO::MatrixOpData singularMatrixOp;

    // Set singular matrix values.
    const float mat[16] 
        = { 1.0f, 0.f, 0.f, 0.2f,
            0.0f, 0.f, 0.f, 0.0f,
            0.0f, 0.f, 0.f, 0.0f,
            0.2f, 0.f, 0.f, 1.0f };

    singularMatrixOp.setRGBA(mat);

    OCIO_CHECK_ASSERT(!singularMatrixOp.isNoOp());
    OCIO_CHECK_ASSERT(!singularMatrixOp.isUnityDiagonal());
    OCIO_CHECK_ASSERT(!singularMatrixOp.isDiagonal());
    OCIO_CHECK_ASSERT(!singularMatrixOp.isIdentity());
    OCIO_CHECK_ASSERT(!singularMatrixOp.hasOffsets());

    // Get inverse of singular matrix operation.
    OCIO_CHECK_THROW_WHAT(singularMatrixOp.inverse(),
                          OCIO::Exception,
                          "Singular Matrix can't be inverted");
}

OCIO_ADD_TEST(MatrixOpData, inverse)
{
    OCIO::MatrixOpData refMatrixOp;

    // Set arbitrary matrix and offset values.
    const float matrix[16] = { 0.9f,  0.8f, -0.7f, 0.6f,
                              -0.4f,  0.5f,  0.3f, 0.2f,
                               0.1f, -0.2f,  0.4f, 0.3f,
                              -0.5f,  0.6f,  0.7f, 0.8f };
    const float offsets[4] = { -0.1f, 0.2f, -0.3f, 0.4f };

    refMatrixOp.setRGBA(matrix);
    refMatrixOp.setRGBAOffsets(offsets);

    // Get inverse of reference matrix operation.
    OCIO::MatrixOpDataRcPtr invMatrixOp;
    OCIO_CHECK_NO_THROW(invMatrixOp = refMatrixOp.inverse());
    OCIO_REQUIRE_ASSERT(invMatrixOp);

    const float expectedMatrix[16] = {
        0.75f,                3.5f,               3.5f,              -2.75f,
        0.546296296296297f,   3.90740740740741f,  1.31481481481482f, -1.87962962962963f,
        0.12037037037037f,    4.75925925925926f,  4.01851851851852f, -2.78703703703704f,
       -0.0462962962962963f, -4.90740740740741f, -2.31481481481482f,  3.37962962962963f };

    const float expectedOffsets[4] = {
        1.525f, 0.419444444444445f, 1.38055555555556f, -1.06944444444444f };

    const OCIO::ArrayDouble::Values & invValues =
        invMatrixOp->getArray().getValues();
    const double* invOffsets = invMatrixOp->getOffsets().getValues();

    // Check matrix coeffs.
    for (unsigned long i = 0; i < 16; ++i)
    {
        OCIO_CHECK_CLOSE(invValues[i], expectedMatrix[i], 1e-6f);
    }

    // Check matrix offsets.
    for (unsigned long i = 0; i < 4; ++i)
    {
        OCIO_CHECK_CLOSE(invOffsets[i],expectedOffsets[i], 1e-6f);
    }
}

OCIO_ADD_TEST(MatrixOpData, channel)
{
    OCIO::MatrixOpData refMatrixOp;

    OCIO_CHECK_ASSERT(!refMatrixOp.hasChannelCrosstalk());

    const float offsets[4] = { -0.1f, 0.2f, -0.3f, 0.4f };
    refMatrixOp.setRGBAOffsets(offsets);
    // False: with offsets.
    OCIO_CHECK_ASSERT(!refMatrixOp.hasChannelCrosstalk());

    const float matrix[16] = { 0.9f, 0.0f,  0.0f, 0.0f,
                               0.0f, 0.5f,  0.0f, 0.0f,
                               0.0f, 0.0f, -0.4f, 0.0f,
                               0.0f, 0.0f,  0.0f, 0.8f };
    refMatrixOp.setRGBA(matrix);
    // False: with diagonal.
    OCIO_CHECK_ASSERT(!refMatrixOp.hasChannelCrosstalk());

    const float matrix2[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.000000001f,
                                0.0f, 0.0f, 0.0f, 1.0f };
    refMatrixOp.setRGBA(matrix2);
    // True: with off-diagonal.
    OCIO_CHECK_ASSERT(refMatrixOp.hasChannelCrosstalk());
}

