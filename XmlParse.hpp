#pragma once
#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <vector>
namespace Dinzai
{
	enum
	{
		Fails,
		Multiple,
		Single
	};

	enum LoadValue
	{
		Fail,
		Successful,
		FileNotFound
	};

	class Node
	{
		struct XMLAtribute
		{
			std::string key;

			std::string value;
		};

	public:
		std::string tag;
		std::string innerText;
		class Node* parent;
		std::vector<XMLAtribute> AtributesList;
		std::vector<Node*> children;



		std::string findKeyValues(const char* Key)
		{
			for (XMLAtribute& attr : AtributesList)
				if (attr.key == Key) { return attr.value; break; }

			return 0;
		}

		int findAttribute(std::string Buffer, unsigned& position)
		{
			XMLAtribute NewAttribute;
			std::string search;
			position++;
			while (Buffer[position] != '>')
			{
				if (Buffer[position] != ' ' && Buffer[position] != '"')
				{
					search.push_back(Buffer[position]);
				}
				position++;
				if (Buffer[position] == ' ' && tag.empty())
				{
					tag = search;
					position++;
					search.clear();
				}
				else if (Buffer[position + 1] == '>' && Buffer[position] != '?' && tag.empty()) // Nodes with no keys
				{
					search.push_back(Buffer[position]);
					tag = search;	 // Add node tag
					position++;	 // The data ends on the next position
					search.clear(); // Clear vector for the next data input
				}
				if (Buffer[position] == '=')
				{
					NewAttribute.key = search;
					search.clear();
					position += 2;
					if (Buffer[position - 1] == '"')
					{
						while (Buffer[position] != '"')
						{
							search.push_back(Buffer[position++]);
						}
					}
					else
					{
						return false;
					}

					NewAttribute.value = search;
					search.clear();

					this->AtributesList.push_back(NewAttribute);
					NewAttribute.key.clear();
					NewAttribute.value.clear();
					continue;


				}
				/*
				if (Buffer[position] == '/' && Buffer[position + 1] == '>')
				{
					position++;
					return Single;
				}
				*/

				if (Buffer[position + 1] == '>')
				{
					position++;

					return Single;
				}


			}

			return Multiple;

		}

	};

	class XMLDocument
	{


		Node* newChild(Node* parrent)
		{
			Node* child = new Node;
			child->parent = parrent;
			parrent->children.push_back(child);
			return child;
		}


	public:
		std::string version;
		std::string encoding;

		Node* Root;

		LoadValue load(const char* path)
		{
			std::ifstream file(path);
			if (!file.is_open())
			{
				std::cout << "The file is not found" << std::endl;
				return FileNotFound;
			}
			else
			{
				std::string Buffer((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());
				//std::cout << Buffer;

				unsigned position = 0;
				std::string search;
				unsigned length = Buffer.size();
				Root = new Node;

				while (position < length)
				{


					if (Buffer[position] == '<')
					{
						if (!search.empty())
						{
							if (search != " ")
							{
								Root->innerText.append(search + " ");
							}
							search.clear();
						}
						if (Buffer[position + 1] == '/')
						{

							position += 2;
							while (Buffer[position] != '>')
							{
								search.push_back(Buffer[position++]);
							}

							if (Root->tag != search)
							{
								std::cout << "Tag mismatched" << search << " " << Root->tag << "\n";
								return Fail;
							}
							Root = Root->parent;
							position++;
							search.clear();
							continue;
						}
						if (Buffer[position + 1] == '?')
						{
							while (Buffer[position] != ' ' && Buffer[position] != '>')
							{
								search.push_back(Buffer[position++]);
							}
							if (search == "<?xml")
							{
								Node Description;

								Description.findAttribute(Buffer, position);
								Root->AtributesList = Description.AtributesList;
								Root->tag = "xml";
								search.clear();
								position++;
								continue;
							}
						}
						Root = newChild(Root);
						if (Root->findAttribute(Buffer, position) == Single)
						{
							Root = Root->parent;
							position++;
							continue;
						}

						if (Root->tag.empty())
						{
							Root->tag = search;
							position++;
							continue;
						}

					}
					else
					{

						if (Buffer[position] != '\n' && Buffer[position != '>'])
						{
							search.push_back(Buffer[position]);

						}

						position++;


					}

				}

			}
			return Successful;
		}


	};

}
