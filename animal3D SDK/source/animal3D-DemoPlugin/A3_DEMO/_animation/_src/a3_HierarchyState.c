/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->hpose_base = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
		{
			poseGroup_out->hpose[i].hpose_base = poseGroup_out->hpose[i - 1].hpose_base + nodeCount;
			poseGroup_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
				// fread
				// -> create

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
				// dumping this to a file. whatever you do to write, your gonna have to do the same thing to read in the same order
				//fwrite
				//	-> 
				// optional

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->hpose_base = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
		{
			state_out->hpose[i].hpose_base = state_out->hpose[i - 1].hpose_base + nodeCount;
			state_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		a3ui32 const hposeCount = sizeof(state->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 i;

		// release everything (one free)
		free(state->hpose->hpose_base);

		// reset pointers
		state->hierarchy = 0;
		for (i = 0; i < hposeCount; ++i)
		{
			state->hpose[i].hpose_base = 0;
			state->hpose[i].hpose_index = 0;
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse local-space matrices
a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		// WIP SETH (this might work? not sure)
		for (i = 0; i < state->hierarchy->numNodes; ++i) 
		{
			a3real4x4TransformInverse(state->localSpaceInv->hpose_base[i].transformMat.m, state->localSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2:
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4TransformInverse(state->objectSpaceInv->hpose_base[i].transformMat.m, state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (a3ui32 i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4Product(state->objectSpaceBindToCurrent->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m,
				state_bind->objectSpaceInv->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}


//-----------------------------------------------------------------------------

//typedef struct NODE {
//	char* name;
//	float length;
//	float offset[3];
//	float euler[3];
//	float colour[3];
//	int noofchildren;
//	NODE** children;
//	NODE* parent;
//	float** froset;
//	float** freuler;
//	float* scale;
//	//BYTE DOFs;
//	//OBJECTINFO* object;
//	//CONSTRAINT* constraints;
//} NODE;
//
//typedef struct MOCAPHEADER {
//	int noofsegments;
//	long noofframes;
//	int datarate;
//	int euler[3][3];
//	float callib;
//	a3boolean degrees;
//	float scalefactor;
//	long currentframe;
//	float floor;
//} MOCAPHEADER;
//
//struct MOCAPSEGMENT {
//	char* name;
//	NODE* root;
//	MOCAPHEADER* header;
//	NODE** nodelist;
//} MOCAPSEGMENT;

// THIS IS THE BIG function, way bigger
// load HTR file, read and store complete pose group and hierarchy
// Referenced https://staffwww.dcs.shef.ac.uk/people/S.Maddock/publications/Motion%20Capture%20File%20Formats%20Explained.pdf for understanding HTR spec
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
		//-----------------------------------------------------------------------------

		enum HTRBlock {
			Header,
			SegmentNameAndHierarchy,
			BasePosition,
			SegmentPoses,
		};
		
		enum HeaderPositions {
			FileType = 0,
			DataType,
			FileVersion,
			NumSegments,
			NumFrames,
			DataFrameRate,
			EulerRotationOrder,
			CalibrationUnits,
			RotationUnits,
			GlobalAxisofGravity,
			BoneLengthAxis,
			ScaleFactor,
		};

		// TODO: unused values
		//int dataFrameRate;
		//float calibrationUnitsScale;
		//a3boolean rotationUsesDegrees;
		//char globalAxisOfGravity;
		//char boneLengthAxis;
		//float scaleFactor;


		enum
		{
			maxSegNames = 256
		};

		enum
		{
			maxLineSize = 512
		};

		char segmentNames[maxSegNames][a3node_nameSize];
		int segmentCount = 0;
		int numFrames = 0;
		int numSegments = 0;
		int parsedPoseCount = 0; // for checking
		a3_SpatialPoseEulerOrder eulerOrder;

		int currentSegmentIndex = -1;
		int currentFrameIndex = 0;

		printf("----------------- HTR LOADING STARTED  ---------------\n");

		FILE* fp;
		fp = fopen(resourceFilePath, "r");
		if (fp == NULL)
		{
			perror("Error opening file");
			return -1;
		}

		int lineNumber = 0;
		char line[512];

		enum HTRBlock currentBlock = Header;
		int blockWordNumber = 0;

		// Iterate through each line
		while (fgets(line, maxLineSize, fp) != NULL) 
		{
			// Ignore empty lines and comments
			if (line[0] == '\n' || line[0] == '#')
			{
				lineNumber--;
				continue;
			}

			// Store a copy of the original, since we do some editing of the line itself
			char originalLine[maxLineSize];
			strcpy(originalLine, line);

			// Gets the first word in the line
			char* word = strtok(line, " \t\n\r");
			if (word == NULL)
				continue;

			// Handle switching between blocks
			if (word[0] == '[') {
				// First block, should be the first word in the file
				if (strcmp(word, "[Header]") == 0)
					currentBlock = Header;

				// Second block that declares the hierarchy parent relationships of segments / nodes / joints
				else if (strcmp(word, "[SegmentNames&Hierarchy]") == 0) {
					currentBlock = SegmentNameAndHierarchy;

					// Do processing that follows completion of header
					// 1. Check if defined bounds are appropriate
					if (segmentCount >= maxSegNames) {
						printf("Too many segments! Array must be resized to support %d\n", segmentCount);
						return -1;
					}
					// 2. Create the hierarchy
					a3ret ret = a3hierarchyCreate(hierarchy_out, numSegments, 0);
					if (ret == -1)
						printf("Failed creating a hierarchy\n");
					else
						printf("Hierarchy created with %d nodes\n", (int)ret);

					// 3. Create the hierarchy group
					ret = a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, numFrames);
					if (ret == -1)
						printf("Failed creating a hierarchy pose group\n");
					else
						printf("Hierarchy pose group created");

					// ?. Initialize sizes of arrays based on info from header
					//if (poseGroup_out->pose == NULL && numSegments > 0 && numFrames > 0) poseGroup_out->pose = calloc(numSegments * numFrames, sizeof(a3_SpatialPose));
					//if (poseGroup_out->hpose == NULL && numSegments > 0 && numFrames > 0) poseGroup_out->hpose = calloc(numSegments * numFrames, sizeof(a3_SpatialPose));
					//hierarchy_out->nodes = calloc(numSegments, sizeof(a3_HierarchyNode));
					poseGroup_out->order[0] = eulerOrder;
				}

				// Third block that sets the base position of each segments / nodes / limbs
				else if (strcmp(word, "[BasePosition]") == 0)
					currentBlock = BasePosition;

				// The rest of the blocks are for each segment, which define the position for each frame
				else 
				{
					// We're in a segment's pose block, such as [Hips]
					currentBlock = SegmentPoses;

					// Parse the segment name to identify which block we're in
					char segmentName[64];
					int segmentNameLength = (int)strlen(word);
					if (segmentNameLength > 2) 
					{
						strncpy(segmentName, word + 1, segmentNameLength - 2);
						segmentName[segmentNameLength - 2] = '\0';

						currentSegmentIndex = -1;
						for (int i = 0; i < segmentCount; i++) 
						{
							if (strcmp(segmentNames[i], segmentName) == 0) 
							{
								currentSegmentIndex = i;
								break;
							}
						}

						if (currentSegmentIndex == -1) 
						{
							if (strcmp(segmentName, "EndOfFile") == 0)
								break;

							printf("Unknown segment in [Poses] block: %s\n", segmentName);
							return -1;
						}
						currentFrameIndex = 0;
					}

					// TODO: 2. Set the base position of that segment
				}

				continue;
			}

			if (currentBlock == BasePosition) {
				// Parse the lines of position data

				char segName[64];
				float tx, ty, tz, rx, ry, rz;

				int parsed = sscanf(originalLine, "%63s %f %f %f %f %f %f",
					segName, &tx, &ty, &tz, &rx, &ry, &rz);
				if (parsed != 7) {
					printf("Failed to parse BasePosition line: %s\n", originalLine);
					return -1;
				}

				// Find pose in the pose array
				int poseIndex = -1;
				for (int i = 0; i < segmentCount; i++) {
					if (strcmp(segmentNames[i], segName) == 0) {
						poseIndex = i;
						break;
					}
				}
				if (poseIndex == -1) {
					printf("Unknown segment in base pose: %s\n", segName);
					return -1;
				}

				a3spatialPoseSetTranslation(poseGroup_out->hpose[poseIndex].hpose_base, tx, ty, tz);
				a3spatialPoseSetRotation(poseGroup_out->hpose[poseIndex].hpose_base, tx, ty, tz);
				//a3SpacialPoseSetScale(&poseGroup_out->hpose[poseIndex].hpose_base, sf)

				lineNumber++;
				continue;
			}
			else if (currentBlock == SegmentPoses && originalLine[0] != '[') {
				int frameIndex;
				float tx, ty, tz, rx, ry, rz, sf;

				int parsed = sscanf(originalLine, "%d %f %f %f %f %f %f %f",
					&frameIndex, &tx, &ty, &tz, &rx, &ry, &rz, &sf);

				if (parsed != 8) {
					printf("Failed to parse pose line: %s\n", originalLine);
					lineNumber++;
					continue;
				}

				if (currentSegmentIndex < 0) {
					printf("Error: pose segment not set before poses.\n");
					return -1;
				}
				if (frameIndex < 1 || frameIndex > numFrames) {
					printf("Warning: frame index %d out of range\n", frameIndex);
				}

				int poseIdx = (frameIndex - 1) * numSegments + currentSegmentIndex;

				poseGroup_out->pose[poseIdx].translate.x = tx;
				poseGroup_out->pose[poseIdx].translate.y = ty;
				poseGroup_out->pose[poseIdx].translate.z = tz;

				poseGroup_out->pose[poseIdx].rotate.x = rx;
				poseGroup_out->pose[poseIdx].rotate.y = ry;
				poseGroup_out->pose[poseIdx].rotate.z = rz;

				poseGroup_out->pose[poseIdx].scale.x = sf;

				parsedPoseCount++;
				lineNumber++;
				blockWordNumber = 0;
				continue;
			}


			while (word != NULL) {
				// Iterate the words

				switch (currentBlock) {
				case Header:
					if (blockWordNumber % 2 == 0) break;
					switch (blockWordNumber / 2)
					{
					case FileType:
						break;
					case DataType:
						break;
					case FileVersion:
						break;
					case NumSegments: {
						// Number of body parts / joints
						int wordInt = (int)strtol(word, NULL, 10);
						numSegments = wordInt;
						poseGroup_out->hposeCount = wordInt; // causing crash

						printf("NumSegments = %d\n", numSegments);
						break;
					}
					case NumFrames:
					{
						// Number of frames across all animations
						int wordInt = (int)strtol(word, NULL, 10);
						numFrames = wordInt;
						poseGroup_out->poseCount = numFrames * numSegments; // causing crash

						printf("NumFrames = %d\n", numFrames);
						break;
					}
					case DataFrameRate:
						break;
					case EulerRotationOrder:

						if (poseGroup_out->order == NULL) poseGroup_out->order = malloc(1 * sizeof(a3_SpatialPoseEulerOrder));

						if (strcmp(word, "XYZ") == 0)
							eulerOrder = a3poseEulerOrder_xyz;
						else if (strcmp(word, "YZX") == 0)
							eulerOrder = a3poseEulerOrder_yzx;
						else if (strcmp(word, "ZXY") == 0)
							eulerOrder = a3poseEulerOrder_zxy;
						else if (strcmp(word, "YXZ") == 0)
							eulerOrder = a3poseEulerOrder_yxz;
						else if (strcmp(word, "XZY") == 0)
							eulerOrder = a3poseEulerOrder_xzy;
						else if (strcmp(word, "ZYX") == 0)
							eulerOrder = a3poseEulerOrder_zyx;
						break;
					case CalibrationUnits:
						break;
					case RotationUnits:
						break;
					case GlobalAxisofGravity:
						break;
					case BoneLengthAxis:
						break;
					case ScaleFactor: {
						float wordFloat = strtof(word, NULL);
						// TODO: use scale factor
						break;
					}
					}
					break;

				case SegmentNameAndHierarchy:
				{
					char* segName = word;
					char* parentName = strtok(NULL, " \t\n\r");
					int segIndex = -1;
					int parentIndex = -1;

					if (hierarchy_out->nodes == NULL) {
						printf("What");
					}

					for (a3ui32 j = 0; j < hierarchy_out->numNodes; j++)
					{
						if (strcmp(segName, hierarchy_out->nodes[j].name) == 0)
						{
							segIndex = (int)j;
						}

						if (strcmp(parentName, "GLOBAL") == 0)
							parentIndex = -1;
						else if (strcmp(parentName, hierarchy_out->nodes[j].name) == 0)
							parentIndex = (int)j;
					}


					if (parentName == NULL) {
						printf("Missing parent for segment: %s\n", segName);
						return -1;
					}

					strcpy(segmentNames[segmentCount], segName);

					if (parentIndex == -2)
					{
						printf("Error, segment declared with unknown parent: %s, the node is %s/n", parentName, segName);
						return -1;
					}

					a3ret ret = a3hierarchySetNode(hierarchy_out, segIndex, parentIndex, segName);
					if (ret == -1)
					{
						printf("Error trying to set node");
					}

					segmentCount++;

					word = strtok(NULL, " \t\n\r");
					blockWordNumber++;
					continue;
					break;
				}
				case BasePosition: {
					// TODO: Parse base poses
					break;
				}
				case SegmentPoses:
				{
					// TODO: parse poses
					break;
				}
				}
				word = strtok(NULL, " \t\n\r");
				blockWordNumber++;
			}
			lineNumber++;
		}
		fclose(fp);
		poseGroup_out->hierarchy = hierarchy_out;

		printf("HTR loading completed!\n\n");
		printf("Parsed (real) segment count: %d\n", segmentCount);
		printf("Parsed (real) pose count: %d\n", parsedPoseCount);
		printf("----------------- HTR LOADING FINISHED ---------------\n");
		return 1;

		//-----------------------------------------------------------------------------
		//****END-TO-DO-PROJECT-2
		//-----------------------------------------------------------------------------
	}
	return -1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
		//-----------------------------------------------------------------------------



		//-----------------------------------------------------------------------------
		//****END-TO-DO-OPTIONAL
		//-----------------------------------------------------------------------------
	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
		//-----------------------------------------------------------------------------



		//-----------------------------------------------------------------------------
		//****END-TO-DO-OPTIONAL
		//-----------------------------------------------------------------------------
	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
		//-----------------------------------------------------------------------------
		//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
		//-----------------------------------------------------------------------------



		//-----------------------------------------------------------------------------
		//****END-TO-DO-OPTIONAL
		//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
