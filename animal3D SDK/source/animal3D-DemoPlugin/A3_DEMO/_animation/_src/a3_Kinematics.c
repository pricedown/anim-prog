/*
Authors:
Joseph Isaacs & Seth Riddensdale
*/

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
	hierarchyState->localSpace->hpose_base[index] = hierarchyState->objectSpace->hpose_base[index];

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

		// NOT copied from class. THIS COULD BE WRONG
		a3ui32 i = firstIndex;
		for (i; i < nodeCount; ++i)
		{
			if (hierarchyState->hierarchy->nodes[i].parentIndex < 0) 
			{
				a3kinematicsSolveInverseRoot(hierarchyState, hierarchyState->hierarchy->nodes[i].index);
			}
			else 
			{
				// we are not the root
				a3kinematicsSolveInverseSingle(hierarchyState,
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

// Helper to resolve single-joint IK after solver
// This is done at the end of a3kinematicsUpdateLookAtIK
// Similar perhaps to updateHierarchyStateFK and updateHierarchyStateSkin 
static void a3kinematicsResolvePostIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const nodeIndex, a3real4x4 const j2obj /*joint to object*/)
{
	// post-IK resolutiaon for single affected joint
	//	-> reassign resolved transform to object-space
	//	-> compute object-space inverse matrix
	//	-> compute local-space matrix
	//	-> restore local-space matrix to pose
	//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
	//a3vec4 pWorldAffected = poseGroup->hpose->hpose_base[nodeIndex].transformMat.v3;

	// for joe <3
	// how are you doing? i am well
	// there is a function that is eerily similar to this
	// like the last 2 lines i think
	// just like change it
	// apparently this is like 4 lines

	//a3vec4 pWorldAffected = poseGroup->hpose->hpose_base[nodeIndex].transformMat.v3;

	//	-> reassign resolved transform to object-space
	a3real4x4SetReal4x4(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj);
	//	-> compute object-space inverse matrix
	a3real4x4TransformInverse(activeHS->objectSpaceInv->hpose_base[nodeIndex].transformMat.m, activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m);
	//	-> compute local-space matrix
	a3kinematicsSolveInverseSingle(activeHS, nodeIndex, activeHS->hierarchy->nodes[nodeIndex].parentIndex);
	//	-> restore local-space matrix to pose
	a3spatialPoseRestore(
		activeHS->localSpace->hpose_base + nodeIndex,
		poseGroup->channel[nodeIndex],
		poseGroup->order[nodeIndex]
	);
	//	-> deconcatenate base pose
	a3spatialPoseDeconcat(
		activeHS->animPose->hpose_base + nodeIndex,
		activeHS->localSpace->hpose_base + nodeIndex,
		baseHS->localSpace->hpose_base + nodeIndex
	);

	//a3real4x4Product(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj);

	//a3real4x4 m;
	//a3real4x4SetReal4x4(m, j2obj);
	//a3real4x4Invert(m);
	//a3real4x4SetReal4x4(activeHS->localSpace->hpose_base[nodeIndex].transformMat.m, j2obj);
	//a3real4x4SetReal4x4(activeHS->localSpaceInv->hpose_base[nodeIndex].transformMat.m, m);

	//a3kinematicsSolveInversePartial(activeHS, nodeIndex, activeHS->hierarchy->numNodes);

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

	// transform basis helper utility into respective matrices
	a3mat4 m_hierarchyObj, m_affected; // we use this for our calculations
	if (!a3basisToMat4(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat4(m_affected.m, basis_affected))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;


//	a3mat4 B_affected_inv;
//	a3real4x4GetInverse(B_affected_inv.m, m_affected.m);
//	a3mat4 B_hierarchyObj_inv;
//	a3real4x4GetInverse(B_hierarchyObj_inv.m, m_hierarchyObj.m);
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	//a3real3x3 lookAt;
	a3mat4 joint2object;

	// FIRST STEP: Put everything in a common space
	a3real4x4* rig2hierarchy = &sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat.m;
	a3vec4 hierarchy_affected; // eye of the 
	a3vec4 hierarchy_effector; // tiger
	
	hierarchy_affected = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3;
	a3vec4 rig_effector = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector].transformMat.v3;
	a3real4ProductTransform(hierarchy_effector.v, rig_effector.v, *rig2hierarchy);

	// SECOND STEP: Create the lookAt matrix
	a3vec3 worldUp = { 0, 1, 0 }; 

	// use only vector3s for this, cross product can be finnicky with vector4s
	// dont use make look at, we need to construct the bases manually

	a3vec3 directionBasis;
	a3vec3 sideBasis = { 0, 0, 1};
	a3vec3 upBasis = { 0, 1, 0 };

	// get the difference (effector - affected)
	a3real3Diff(directionBasis.v, hierarchy_effector.v, hierarchy_affected.v);
	a3real3Normalize(directionBasis.v);

	a3real3CrossUnit(sideBasis.v, directionBasis.v, worldUp.v);
	a3real3Cross(upBasis.v, directionBasis.v, sideBasis.v); 	// swapping this

	// normalize all the bases
	a3real3Normalize(upBasis.v);
	a3real3Normalize(directionBasis.v);

	// then we set the matrix
	a3real4Set(joint2object.v0.v, sideBasis.x, sideBasis.y, sideBasis.z, 0);
	a3real4Set(joint2object.v1.v, upBasis.x, upBasis.y, upBasis.z, 0);
	a3real4Set(joint2object.v2.v, directionBasis.x, directionBasis.y, directionBasis.z, 0);
	a3real4Set(joint2object.v3.v, hierarchy_affected.x, hierarchy_affected.y, hierarchy_affected.z, 1);
	//a3real4x4Set(joint2object,
	//	sideBasis.x, sideBasis.y, sideBasis.z, 0,
	//	upBasis.x, upBasis.y, upBasis.z, 0,
	//	directionBasis.x, directionBasis.y, directionBasis.z, 0,
	//	hierarchy_affected.x, hierarchy_affected.y, hierarchy_affected.z, 1);

	// add the translation back to the joint2ob

	// do not trust
//	a3real4x4MakeLookAt(joint2object, 0, hierarchy_affected.v3.v, hierarchy_effector.v3.v, worldUp);

	// LAST STEP: resolve every affected joint:
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, joint2object.m);

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
	//a3mat4 rig2hierarchy = sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;

	//a3vec4 pEffectorWorld = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.v3;
	//a3vec4 pPoleWorld = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_constraint].transformMat.v3;

	//a3vec4 pTarget, pPole;
	//a3real4TransformProduct(pTarget.v, rig2hierarchy.m, pEffectorWorld.v);
	//a3real4TransformProduct(pPole.v, rig2hierarchy.m, pPoleWorld.v);

	//a3vec4 pBase = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat.v3;
	//a3vec4 pHinge = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_hinge].transformMat.v3;
	//a3vec4 pEnd = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_end].transformMat.v3;

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
