#include "CSVRow.h"

const std::string & CSVRow::operator[](std::size_t nIndex) const
{
	return m_vecRowData.at(nIndex);
}

const size_t CSVRow::Size()
{
	return m_vecRowData.size();
}

void CSVRow::ReadNextRow(std::istream & iStream)
{
	if (!m_vecRowData.empty())
	{
		m_vecRowData.erase(m_vecRowData.begin(), m_vecRowData.end());
	}

	/* Represents the current line from the file */
	std::string strLine;

	/* Parse the current line to strLine */
	getline(iStream, strLine);

	/* Stream object, used to manipulate strLine */
	std::stringstream streamLine(strLine);
	
	/* Stores the value of the current cell*/
	std::string strCell;

	/* Separate the line by commas */
	while (getline(streamLine, strCell, ','))
	{
		m_vecRowData.push_back(strCell);
	}

	/* Check for a trailing comma with no data after it */
	if (!streamLine && strCell.empty())
	{
		/* If there was a trailing comma, then add an empty element*/
		m_vecRowData.push_back("");
	}
}

std::istream& operator>>(std::istream & iStream, CSVRow & csvRow)
{
	csvRow.ReadNextRow(iStream);
	return iStream;
}

void CSVRow::SetRow(size_t nRow)
{
	m_nRow = nRow;
}

size_t CSVRow::GetRow() const
{
	return m_nRow;
}