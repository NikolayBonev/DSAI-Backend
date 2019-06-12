#pragma once
#include "CSVRow.h"

/* Simple class, to iterate through a *.csv file row by row */
class CSVIterator
{
private:
	/* Stream object, used to read from the *.csv file */
	std::istream* m_ptrStream = nullptr;

	/* CSVRow object, used to store the current csv row */
	CSVRow m_csvRow;

	/* Index for the current row */
	size_t m_nCurrentRow = 0;
public:
	/* Construct an iterator by passing a stream object */
	CSVIterator(std::istream& str);

	/* Construct an iterator with an empty stream object*/
	CSVIterator();

	/* Pre-increment operator */
	CSVIterator& operator++();

	/* Post-increment operator */
	CSVIterator operator++(int);

	/* Dereference operator */
	CSVRow const& operator*()   const;

	/* Reference operator */
	CSVRow const* operator->()  const;

	/*Comparisson operators*/
	bool operator==(CSVIterator const& rhs);
	bool operator!=(CSVIterator const& rhs);

	size_t GetRow() const;
};