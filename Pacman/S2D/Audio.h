// file:	Audio.h
//
// summary:	Declares the Audio methods
//
// author: Paul Boocock, Staffordshire University
//
// remarks: The reason this is in namespaces, rather than a static class pattern: http://stackoverflow.com/questions/9321/how-do-you-create-a-static-class-in-c/112451#112451

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#endif

#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "OpenAL32.lib")

#include "S2D.h"
#include "SoundEffect.h"

using namespace std;

namespace S2D
{
	//Forward declaration of SoundEffect class
	//This enables the Audio to know about the SoundEffect class before it is compiled
	class SoundEffect;

	/// <summary>	Handles all the Audio Playback. </summary>
	namespace Audio
	{
		/// <summary>	Initialises the Audio. </summary>
		S2D_API void Initialise();

		/// <summary>	Destroys the Audio. </summary>
		S2D_API void Destroy();

		/// <summary>	Plays the given sound effect. </summary>
		///
		/// <param name="soundEffect">	[in,out] If non-null, the sound effect. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		S2D_API bool Play(SoundEffect* soundEffect);

		/// <summary>	Stops the given sound effect. </summary>
		///
		/// <param name="soundEffect">	[in,out] If non-null, the sound effect. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		S2D_API bool Stop(SoundEffect* soundEffect);

		/// <summary>	Pauses the given sound effect. </summary>
		///
		/// <param name="soundEffect">	[in,out] If non-null, the sound effect. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		S2D_API bool Pause(SoundEffect* soundEffect);

		/// <summary>	Resumes the given sound effect. </summary>
		///
		/// <param name="soundEffect">	[in,out] If non-null, the sound effect. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		S2D_API bool Resume(SoundEffect* soundEffect);

		/// <summary>	Query if Audio is initialised. </summary>
		///
		/// <returns>	true if initialised, false if not. </returns>
		S2D_API bool IsInitialised();
	}
}

