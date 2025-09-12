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
	//-----------------------------------------------------------------------------
	//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
	//-----------------------------------------------------------------------------

	if (!clipCtrl || !clipCtrl->clipPool) // ensure the clip controller and clip pool exist
		return -1;

	if (dt == 0)
		return 1;

	// Increment time based on whether or not we are reversed
	if (clipCtrl->playback_reversed)
		clipCtrl->clipTime_sec -= dt;
	else
		clipCtrl->clipTime_sec += dt;


	// Resolve keyframe (and clip if transitioned)
	while (true)
	{
		// Continue if the controller's keyframe is ahead or behind the keyframe that it's supposed to be at
		a3boolean isControllerBehind = clipCtrl->clipTime_sec >= clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex1].time_sec;
		a3boolean isControllerAhead = clipCtrl->clipTime_sec < clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex0].time_sec;
		if (!isControllerAhead && !isControllerBehind)
			break;

		if (isControllerBehind)
		{
			// Step forward to catch up

			if (clipCtrl->keyframeIndex == clipCtrl->clipPool->keyframeCount - 1)
			{
				// Passed the final keyframe, forward transition!

				if (clipCtrl->clip->transitionForward->flag == a3clip_stopFlag)
				{
					// Stop
					clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec;
					break;
				}

				else if (clipCtrl->clip->transitionForward->flag == a3clip_playFlag)
				{
					// Play (loop)
					clipCtrl->clipTime_sec -= clipCtrl->clip->duration_sec;
					clipCtrl->keyframeIndex = 0;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					continue; // Continue to further resolve 
				}
				else if (clipCtrl->clip->transitionForward->flag == a3clip_reverseFlag)
				{
					// Reverse (Ping-Pong)
					clipCtrl->playback_reversed = !clipCtrl->playback_reversed;
					clipCtrl->clipTime_sec = 2 * clipCtrl->clip->duration_sec - clipCtrl->clipTime_sec; // Account for overstep and find the correct keyframe
					continue; // Continue to further resolve 
				}
			}

			// Increment the keyframe
			clipCtrl->keyframeIndex++;
			clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
		}

		else if (isControllerAhead)
		{
			// Step backward to catch up

			if (clipCtrl->keyframeIndex == 0)
			{
				// Passed behind the first keyframe, reverse transition!

				if (clipCtrl->clip->transitionReverse->flag == a3clip_stopFlag)
				{
					// Stop
					clipCtrl->clipTime_sec = 0.0f;
					break;
				}

				else if (clipCtrl->clip->transitionReverse->flag == a3clip_playFlag)
				{
					// Play (loop)
					clipCtrl->clipTime_sec += clipCtrl->clip->duration_sec;
					clipCtrl->keyframeIndex = clipCtrl->clipPool->keyframeCount - 1;
					clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
					continue; // Continue to further resolve 
				}

				else if (clipCtrl->clip->transitionReverse->flag == a3clip_reverseFlag)
				{
					// Reverse (Ping-Pong)
					clipCtrl->playback_reversed = !clipCtrl->playback_reversed;
					clipCtrl->clipTime_sec = -clipCtrl->clipTime_sec; // Account for overstep and find the correct keyframe
					continue; // Continue to further resolve 
				}
			}

			// Decrement the keyframe
			clipCtrl->keyframeIndex--;
			clipCtrl->keyframe = &clipCtrl->clipPool->keyframe[clipCtrl->keyframeIndex];
		}
	}


	// Calculate time since beginning of keyframe (keyframeTime)
	clipCtrl->keyframeTime_sec = (clipCtrl->clipTime_sec - clipCtrl->clipPool->sample[clipCtrl->keyframe->sampleIndex0].time_sec);
	// Calculate normalized position of controller within keyframe (keyframeParam)
	clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec * clipCtrl->keyframe->durationInv;
	// Calculate normalized position of controller within clip (clipParam)
	clipCtrl->clipParam = (clipCtrl->clipTime_sec) / (clipCtrl->clip->duration_sec);

	return 1;

	//-----------------------------------------------------------------------------
	//****END-TO-DO-PROJECT-1
	//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
