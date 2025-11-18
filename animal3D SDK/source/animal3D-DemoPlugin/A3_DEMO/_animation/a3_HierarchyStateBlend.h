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
	
	a3_HierarchyStateBlend.h
	Hierarchy blend operations.
*/

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H


#include "a3_HierarchyState.h"

#include "a3_Kinematics.h"


#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// operation template for any real vector/matrix/array type
// The ... is filled in by the executor
typedef a3real* (*a3realOp)(a3real v_out[], ...);

// forward declare blend operation type
typedef struct a3_BlendOp a3_BlendOp;

// operation template for executing an operation
/*
	Function pointer
	Functions are just kept as addresses in memory to code. You create a call stack and put the local variables in before jumping every time you call a function

	The executor is a function that returns a pointer to a blendOp, and receieves a pointer to a blendOp
	Any function that matches this format: returns a blendOp pointer and takes ablendOp pointer, can be represented by this data type
	The blendOp itself has some data aray and ...!!
	The executor's job is to fill in the blanks!
	The op itself the blendOp wants some arbitrary data 

	There are some examples of initializing the source for this
	You don't have to use the HierarchyStateBlend ops in general
	They have a varying number of outputs and shit
	The executor's job is to invoke each of these function in this way

	It returns a blendOp pointer and takes a blendOp pointer and it calls a function that begins with an output array and
	I am concat I am negate I am concat I am negate

	a3blndOpExecute has no concrete definition

	2 stages to this whole thing, but we've been concerned with the build stage
	building happens once at load
	You read all that data that your animator provided and then build
	But execution stage
	March through the hierarchy and multiply a bunch of times
	There's pre-traversal and post-traversal. FK is parent first before child, this is a post-traversal algorithm. The children need to be resolved before the parent


*/
typedef a3_BlendOp const* (*a3blendOpExecute)(a3_BlendOp const* const blendOp);

// limits
enum
{
	a3blendOpLimitControl = 16,
	a3blendOpLimitInput = 8,
};

// replicable data structure for vector operations
typedef struct a3_BlendOp
{
	/// <summary>
	/// This system is exactly how Blueprint works as well. 
	/// It's just a dependency tree: any value that depends on another valu.
	/// Think of a blend node as a box, containing either literal stuff or knowledge of other stuff.
	/// Knowledge of other stuff would be using pointers...
	/// 
	/// exec(utor): a tool, a thing, a process, that knows how to execute an operation
	/// its job is to call a function *correctly*
	/// it is also a function, whose job is calling another function
	/// 
	/// op(eration): the function that the executor is trained to call
	/// could be lerp, concat, or negate,
	/// the executor knows how to call this kind of function
	/// 
	/// v_out: arbitrary pointer to a real value, the output value
	/// pointer to decouple the raw data from the thing that's operating on it
	/// 
	/// v_ctrl: controls of the operation
	/// u: any number of independent variables, parameters, input values, whatever you want to call them
	/// They're constant real pointers, why is that?
	/// So you can plug other things into it without duplicating values. Maybe other nodes need to use that value
	/// as a control. 
	///
	///	vCount: how many controls
	/// uCount: how many inputs
	/// like concat would have 2 controls 0 inputs
	/// and identity would have 0 controls 0 inputs
	///
	/// Why const pointers? "a3real const* ..."
	/// The value (a3real) keyword is affected by the const, since it goes to the left
	/// It ensures that the blend node can't start editing the output of a different node, all it can do is consume!
	/// ...A word about const: you should adopt the style of putting it on the very right of the thing you're affecting 
	/// because it has an ambiguous exception when you put it on the far left. 
	/// When you put it on the leftmost side it defers to affecting the next word (the word to the right of it)
	/// The qualifier is meant to go on the RIGHT, not the LEFT
	///
	/// Instantiate the blendOps, one per node, and then a data pool, and then the build process is wiring the nodes and their pointers together
	/// It's like connecting a complex electronic device
	/// If you want unique data for every instance of a parameter or control you would need one of these for each instance
	/// If you have a blend tree to describe multiple behaviors, you only need one hierarchy for those behaviors. Because that's just a guideline for how to build that one tree
	/// The description (hierarchy) is separate from the embodiment (tree).
	/// </summary>
	a3blendOpExecute exec;							// execution function

	a3realOp op;									// blend operation function
	a3real* v_out;									// output value
	a3real const* v_ctrl[a3blendOpLimitControl];	// control value set (maxed at 16 to pre-allocate array)
	a3real const* u[a3blendOpLimitInput];			// input value set
	a3ui16 vCount, uCount;							// control and input counts
} a3_BlendOp;


// set of operations for spatial pose
typedef struct a3_BlendOpSet
{
	a3blendOpExecute exec;
	a3realOp op_transformMat;
	a3realOp op_transformDQ;
	a3realOp op_rotate;
	a3realOp op_scale;
	a3realOp op_translate;
	a3realOp op_user;
} a3_BlendOpSet;

// init
a3_BlendOp* a3blendOpInit(a3_BlendOp* const blendOp, a3ui16 const controlCount, a3ui16 const inputCount, a3realOp const op, a3real* const v_out, a3real const* const v[a3blendOpLimitControl], a3real4 const* const u[a3blendOpLimitInput]);
a3_BlendOpSet* a3blendOpSetInit(a3_BlendOpSet* const blendOpSet, a3ui16 const controlCount, a3ui16 const inputCount, a3realOp const op_transformMat, a3realOp const op_transformDQ, a3realOp const op_rotate, a3realOp const op_scale, a3realOp const op_translate, a3realOp const op_user);

// execute
a3_BlendOp const* a3blendOpExecute0C(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute1C(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute2C(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute1C1I(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute2C1I(a3_BlendOp const* const blendOp);

// operation examples (follow templates above)
a3real4r a3blendOpRET4(a3real4 v_out);
a3real4r a3blendOpZERO4(a3real4 v_out);
a3real4r a3blendOpONE4(a3real4 v_out);
a3real4r a3blendOpID4(a3real4 v_out);
a3real4r a3blendOpCOPY4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpNEGATE4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpRECIP4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpCONJQ4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpADD4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpSUB4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpMUL4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpDIV4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpMULQ4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpMULCONJQ4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpSCALE4(a3real4 v_out, a3real4 const v, a3real const u);
a3real4r a3blendOpPOW4(a3real4 v_out, a3real4 const v, a3real const u);
a3real4r a3blendOpNEAR4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);
a3real4r a3blendOpLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);
a3real4r a3blendOpNLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);
a3real4r a3blendOpSLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);

a3real4r a3blendOpRET4X2(a3real4 m_out);
a3real4r a3blendOpID4X2(a3real4 m_out);
a3real4r a3blendOpCOPY4X2(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpCONJDQ4X2(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpMULDQ4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpMULCONJDQ4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpSCALE4X2(a3real4 m_out, a3real4 const m, a3real const u);
a3real4r a3blendOpNEAR4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpNLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpSCLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);

a3real4r a3blendOpRET4X4(a3real4 m_out);
a3real4r a3blendOpID4X4(a3real4 m_out);
a3real4r a3blendOpCOPY4X4(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpINVR4X4(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpMULM4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpMULINVR4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpSCALE4X4(a3real4 m_out, a3real4 const m, a3real const u);
a3real4r a3blendOpNEAR4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpLERP4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);


//-----------------------------------------------------------------------------

typedef struct a3_SpatialPoseBlendNode
{
	const a3_BlendOpSet* blendOpSet;
	a3_SpatialPose* pose_out;
	a3_SpatialPose const* pose_ctrl[a3blendOpLimitControl];
	a3real const* u[a3blendOpLimitInput];
	a3ui16 vCount, uCount;
} a3_SpatialPoseBlendNode;

typedef struct a3_SpatialPoseBlendTree
{
	a3_Hierarchy const* blendTreeDescriptor;

	a3_SpatialPoseBlendNode* nodes;
} a3_SpatialPoseBlendTree;


//-----------------------------------------------------------------------------

// create blend tree
a3ret a3spatialPoseBlendTreeCreate(a3_SpatialPoseBlendTree* blendTree, a3_Hierarchy const* blendTreeDescriptor);

// release blend tree
a3ret a3spatialPoseBlendTreeRelease(a3_SpatialPoseBlendTree* blendTree);

// configure node internally; set pointers
a3ret a3spatialPoseBlendTreeConfigureNode(a3_SpatialPoseBlendTree const* blendTree, a3ui32 const nodeIndex);

// execute tree from leaves to root
a3ret a3spatialPoseBlendTreeExecute(a3_SpatialPoseBlendTree const* blendTree);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out);

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H