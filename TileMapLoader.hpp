#pragma once
#include "XMLParse.hpp"
#include <SFML/Graphics.hpp>

namespace Dinzai
{
	struct tileset
	{
		std::string name;
		std::string image;
		int tileWidth;
		int tileHeight;
		tileset(Node* doc)
		{
			for (Node* node : doc->children)
			{
				if (node->tag == "image")
				{
					image = node->findKeyValues("source");

					break;
				}
			}
		}


	};


	struct tileLayer
	{
		int rows;
		int columns;
		int id;
		std::vector<int> lay;

		tileLayer(Node node)
		{
			columns = stoi(node.findKeyValues("width"));
			rows = stoi(node.findKeyValues("height"));
			id = stoi(node.findKeyValues("id"));


			std::string layer = node.children[0]->innerText;

			int i = 1;
			std::string number;

			while (layer[i] != '\0')
			{
				if (layer[i] == ',' || layer[i + 1] == '\0')
				{
					//std::cout << layer;
					lay.push_back(stoi(number));
					number.clear();

				}
				else
				{
					number = number + layer[i];
				}

				i++;


			}
		}

	};

	class TileMapFile
	{
		XMLDocument* Map = nullptr;

	public:

		TileMapFile()
		{
			Map = new XMLDocument;

		}

		std::vector<tileset> tileSets;
		std::vector<tileLayer> tileLayers;
		std::vector<Node> ObjectLayers;
		std::vector<Node> ImageLayers;
		int width;
		int height;

		bool load(std::string FOLDER, std::string FILE)
		{
			int result = Map->load((FOLDER + FILE).c_str());
			if (result == Successful)
			{
				for (Node* child : Map->Root->children[0]->children)
				{
					if (child->tag == "tileset")
					{
						XMLDocument newDoc; std::string newfile = child->findKeyValues("source");

						int ok = newDoc.load((FOLDER + newfile).c_str());
						if (ok == FileNotFound)
						{
							std::cout << FOLDER + newfile << " : Could not be Found ";
							return 0;
						}
						else if (ok == Successful)
						{
							std::cout << "Tileset " << newfile << " : has loaded succesfully \n";
							tileSets.push_back(tileset(newDoc.Root->children[0]));

						}
					}
					if (child->tag == "objectgroup")
					{
						ObjectLayers.push_back(*child);


					}
					if (child->tag == "imagelayer")
					{
						ImageLayers.push_back(*child);


					}
					if (child->tag == "layer")
					{
						tileLayers.push_back(tileLayer(*child));

					}

				}

				std::cout << "Tilemap " << FILE << " : has loaded successfully \n";

				width = stoi(Map->Root->children[0]->findKeyValues("tilewidth"));
				height = stoi(Map->Root->children[0]->findKeyValues("tileheight"));


			}
			else if (result == FileNotFound)
			{
				std::cout << FOLDER + FILE << " : Could not be found ";
				return 0;
			}

			delete(Map);


			return 1;

		}

	};

	class SFMLMap : public sf::Drawable
	{
		sf::Texture m_texture;
		sf::VertexArray m_verticies;

	public:
		SFMLMap(std::string FOLDER, std::string FILE)
		{
			TileMapFile doc;

			doc.load(FOLDER, FILE);

			m_texture.loadFromFile(FOLDER + doc.tileSets[0].image);
			int dis = m_texture.getSize().x / doc.width;
			m_verticies.setPrimitiveType(sf::Quads);
			m_verticies.resize(doc.tileLayers[0].columns * doc.tileLayers[0].rows * 4);

			for (int i = 0; i < doc.tileLayers[0].rows; i++)
			{
				for (int j = 0; j < doc.tileLayers[0].columns; j++)
				{
					int index = (j + i * doc.tileLayers[0].columns) * 4;
					int gid = doc.tileLayers[0].lay[j + i * doc.tileLayers[0].columns];

					sf::Vertex* vertex = &m_verticies[index];
					vertex[0].position = sf::Vector2f(doc.width * j, doc.height * i);
					vertex[1].position = sf::Vector2f((j + 1) * doc.width, i * doc.height);
					vertex[2].position = sf::Vector2f((j + 1) * doc.width, (i + 1) * doc.height);
					vertex[3].position = sf::Vector2f(j * doc.width, (i + 1) * doc.height);

					int colp = gid % dis - 1;
					int rowp = gid / dis;

					vertex[0].texCoords = sf::Vector2f(doc.width * colp, doc.height * rowp);
					vertex[1].texCoords = sf::Vector2f((colp + 1) * doc.width, rowp * doc.height);
					vertex[2].texCoords = sf::Vector2f((colp + 1) * doc.width, (rowp + 1) * doc.height);
					vertex[3].texCoords = sf::Vector2f(colp * doc.width, (rowp + 1) * doc.height);


				}
			}


		}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const
		{
			states.texture = &m_texture;
			target.draw(m_verticies, states);
		}
	};



}