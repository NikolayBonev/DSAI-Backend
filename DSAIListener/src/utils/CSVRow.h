#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

/* Class, used to parse a single row from a *.csv(comma-separated values) file */
class CSVRow
{
private:
	/* Holds the current row */
	std::vector<std::string> m_vecRowData;

	/* The current Row index*/
	size_t m_nRow = 0;
public:
	/* Return the token at cell nIndex */
	const std::string& operator[]( std::size_t nIndex) const;

	/* Return the number of tokens for the current row*/
	const size_t Size();

	/* Parse the next row of the *.csv file to this CSVRow */
	void ReadNextRow(std::istream& iStream);

	/* Overloaded input operator, used to parse a line
	@param iStream - input stream object, used to read from files 
	@csvRow - stores a row from a *.csv file */
	friend std::istream& operator>>(std::istream& iStream, CSVRow& csvRow);

	/* Set a value to m_nRow */
	void SetRow(size_t nRow);

	/* Get m_nRow*/
	size_t GetRow() const;
};
