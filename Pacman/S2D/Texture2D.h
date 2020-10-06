// file:	Texture2D.h
//
// summary:	Declares a Texture2D class
//
// author: Paul Boocock, Staffordshire University

#pragma once

#ifdef S2D_EXPORTS
#define S2D_API __declspec(dllexport)
#else
#define S2D_API __declspec(dllimport)
#endif

#pragma comment(lib, "SOIL.lib")

#include "S2D.h"
#include "SOIL\SOIL.h"

namespace S2D
{
	/// <summary>	Allows for loading of texture data from a variety of file types. </summary>
	class S2D_API Texture2D
	{
	private:
		GLuint * _ID; //Texture ID
		int _width, _height;

	public:

		/// <summary>	Default constructor. </summary>
		Texture2D();

		/// <summary>	Destructor. </summary>
		~Texture2D();

		/// <summary>	Loads RAW Textures. </summary>
		///
		/// <param name="textureFileName">	Filename of the texture file. </param>
		/// <param name="width">		  	The width. </param>
		/// <param name="height">		  	The height. </param>
		/// <param name="buildMipMaps">   	true to build mip maps. </param>
		void LoadRAW(const char* textureFileName, int width, int height, bool buildMipMaps);

		/// <summary>	Loads Textures of most file formats. </summary>
		///
		/// <param name="textureFileName">	Filename of the texture file. </param>
		/// <param name="buildMipMaps">   	true to build mip maps. </param>
		void Load(const char* textureFileName, bool buildMipMaps);

		/// <summary>	Gets the texture identifier. </summary>
		///
		/// <returns>	null if it fails, else the identifier. </returns>
		GLuint * GetID() const;

		/// <summary>	Gets the width. </summary>
		///
		/// <returns>	The width. </returns>
		int GetWidth() const;

		/// <summary>	Gets the height. </summary>
		///
		/// <returns>	The height. </returns>
		int GetHeight() const;

		/// <summary>	Gets the texture data. </summary>
		void GetData(Color* data) const;
	};
}

