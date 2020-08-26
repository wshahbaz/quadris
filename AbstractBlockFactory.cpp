#include "AbstractBlockFactory.h"
#include "IBlock.h"
#include "JBlock.h"
#include "LBlock.h"
#include "OBlock.h"
#include "SBlock.h"
#include "ZBlock.h"
#include "TBlock.h"
#include "StarBlock.h"
#include "HintBlock.h"
#include "VBlock.h"
#include "XBlock.h"
#include "WBlock.h"
#include <memory>

using namespace std;

AbstractBlockFactory BlockFactorySingleton::_factory;

AbstractBlockFactory::AbstractBlockFactory()
{
	factories_["IBlock"] = shared_ptr<IBlockFactory>(new IBlockFactory());
	factories_["JBlock"] = shared_ptr<JBlockFactory>(new JBlockFactory());
	factories_["LBlock"] = shared_ptr<LBlockFactory>(new LBlockFactory());
	factories_["OBlock"] = shared_ptr<OBlockFactory>(new OBlockFactory());
	factories_["SBlock"] = shared_ptr<SBlockFactory>(new SBlockFactory());
	factories_["ZBlock"] = shared_ptr<ZBlockFactory>(new ZBlockFactory());  
	factories_["TBlock"] = shared_ptr<TBlockFactory>(new TBlockFactory());
	factories_["StarBlock"] = shared_ptr<StarBlockFactory>(new StarBlockFactory());
	factories_["HintBlock"] = shared_ptr<HintBlockFactory>(new HintBlockFactory());
	factories_["VBlock"] = shared_ptr<VBlockFactory>(new VBlockFactory());
	factories_["XBlock"] = shared_ptr<XBlockFactory>(new XBlockFactory());
	factories_["WBlock"] = shared_ptr<WBlockFactory>(new WBlockFactory());
}

AbstractBlockFactory::~AbstractBlockFactory() {}

shared_ptr<Block> AbstractBlockFactory::createBlock(string blockType, int levelNum)
{
	map<string, shared_ptr<BlockFactory>>::iterator it = factories_.find(blockType);
	if (it == factories_.end())
	{
		return NULL;
	}
	return (*it).second->createBlock(levelNum);
}

//Retrieves the static instance of AbstractBlockFactory.
AbstractBlockFactory& BlockFactorySingleton::getInstance()
{
	return _factory;
}
