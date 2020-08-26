#ifndef H_ABSTRACT_BLOCK_FACTORY
#define H_ABSTRACT_BLOCK_FACTORY
#include <map>
#include <memory>
#include "Block.h"

class BlockFactorySingleton;
class AbstractBlockFactory
{
	friend class BlockFactorySingleton;
	public:
		std::shared_ptr<Block> createBlock(std::string blockType, int levelNum);
		virtual ~AbstractBlockFactory();
		
	private:
		AbstractBlockFactory();
		std::map<std::string, std::shared_ptr<BlockFactory>> factories_;
};

class BlockFactorySingleton
{
	public:
		static AbstractBlockFactory& getInstance();

	private:
		static AbstractBlockFactory _factory;
		BlockFactorySingleton();
		BlockFactorySingleton (const BlockFactorySingleton&);
};

#define BFactory BlockFactorySingleton::getInstance()

#endif
