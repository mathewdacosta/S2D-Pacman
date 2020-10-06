// file:	SoundEffect.h
//
// summary:	Declares a SoundEffect class and SoundEffectStates
//
// author: Paul Boocock, Staffordshire University

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#endif

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "ALURE32.lib")

#include "S2D.h"

#include "AL\al.h"
#include "AL\alc.h"
#include "AL\alure.h"

namespace S2D
{
	/// <summary>	Allows checking of the current state of a sound effect. </summary>
	enum class S2D_API SoundEffectState
	{
		STOPPED,
		PLAYING,
		PAUSED,
		UNKNOWN
	};

	/// <summary>	Loads and represents a sound effect. </summary>
	class S2D_API SoundEffect
	{
	public:
		/// <summary>	Creates a standard sound effect. </summary>
		SoundEffect();

		/// <summary>	Creates a custom sound effect. </summary>
		///
		/// <param name="looping">	true to looping. </param>
		/// <param name="pitch">  	The pitch. </param>
		/// <param name="gain">   	The gain. </param>
		SoundEffect(bool looping, float pitch, float gain);

		/// <summary>	Destructor. </summary>
		~SoundEffect();

		/// <summary>	Loads the given sound file. </summary>
		///
		/// <param name="soundFileName">	The sound file name to load. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool Load(const char* soundFileName);

		/// <summary>	Query if this SoundEffect is loaded. </summary>
		///
		/// <returns>	true if loaded, false if not. </returns>
		bool IsLoaded();

		/// <summary>	Query if this SoundEffect is looping. </summary>
		///
		/// <returns>	true if looping, false if not. </returns>
		bool IsLooping();

		/// <summary>	Gets the pitch. </summary>
		///
		/// <returns>	The pitch. </returns>
		float GetPitch();

		/// <summary>	Gets the gain. </summary>
		///
		/// <returns>	The gain. </returns>
		float GetGain();

		/// <summary>	Gets the source. </summary>
		///
		/// <returns>	The source. </returns>
		ALuint GetSource();

		/// <summary>	Gets the state. </summary>
		///
		/// <returns>	The state. </returns>
		SoundEffectState GetState();

		/// <summary>	Sets if looping. </summary>
		///
		/// <param name="loop">	true to loop. </param>
		void SetLooping(bool loop);

		/// <summary>	Sets the pitch. </summary>
		///
		/// <param name="pitch">	The pitch. </param>
		void SetPitch(float pitch);

		/// <summary>	Sets the gain. </summary>
		///
		/// <param name="gain">	The gain. </param>
		void SetGain(float gain);

		/// <summary>	Offsets the given sound effect. </summary>
		///
		/// <param name="offset">	The offset. </param>
		void Offset(float offset);

	private:
		bool _isLoaded;
		bool _isLooping;

		float _pitch;
		float _gain;

		ALuint _source;
		ALuint _bufferID;                                                          //Stores the sound data
	};
}