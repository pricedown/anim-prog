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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------

		// 1. increment time
		if (dt == 0)
			return 0;

		clipCtrl->clipTime_sec += dt;

		// 2. resolve current keyframe
		while (true) 
		{
			a3boolean isControllerBehindKeyframe = clipCtrl->clipTime_sec >= clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex1].time_sec;
			a3boolean isControllerAheadKeyframe = clipCtrl->clipTime_sec < clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex0].time_sec;
			if (!isControllerAheadKeyframe && !isControllerBehindKeyframe)
				break;

			if (isControllerBehindKeyframe) 
			{
				//if (clipCtrl->keyframeIndex == clipCtrl->clipPool->keyframeCount - 2) 
				//{
				//	clipCtrl->keyframeIndex++;
				//	clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
				//	clipCtrl->clipPool->keyframe->sampleIndex1 = clipCtrl->clipPool->keyframe->sampleIndex0;
				//	continue;
				//}
				if (clipCtrl->keyframeIndex == clipCtrl->clipPool->keyframeCount - 1)
				{
					// terminated at the end of the clip
					clipCtrl->clipTime_sec -= clipCtrl->clip->duration_sec; 
					clipCtrl->keyframeIndex = 0; 
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					continue; // return to catch up if behind
				}

				clipCtrl->keyframeIndex++;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
			}
			else if (isControllerAheadKeyframe) 
			{
				if (clipCtrl->keyframeIndex == 0)
				{
					// terminated at the beginning of the clip
					clipCtrl->clipTime_sec += clipCtrl->clip->duration_sec;
					clipCtrl->keyframeIndex = clipCtrl->clipPool->keyframeCount - 1;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					continue; // return to catch up if ahead
				}

				clipCtrl->keyframeIndex--;
				clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
			}
		}

		clipCtrl->keyframeParam = (clipCtrl->clipTime_sec - clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex0].time_sec) * clipCtrl->keyframe->durationInv;
		clipCtrl->clipParam = (clipCtrl->clipTime_sec) / (clipCtrl->clip->duration_sec);
		return 0;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
