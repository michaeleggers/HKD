#include "iqm_loader.h" 

#include <assert.h>

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



	hkd_destroy_file(&iqmFile);



	return result;
}
