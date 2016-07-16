#ifndef PEGAS_PHYSICS_CELL_GRID_H
#define PEGAS_PHYSICS_CELL_GRID_H
#pragma once

#include "../core/types.h"

//----------------------------------------------------------------------------
//	¬спомогательные классы дл€ минимизации числа проверок пересечени€
//	игровых объектов
//-----------------------------------------------------------------------------

namespace pegas
{

	template<class T>
	class CellGrid;


	//	ячейка. ѕредоставл€ет доступ к объектам, занимающим данную €чейку,
	//	а также к соседним €чейкам.
	template<class T>
	class Cell
	{
	public:
		typedef typename std::set<T> ObjectList;
		typedef typename ObjectList::iterator ObjectListIt;

		//получить указатель на соседнюю €чейку, в качестве параметра передаетьс€
		//число от 0 до 7 - индекс €чейки. ячейки выдаютьс€ в пор€дке по часовой стрелке,
		//начина€ той, что находитьс€ на одну строку выше данной и в том же столбце
		//если соседн€€ €чейка отсуствует (например дл€ €чеек по границам сетки) - выдает 0,
		//результат следует провер€ть на ноль. 
		Cell<T>* getSibling(int32 index);

		//получить итератор указывающий на первый обьект в €чейке
		ObjectListIt begin();
		ObjectListIt end();
		
	private:
		friend class CellGrid<T>; 
		//эти функции вызваютьс€ классом CellGrid

		//инициализаци€ €чейки еЄ номером строки и столбца
		void init(CellGrid<T>* owner, int32 row, int32 column);
		//поместить обьект в €чейку
		void addObject(const T& object);
		//удалить объект из €чейки
		void removeObject(const T& object);	

	private:
		CellGrid<T>*	m_owner;
		ObjectList		m_objects;
		int32			m_row;
		int32			m_column;
	};


	//—обственно, сетка
	template<class T>
	class CellGrid
	{
	public:
		CellGrid(): m_cells(0) {}


		//	создание сетки. в качестве параметров передаютьс€ ширина и высота
		//	(по размеру игрового уровн€) и размер €чейки (по размеру самого крупного обьекта
		//	в игре)

		void create(float gridWidth, float gridHeight, float cellLength);
		void destroy();


		//	поместить игровой обьект в €чейку. если обьект уже расположен в сетке, он перемещаетьс€
		//	в другую €чейку. »гровые обьекты вызвают эту функцию каждый раз, когда их позици€ мен€етьс€
		void placeToGrid(const Vector3& position, const T& obj);

		//  удалить обьект из €чейки. игровые обьекты вызвают эту функцию в конце при уничтожении обьекта
		void removeObject(const T& obj);

		//  получить €чейку дл€ данной точки
		Cell<T>* getCell(const Vector3& position);
		Cell<T>* getCell(int32 row, int32 column);

	private:
		Cell<T>** m_cells;
		int32    m_numRows;
		int32    m_numColumns;
		float     m_width;
		float     m_height;
		float	  m_halfWidth;
		float     m_halfHeight;
		float     m_cellLength;

		typedef typename std::map<T, Cell<T>*>  CellsLookupMap;
		CellsLookupMap m_lookupMap;

	private:
		CellGrid(const CellGrid& src);
		CellGrid& operator=(const CellGrid& src);
	};

	/*******************************************************************************************
		Cell class functions
	*******************************************************************************************/
	template<class T>
	inline Cell<T>* Cell<T>::getSibling(int32 index)
	{
		switch(index)
		{
		case 0:
			return m_owner->getCell(m_row + 1, m_column);
		case 1:
			return m_owner->getCell(m_row + 1, m_column + 1);
		case 2:
			return m_owner->getCell(m_row, m_column + 1);
		case 3:
			return m_owner->getCell(m_row - 1, m_column + 1);
		case 4:
			return m_owner->getCell(m_row - 1, m_column);
		case 5:
			return m_owner->getCell(m_row - 1, m_column - 1);
		case 6:
			return m_owner->getCell(m_row, m_column - 1);
		case 7:
			return m_owner->getCell(m_row + 1, m_column - 1);
		case 8:
			return this;
		default:
			break;
		}

		return 0;
	};

	template<class T>
	inline typename Cell<T>::ObjectListIt Cell<T>::begin()
	{
		return m_objects.begin(); 
	};

	template<class T>
	inline typename Cell<T>::ObjectListIt Cell<T>::end()
	{
		return m_objects.end();
	};

	template<class T>
	inline void Cell<T>::init(CellGrid<T>* owner, int32 row, int32 column)
	{
		m_owner = owner;
		m_row = row;
		m_column = column;
	};

	template<class T>
	inline void Cell<T>::addObject(const T& object)
	{
		if(m_objects.count(object) == 0)
			m_objects.insert(object);
	};

	template<class T>
	inline void Cell<T>::removeObject(const T& object)
	{
		if(m_objects.count(object) > 0)
			m_objects.erase(object);
	};

	//-----------------------------------------------------------------------------------
	//	CellGrid class implementation
	//------------------------------------------------------------------------------------
	template<class T>
	inline void CellGrid<T>::create(float gridWidth, float gridHeight, float cellLength)
	{
		assert(gridWidth > 0 && "invalid argument");
		assert(gridHeight > 0 && "invalid argument");
		assert(cellLength > 0 && "invalid argument");

		m_width = gridWidth;
		m_height = gridHeight;
		m_halfWidth = m_width / 2;
		m_halfHeight = m_height / 2;

		m_cellLength = cellLength;
		m_numRows = ceil(gridWidth / cellLength);
		m_numColumns = ceil(gridHeight / cellLength);

		m_cells = new Cell<T>*[m_numRows];
		for(int32 row = 0; row < m_numRows; row++)
		{
			m_cells[row] = new Cell<T>[m_numColumns];
			for(int32 column = 0; column < m_numColumns; column++)
			{
				m_cells[row][column].init(this, row, column);
			}
		}
	};

	template<class T>
	inline void CellGrid<T>::destroy()
	{
		if(m_cells)
		{
			for(int32 row = 0; row < m_numRows; row++)
			{
				delete[] m_cells[row];		
			}

			delete[] m_cells;
			m_cells = 0;
		}

		m_lookupMap.clear();
	};

	template<class T>
	inline void CellGrid<T>::placeToGrid(const Vector3& position, const T& obj)
	{
		removeObject(obj);

		Cell<T>* cell = getCell(position);
		if(cell)
		{
			cell->addObject(obj);
			m_lookupMap[obj] = cell;
		}
	};

	template<class T>
	inline void CellGrid<T>::removeObject(const T& obj)
	{
		if(m_lookupMap.count(obj) > 0)
		{
			m_lookupMap[obj]->removeObject(obj);
			m_lookupMap.erase(obj);
		}
	};

	template<class T>
	inline Cell<T>* CellGrid<T>::getCell(const Vector3& position)
	{
		int32 row = floor(((position._x + m_halfWidth) / m_width) * m_numRows);
		int32 column = floor(((position._y + m_halfHeight) / m_height) * m_numColumns);

		return getCell(row, column);
	};

	template<class T>
	inline Cell<T>* CellGrid<T>::getCell(int32 row, int32 column)
	{
		if(row >= 0 && row < m_numRows && column >= 0 && column <  m_numColumns)
		{
			return &m_cells[row][column];
		}
		return 0;
	};

}

#endif
