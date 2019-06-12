#include "CSVIterator.h"


CSVIterator::CSVIterator(std::istream& str) :
	m_ptrStream(str.good() ? &str : nullptr)
{ 
	++(*this); 
}

CSVIterator::CSVIterator() :
	m_ptrStream(nullptr)
{
}

// Pre Increment
CSVIterator& CSVIterator::operator++()
{
	if (m_ptrStream != nullptr)
	{ 
		/* if there are no more lines to pass */
		if (!((*m_ptrStream) >> m_csvRow))
		{ 
			m_ptrStream = nullptr; 
		} 

		m_csvRow.SetRow(m_nCurrentRow++);
	}

	return *this; 
}


CSVIterator CSVIterator::operator++(int) 
{ 
	/*Construct an object, holding the current iterator */
	CSVIterator currentIterator(*this); 

	/* Increment the original iterator */
	++(*this); 

	m_csvRow.SetRow(m_nCurrentRow++);

	/* Return the current iterator */
	return currentIterator;
}

CSVRow const& CSVIterator::operator*()   const 
{ 
	/* Return the csv row by value */
	return m_csvRow; 
}
CSVRow const* CSVIterator::operator->()  const 
{ 
	/* Return the csv row by reference */
	return &m_csvRow;
}

bool CSVIterator::operator==(CSVIterator const& rhs) 
{ 
	return ((this == &rhs) || 
		((this->m_ptrStream == nullptr) && (rhs.m_ptrStream == nullptr)));
}

bool CSVIterator::operator!=(CSVIterator const& rhs)
{
	return !((*this) == rhs);
}

size_t CSVIterator::GetRow() const
{
	return m_nCurrentRow;
}
