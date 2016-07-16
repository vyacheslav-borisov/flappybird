#ifndef PEGAS_GFX_ATLAS_H_
#define PEGAS_GFX_ATLAS_H_

#include "../core/types.h"
#include "texture.h"
#include "sprite.h"

namespace pegas
{
	class Atlas
	{
	public:
		Atlas(android_app* context, const std::string& path);

		status load();
		void unload();

		SpritePtr getSprite(const std::string& name);
		ProcessPtr getAnimation(const std::string& name);
	private:
		typedef std::map<std::string, SpritePtr> SpriteMap;
		typedef SpriteMap::iterator SpriteMapIt;
		typedef std::map<std::string, ProcessPtr> AnimationMap;
		typedef AnimationMap::iterator AnimationMapIt;

		android_app* m_context;
		std::string  m_path;
		TexturePtr 	 m_texture;
		SpriteMap	 m_sprites;
		AnimationMap m_animations;
	private:
		void parseCoords(Sprite::Frame& frame, const std::string& coords, bool pixels);

	private:
		Atlas(const Atlas& other);
		Atlas& operator=(const Atlas& other);
	};
}

#endif /* GFX_ATLAS_H_ */
