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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"


//-----------------------------------------------------------------------------

// single FK helpers
static inline void a3kinematicsSolveForwardSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_object] = T[parent_object] * T[this_local]
	a3real4x4Product(
		hierarchyState->objectSpace->hpose_base[index].transformMat.m,		// Result: this node object-space.
		hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m,// Left-hand: parent node object-space.
		hierarchyState->localSpace->hpose_base[index].transformMat.m		// Right-hand: this node local space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveForwardRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_object] = T[root_local]
	hierarchyState->objectSpace->hpose_base[index] = hierarchyState->localSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}

// partial FK solver
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement forward kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- object matrix = parent object matrix * local matrix
		//		- else
		//			- copy local matrix to object matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveForwardSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveForwardRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

// single IK helpers
static inline void a3kinematicsSolveInverseSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	a3real4x4Product(hierarchyState->localSpace->hpose_base[index].transformMat.m, // this node local space
		hierarchyState->objectSpaceInv->hpose_base[parentIndex].transformMat.m, // parent node object space inverse
		hierarchyState->objectSpace->hpose_base[index].transformMat.m); // this node object space

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveInverseRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_local] = T[root_object]
	hierarchyState->localSpace->hpose_base[index].transformMat = hierarchyState->objectSpace->hpose_base[index].transformMat;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

// partial IK solver
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement inverse kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- local matrix = inverse parent object matrix * object matrix
		//		- else
		//			- copy object matrix to local matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

		// NOT copied from class
		a3ui32 i = firstIndex;
		for (i; i < nodeCount; ++i)
		{
			if (hierarchyState->hierarchy->nodes[i].parentIndex < 0) 
			{
				// we are the root
				a3kinematicsSolveForwardRoot(hierarchyState, hierarchyState->hierarchy->nodes[i].index);
			}
			else 
			{
				// we are not the root
				a3kinematicsSolveForwardSingle(hierarchyState,
					hierarchyState->hierarchy->nodes[i].index,
					hierarchyState->hierarchy->nodes[i].parentIndex);
			}
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

void a3kinematicsUpdateHierarchyStateFK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// FK pipeline
		//	-> concatenate base pose
		//	-> convert poses to local-space matrices
		//	-> perform recursive FK
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyPoseConcat(activeHS->localSpace,	// local: goal to calculate
			activeHS->animPose,						// holds current sample pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);
		a3hierarchyPoseConvert(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3kinematicsSolveForward(activeHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// IK pipeline
		//	-> perform recursive IK
		//	-> restore local-space matrices to poses
		//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		a3kinematicsSolveInverse(activeHS);
		// we now have the animation pose from this concatonate pose. we have a description of the pose, we now need to conert
		a3hierarchyPoseRestore(
			activeHS->localSpace, // goal: convert local pose description to matrix
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order
		);
		a3hierarchyPoseDeconcat(
			activeHS->animPose, // result is animation pose
			activeHS->localSpace, // LH input: local pose
			baseHS->localSpace, // subtract base lcoal
			activeHS->hierarchy->numNodes); // finally, do fk algorithm


//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateSkin(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS)
{
	if (activeHS->hierarchy == baseHS->hierarchy)
	{
		// FK pipeline extended for skinning and other applications
		//	-> update local-space inverse matrices
		//	-> update object-space inverse matrices
		//	-> update transform from base to current
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyStateUpdateLocalInverse(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}


//-----------------------------------------------------------------------------

// helper to resolve single-joint IK after solver
// This is done at the end of a3kinematicsUpdateLookAtIK
static void a3kinematicsResolvePostIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const nodeIndex, a3real4x4 const j2obj)
{
	// post-IK resolution for single affected joint
	//	-> reassign resolved transform to object-space
	//	-> compute object-space inverse matrix
	//	-> compute local-space matrix
	//	-> restore local-space matrix to pose
	//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// j2obj - joint to object (a3real4x4 is an array. to set it we need to a3SetReal4x4)

	// anytime you do IK, you're going backwards. you know the solution in forward, and you want the local transform that would get you there
	// we only need to perform IK on one joint at a time
	

	// basically a copy for a matrix.
	//a3real4x4SetReal4x4();

	// there's a very similar function in kinematics
	// just remember this is a single pose, not the whole hierarchy

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

// the basis describes the mapping of these axes to the actual joint orientation (basis_hierarchyObj) (basis_affected). the basis tells you which axes maps to whcih axes
// look into the basis class. you will need these functions (a3basisToMat3)
// ONE CAVEAT!! YOUR LOOK AT TARGET could be cancelled out by if your axes are parallel before the cross product, making your head dissapaear. if they are parallel, the up basis chosen is invalid. so we could choose a different basis such as right, change the order of solving the problem so you still get a valid transformation

void a3kinematicsUpdateLookAtIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector,
	a3ui32 const hierarchyObjIndex_affected, a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected)
{

	// he already gives us the basis. they are already calculataed. they are orientations, we dont have to touch them at all
	// itsd just a helper utility

	a3mat3 m_hierarchyObj, m_affected; // we use this for our calculations
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected.m, basis_affected))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
	// FIRST STEP:
	// -> lookAtTarget
	a3real3x3 lookAt;
	a3real3 worldUp = { 0, 1, 0 }; 

	// these are BOTH buckstein approved. there's two here because there's two different ways of going about it
	//a3mat4 hierachy2rig = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;
	a3mat4 rig2hierarchy = sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;
	
	// this is not written by buckstein, may be wrong
	a3mat4 pWorldEffector = poseGroup->hpose->hpose_base[sceneGraphIndex_effector].transformMat;

	// dont change the effector at all. you're either taking the effector into the hierarchy, or youre taking the affected
	// positions are the fourth column of the transformation matrix
	// our target effector in hierarchy / object space
	// we need to move target from world to hierarchy
	a3real4 effectorHierarchySpace;
	a3real4TransformProduct(effectorHierarchySpace, rig2hierarchy.v, pWorldEffector.v); // use this, not product comp

	//a3real3ProductComp(effectorHierarchySpace, jointPos.v, target.v);
	a3real3x3MakeLookAt(lookAt, 0, effectorHierarchySpace, pWorldEffector.v, worldUp);

	// MAIN STEP:
	// solver: build an orthonormal basis (joint-to-object)
	a3real3 directionBasis;
	a3real3 sideBasis;
	a3real3 upBasis;

	// 1. direction basis = target - joint position
	a3real3SetReal3(directionBasis, rig2hierarchy.v);
	a3real3Sub(directionBasis, effectorHierarchySpace);

	// 2. side basis = known up x direction basis
	a3real3Cross(sideBasis, worldUp, directionBasis);
	// 3. up basis = direction basis x side basis
	a3real3Cross(upBasis, directionBasis, sideBasis);

	// 4. normalize all (save this step by normalizing first and second) 
	a3real3Normalize(directionBasis);
	a3real3Normalize(sideBasis);
	a3real3Normalize(upBasis);
	
	a3real4x4 orthonormalBasisMatrix;
	a3real4x4Set(orthonormalBasisMatrix,
		upBasis[0], sideBasis[1], directionBasis[2], 0,
		upBasis[0], sideBasis[1], directionBasis[2], 0,
		upBasis[0], sideBasis[1], directionBasis[2], 0,
		0, 0, 0, 1);


	// LAST STEP:
	// resolve every affected joint:
	// -> 
	// a3kinematicsResolvePostIK
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, orthonormalBasisMatrix);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
//  sceneGraphIndex_hierarchyObj - indices in the scene of the hierarchy
// sceneGraphIndex_effector - index in the scene of the end effector (wrist)
// hierarchyObjIndex_affected - index in hierarchy of the affected end join
// hierarchyObjIndex_affected_base - the base joint of the affected (usually shoulder or hip)
// a3_Basis is a helper utility provided (has a bunch of functions) - it describes how the object or node in question is actually oriented. you need to know this becuase you need to know the direction you're aiming along (good for look at 30:13) when you build the matrix for look at, follow the basis axis. if the characters head is pointing in the wrong direction, take another look at the basis. The matrix itself is just a bunch of vectors. 

void a3kinematicsUpdateLimbIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector_end, a3ui32 const sceneGraphIndex_constraint,
	a3ui32 const hierarchyObjIndex_affected_end, a3ui32 const hierarchyObjIndex_affected_hinge, a3ui32 const hierarchyObjIndex_affected_base,
	a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected_end, a3_Basis const basis_affected_hinge, a3_Basis const basis_affected_base)
{
	a3mat3 m_hierarchyObj, m_affected_end, m_affected_hinge, m_affected_base;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected_end.m, basis_affected_end))
		return;
	if (!a3basisToMat3(m_affected_hinge.m, basis_affected_hinge))
		return;
	if (!a3basisToMat3(m_affected_base.m, basis_affected_base))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
	// this one is more complicated than look at, refer to slide deck for walk through and possible implementation
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// WE HAVE THE WHOLE SCENE
	// from the sceneGraphState, you use the a3ui32 indices
	// effector is the node thaat moves around
	// sceneGraphIndex_hierarchyObj - the index of the hierarchy in the scenee
	// 
	// the target needs to be in the right space. we'd be doign a lot of extra work for doing it in world space
	// make every piece of the problem relative to the hierarchy, making it the center of the universe

	// FIRST STEP:
	// transform everything into the space of the skeleton / hierarchy (use the inverse function we've been using)
	// we need it in this space because its the same space forward kinematics is ultimately solved in
	// you have an effector, constraints, etc, move it into the skeletons space (same space as forward kinematic solution)
	// transform everything into the space of the skeleton / hierarchy

	// MAIN STEP:
		// - IMPLEMENTATION NOTES - 
	// solve joint to object for end, hinge, base
	// -> end position*
	// -> hinge position*
	// ONE THING: you have to check if the target is too far away because you could have a hyper extention. if its too far away, your problem is solved its a straight line ur done
	// 1. base joint to end effector vector (and distance)
	// 2. base joint to pole vector constraint - oyu need to map the arm triangle to the imaginary plane of the pole vector
	//		- geometric solution (describeed in the slides, walks through step by step)
	//		- algebraic solution
	//		- either way, you need the planes normal
	// 3. plane normal = cross product of step 1 and 2 (base to pole) x (base to end)
	// 4. geometric (Heron's formula) or algebraic (law of cosines)
		// -> solves elbow position
	// 5. "look at" solves shoulder and elbow rotations
	// DONT OVER THINK IT ITS NOT THAT MANY LINES OF CODE smiles :)
	// 


	// LAST STEP:
	// resolve every affected joint:
	// -> because each joint depends on the parent, you need to start closer to the root and then down. ORDER MATTERS!
	// a3kinematicsResolvePostIK
	// a3kinematicsResolvePostIK
	// a3kinematicsResolvePostIK

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
