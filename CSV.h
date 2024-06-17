#ifndef CSV_H
#define CSV_H

#include <string>
#include <vector>

/**
 * @brief CSV Reader/Writer. Default delimiter is a comma. CSV format should have 
 * data labels for each column within the first row of the file.
 */
class CSV
{
    public:
        CSV();
        CSV(const std::string FILENAME);
        CSV(const std::string FILENAME, const char DELIM);

        void createTable(const std::string FILENAME);
        void createTable(const std::string FILENAME, const char DELIM);

        /**
         * @brief Changes the delimiter to DELIM and updates the associated file
         * 
         * @param DELIM The delimiter character
         */
        void setDelim(const char DELIM);
        void setItem(const std::string ITEM, const int ROW, const int COL);

        std::string getItem(const int ROW, const int COL);
        std::vector<std::string> getRow(const int ROW);
        std::vector<std::string> getCol(const int COL);

        /**
         * @brief Adds a row to the table and updates the associated file
         * 
         * @param ROW Vector of strings representing the row to be added
         */
        void pushRow(const std::vector<std::string> ROW);
        /**
         * @brief Adds a column to the table and updates the associated file
         * 
         * @param COL Vector of strings representing the column to be added. First index should be data label of the column
         */
        void pushCol(const std::vector<std::string> COL);

        /**
         * @brief Checks if a value is present within the table
         * 
         * @param ITEM Item to be searched for
         * @return true when item is found
         * @return false when item is not found
         */
        bool searchForItem(const std::string ITEM) const;
        bool errorCheck(const int ROW, const int COL) const;
    private:
        void _build(const char DELIM);
        void _updateFile();

        /**
         * @brief Vector of vector of strings representing CSV data. First row labels are NOT included
         * 
         */
        std::vector<std::vector<std::string>> _table;
        /**
         * @brief Vector of strings representing the data labels within the first row of the CSV
         * 
         */
        std::vector<std::string> _header;
        std::string _filename;
        /**
         * @brief The most recently specified delimiter character
         * 
         */
        char _delim;
        int _columns;
        int _rows;
};

#endif