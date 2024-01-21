#include "iqm_loader.h" 

#include <assert.h>

#include <string>
#include <vector>

#include "platform.h"

IQMData LoadIQM(const char* file)
{
	IQMData result = {};

	HKD_File iqmFile;
	if (hkd_read_file(file, &iqmFile) != HKD_FILE_SUCCESS) {
		printf("Could not read IQM file: %\n", file);
		exit(-1);
	}

	uint8_t* iqmData = (uint8_t*)iqmFile.data;

	IQMHeader* pHeader = (IQMHeader*)iqmFile.data;
	char* pText = (char*)(iqmData + pHeader->ofsText);
	IQMMesh* pMeshes = (IQMMesh*)(iqmData + pHeader->ofsMeshes);
	IQMVertArray* pVertArrays = (IQMVertArray*)(iqmData + pHeader->ofsVertArrays);
	uint32_t numVertices = pHeader->numVertices;
	IQMTri* pTris = (IQMTri*)(iqmData + pHeader->ofsTris);
	uint8_t* pAdjacency = (uint8_t*)(iqmData + pHeader->ofsAdjacency);
	IQMJoint* pJoints = (IQMJoint*)(iqmData + pHeader->ofsJoints);
	IQMPose* pPoses = (IQMPose*)(iqmData + pHeader->ofsPoses);
	IQMAnim* pAnims = (IQMAnim*)(iqmData + pHeader->ofsAnims);
	uint16_t* pFrames = (uint16_t*)(iqmData + pHeader->ofsFrames);
	uint8_t* pBounds = (uint8_t*)(iqmData + pHeader->ofsBounds);
	char* pComments = (char*)(iqmData + pHeader->ofsComment);
	uint8_t* pExt = (uint8_t*)(iqmData + pHeader->ofsExt);

	printf("IQM Header: %s\n", pHeader->magic);

	uint8_t* pPositions = NULL;
	uint32_t positionStride = 0;
	uint8_t* pTexCoords = NULL;
	uint32_t texCoordStride = 0;
	uint8_t* pBlendIndices = NULL;
	uint32_t blendIndexStride = 0;
	uint8_t* pBlendWeights = NULL;
	uint32_t blendWeightStride = 0;

	for (int i = 0; i < pHeader->numVertArrays; i++) {
		
		IQMVertArray* pVertArray = pVertArrays + i;
		IQMVertArrayType type = static_cast<IQMVertArrayType>(pVertArray->type);
		IQMVertArrayFormat format = static_cast<IQMVertArrayFormat>(pVertArray->format);
		uint32_t numComponents = pVertArray->size;
		uint32_t offset = pVertArray->offset;

		if (type == IQM_POSITION) {
			pPositions = iqmData + offset;
			uint32_t dataSize = 0;
			if (format == IQM_FLOAT) {
				dataSize = sizeof(float);
			}
			else if (format == IQM_DOUBLE) {
				dataSize = sizeof(double);
			}
			positionStride = numComponents * dataSize;
		}
		else if (type == IQM_TEXCOORD) {
			pTexCoords = iqmData + offset;
			uint32_t dataSize = 0;
			if (format == IQM_FLOAT) {
				dataSize = sizeof(float);
			}
			else if (format == IQM_DOUBLE) {
				dataSize = sizeof(double);
			}
			texCoordStride = numComponents * dataSize;
		}
		else if (type == IQM_BLENDINDIXES) {
			pBlendIndices = iqmData + offset;
			uint32_t dataSize = 0;
			if (format == IQM_UBYTE) {
				dataSize = sizeof(uint8_t);
			}
			blendIndexStride = numComponents * dataSize;
		}
		else if (type == IQM_BLENDWEIGHTS) {
			pBlendWeights = iqmData + offset;
			uint32_t dataSize = 0;
			if (format == IQM_UBYTE) {
				dataSize == sizeof(uint8_t);
			}
			blendWeightStride = numComponents * dataSize;
		}
		
		printf("type: %d, format: %d, numComponents: %d, offset: %d\n", type, format, numComponents, offset);
	}
	
	for (int i = 0; i < pHeader->numMeshes; i++) {
		IQMMesh* iqmMesh = pMeshes + i;
		uint32_t firstTri = iqmMesh->firstTri;
		uint32_t numTris = iqmMesh->numTris;
		Mesh mesh = {};
		//mesh.vertices = (IQMVertex*)malloc(iqmMesh->numVerts * sizeof(IQMVertex));
		mesh.material = std::string(pText + iqmMesh->material);
		mesh.firstTri = firstTri;
		mesh.numTris = numTris;

		for (int j = 0; j < numTris; j++) {
			IQMTri* tri = pTris + firstTri + j;

			IQMVertex vertA = {};
			IQMVertex vertB = {};
			IQMVertex vertC = {};

			memcpy(vertA.pos, pPositions + tri->vertex[0] * positionStride, 3 * sizeof(float));
			memcpy(vertB.pos, pPositions + tri->vertex[1] * positionStride, 3 * sizeof(float));
			memcpy(vertC.pos, pPositions + tri->vertex[2] * positionStride, 3 * sizeof(float));

			memcpy(vertA.texCoord, pTexCoords + tri->vertex[0] * texCoordStride, 2 * sizeof(float));
			memcpy(vertB.texCoord, pTexCoords + tri->vertex[1] * texCoordStride, 2 * sizeof(float));
			memcpy(vertC.texCoord, pTexCoords + tri->vertex[2] * texCoordStride, 2 * sizeof(float));

			memcpy(vertA.blendindices, pBlendIndices + tri->vertex[0] * blendIndexStride, 4);
			memcpy(vertB.blendindices, pBlendIndices + tri->vertex[1] * blendIndexStride, 4);
			memcpy(vertC.blendindices, pBlendIndices + tri->vertex[2] * blendIndexStride, 4);

			memcpy(vertA.blendweights, pBlendWeights + tri->vertex[0] * blendWeightStride, 4);
			memcpy(vertB.blendweights, pBlendWeights + tri->vertex[1] * blendWeightStride, 4);
			memcpy(vertC.blendweights, pBlendWeights + tri->vertex[2] * blendWeightStride, 4);


			mesh.vertices.push_back(vertA);
			mesh.vertices.push_back(vertB);
			mesh.vertices.push_back(vertC);
		}
		result.meshes.push_back(mesh);
	}

	hkd_destroy_file(&iqmFile);

	return result;
}

void UnloadIQM(IQMData* iqmData)
{
}
