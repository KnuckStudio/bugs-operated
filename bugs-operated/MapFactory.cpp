#include "MapFactory.h"

#include "MapLoader.h"

MapFactory::MapFactory(const std::string & filename) :
	AbstractFactory(tag<Map>{}), m_data(nullptr),
	m_filename(filename)
{
}

void * MapFactory::load()
{
	if (m_data == nullptr) {
		std::unique_ptr<Map> map = std::make_unique<Map>();
		
		MapLoader mapLoader;
		if (!mapLoader.load(m_filename, *map)) {
			throw std::runtime_error("Unable to load map: \"" + m_filename + "\"");
		}

		m_data = std::move(map);
	}

	return m_data.get();
}

void MapFactory::clear()
{
	m_data.reset(nullptr);
}
