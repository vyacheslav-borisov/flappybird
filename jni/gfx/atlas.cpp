#include "../common.h"
#include "atlas.h"

#include "../system/log.h"
#include "../system/resource.h"

namespace rapidxml
{
	static jmp_buf jumpBuffer;
	void parse_error_handler(const char* what, void* where)
	{
		LOGE("Error while parsing atlas XML file.");
		LOGE(what);
		longjmp(jumpBuffer, 0);
	}
}

namespace pegas
{
		Atlas::Atlas(android_app* context, const std::string& path)
			:m_context(context), m_path(path)
		{
			LOGI("Atlas constructor");
		}

		status Atlas::load()
		{
			LOGI("Atlas::load");

			rapidxml::xml_document<> xmlDocument;
			rapidxml::xml_node<> *xmlNodeAtlas, *xmlNodeSprite, *xmlNodeAnimation, *xmlNodeFrame;
			rapidxml::xml_attribute<> *xmlAttrTexture, *xmlAttrCoordType;
			rapidxml::xml_attribute<> *xmlAttrName, *xmlAttrCoords;
			size_t xmlBufferLength = 0;
			char* xmlBuffer = NULL;
			void* scrBuffer = NULL;
			bool coordsIsPixels = false;
			std::string texturePath;

			LOGI("loading resource %s", m_path.c_str());
			AssetResource resource(m_context, m_path);
			if(resource.load() != STATUS_OK)
			{
				LOGE("resource.load() != STATUS_OK");
				goto ERROR;
			}

			xmlBufferLength = resource.getLength();
			if(xmlBufferLength <= 0)
			{
				LOGE("xmlBufferLength <= 0");
				goto ERROR;
			}

			scrBuffer = (char*)resource.bufferize();
			if(scrBuffer == NULL)
			{
				LOGE("scrBuffer == NULL");
				goto ERROR;
			}

			LOGI("creating xml buffer...");
			xmlBuffer = new char[xmlBufferLength + 1];

			LOGI("copy data to xml buffer...");
			memcpy(xmlBuffer, scrBuffer, xmlBufferLength);
			xmlBuffer[xmlBufferLength] = '\0';

			LOGI("unloading resource...");
			resource.unload();

			if(setjmp(rapidxml::jumpBuffer)) goto ERROR;

			LOGI("parsing xml data...");
			xmlDocument.parse<rapidxml::parse_default>(xmlBuffer);
			xmlNodeAtlas = xmlDocument.first_node("atlas");
			if(xmlNodeAtlas == NULL)
			{
				LOGE("invalid xml data: <atlas> node not found");
				goto ERROR;
			}

			xmlAttrTexture = xmlNodeAtlas->first_attribute("texture");
			if(xmlAttrTexture == NULL)
			{
				LOGE("invalid xml data: \"texture\" attribute not found");
				goto ERROR;
			}

			xmlAttrCoordType = xmlNodeAtlas->first_attribute("coordType");
			if(xmlAttrCoordType != NULL)
			{
				std::string strCoordType = xmlAttrCoordType->value();
				coordsIsPixels = (strCoordType == "pixels");
			}

			LOGI("loading atlas texture..");
			texturePath = xmlAttrTexture->value();
			m_texture = TexturePtr(new Texture(m_context, texturePath));
			if(m_texture->load() != STATUS_OK)
			{
				LOGE("m_texture->load() != STATUS_OK");
				goto ERROR;
			}

			LOGI("loading sprites...");
			xmlNodeSprite = xmlNodeAtlas->first_node("sprite");
			while(xmlNodeSprite != NULL)
			{
				xmlAttrName = xmlNodeSprite->first_attribute("name");
				if(xmlAttrName != NULL)
				{
					std::string name = xmlAttrName->value();

					LOGI("adding sprite: %s", name.c_str());

					SpritePtr sprite = SpritePtr(new Sprite(m_texture.get()));
					m_sprites[name] = sprite;

					xmlNodeFrame = xmlNodeSprite->first_node("frame");
					while(xmlNodeFrame != NULL)
					{
						xmlAttrCoords = xmlNodeFrame->first_attribute("coords");
						if(xmlAttrCoords != NULL)
						{
							Sprite::Frame frame;
							std::string coords = xmlAttrCoords->value();
							parseCoords(frame, coords, coordsIsPixels);

							LOGI("adding frame [u1: %0.2f, v1: %0.2f, u2: %0.2f, v2: %0.2f,]",
									frame._fromX, frame._fromY, frame._toX, frame._toY);
							sprite->addFrame(frame);
						}

						xmlNodeFrame = xmlNodeFrame->next_sibling("frame");
					}
				}

				xmlNodeSprite = xmlNodeSprite->next_sibling("sprite");
			}

			//loading animations

			if(xmlBuffer)
			{
				LOGI("deleting xml buffer...");
				delete[] xmlBuffer;
			}

			return STATUS_OK;

		ERROR:
			LOGI("unloading resource...");
			resource.unload();

			if(xmlBuffer)
			{
				LOGI("deleting xml buffer...");
				delete[] xmlBuffer;
			}

			return STATUS_KO;
		}

		void Atlas::unload()
		{
			LOGI("Atlas::unload");

			m_texture->unload();
			m_sprites.clear();
			m_animations.clear();
		}

		SpritePtr Atlas::getSprite(const std::string& name)
		{
			if(m_sprites.count(name) > 0)
			{
				return m_sprites[name];
			}

			return NULL;
		}

		ProcessPtr Atlas::getAnimation(const std::string& name)
		{
			if(m_animations.count(name) > 0)
			{
				return m_animations[name];
			}

			return NULL;
		}

		void Atlas::parseCoords(Sprite::Frame& frame, const std::string& coords, bool pixels)
		{
			std::istringstream iss(coords);
			std::string strFromX, strFromY, strToX, strToY;

			iss >> strFromX;
			iss >> strFromY;
			iss >> strToX;
			iss >> strToY;

			if(pixels)
			{
				int iFromX = atoi(strFromX.c_str());
				int iFromY = atoi(strFromY.c_str());
				int iToX = atoi(strToX.c_str());
				int iToY = atoi(strToY.c_str());

				int width = m_texture->getWidth();
				int height = m_texture->getHeight();
				if(width > 0 && height > 0)
				{
					frame._fromX = (iFromX * 1.0f / width);
					frame._fromY = 1.0 - (iFromY * 1.0f / height);
					frame._toX = (iToX * 1.0f / width);
					frame._toY = 1.0 - (iToY * 1.0f / height);
				}
			}else
			{
				frame._fromX = atof(strFromX.c_str());
				frame._fromY = atof(strFromY.c_str());
				frame._toX = atof(strToX.c_str());
				frame._toY = atof(strToY.c_str());
			}
		}
}



