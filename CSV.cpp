#include "CSV.h"

#include <iostream>
#include <fstream>

CSV::CSV()
{
    _columns = 0;
    _rows = 0;
    _table = {{}}, _header = {};
    _filename = "";
    _delim = ',';
}

CSV::CSV(const std::string FILENAME)
{
    _columns = 0, _rows = 0;
    _filename = FILENAME;
    _delim = ',';
    _build(',');
}

CSV::CSV(const std::string FILENAME, const char DELIM)
{
    _columns = 0, _rows = 0;
    _filename = FILENAME;
    _delim = DELIM;
    _build(DELIM);
}

void CSV::createTable(const std::string FILENAME)
{
    _table.clear();
    _columns = 0, _rows = 0;
    _filename = FILENAME;
    _delim = ',';
    _build(',');
}

void CSV::createTable(const std::string FILENAME, const char DELIM)
{
    _table.clear();
    _columns = 0, _rows = 0;
    _filename = FILENAME;
    _delim = DELIM;
    _build(DELIM);
}

void CSV::_build(const char DELIM)
{
    std::ifstream file;
    file.open(_filename);
    int lineIdx = 0;
    std::string item = "";
    std::string line = "";

    while(!file.eof())
    {
        std::getline(file, line);
        if(file.fail()) { break; }
        lineIdx++;
        if(line.find_first_of(_delim) == std::string::npos)
        {
            std::cerr << "Error: specified delimiter not found on line " << lineIdx << std::endl;
            _columns = 0, _rows = 0;
            _table.clear();
            _header.clear();
            break;
        }
        if(lineIdx == 1)
        {
            for(size_t i = 0; i < line.length(); i++)
            {
                if(line.at(i) != DELIM)
                {
                    item += line.at(i);
                }
                else
                {
                    _columns++;
                    _header.push_back(item);
                    item = "";
                }
            }
            _columns++;
            _header.push_back(item);
            item = "";
        }
        else
        {
            _table.push_back(std::vector<std::string>());
            for(size_t i = 0; i < line.length(); i++)
            {
                if(line.at(i) != DELIM)
                {
                    item += line.at(i);
                }
                else
                {
                    _table.at(lineIdx - 2).push_back(item);
                    item = "";
                }
            }
            _table.at(lineIdx - 2).push_back(item);
            item = "";
            _rows++;
        }
    }
    file.close();
}

void CSV::_updateFile()
{
    std::ofstream file;
    file.open(_filename);
    for(size_t k = 0; k < _header.size(); k++)
    {
        file << _header.at(k);
        if(k != _header.size() - 1) { file << _delim; }
    }
    file << "\n";
    for(size_t i = 0; i < _table.size(); i++)
    {
        for(size_t j = 0; j < _table.at(i).size(); j++)
        {
            file << _table.at(i).at(j);
            if(j != _header.size() - 1) { file << _delim; }
        }
        file << "\n";
    }
    file << std::flush;
    file.close();
}

void CSV::setDelim(const char DELIM)
{
    _delim = DELIM;
    _updateFile();
}

void CSV::setItem(const std::string ITEM, const int ROW, const int COL)
{
    if(errorCheck(ROW, COL))
    {
        _table.at(ROW - 1).at(COL - 1) = ITEM;
        _updateFile();
    }
}

std::string CSV::getItem(const int ROW, const int COL)
{
    if(errorCheck(ROW, COL))
    {
        return _table.at(ROW - 1).at(COL - 1);
    }
    return "Error";
}

std::vector<std::string> CSV::getRow(const int ROW)
{
    if(errorCheck(ROW, _columns))
    {
        return _table.at(ROW - 1);
    }
    return std::vector<std::string>();
}

std::vector<std::string> CSV::getCol(const int COL)
{
    std::vector<std::string> tempVec;
    if(errorCheck(_rows, COL))
    {
        for(int i = 0; i < _rows; i++)
        {
            tempVec.push_back(_table.at(i).at(COL - 1));
        }
    }
    return tempVec;
}

bool CSV::searchForItem(const std::string ITEM) const
{
    for(size_t i = 0; i < _table.size(); i++)
    {
        for(size_t j = 0; j < _table.at(i).size(); j++)
        {
            if(_table.at(i).at(j) == ITEM)
            {
                return true;
            }
        }
    }
    return false;
}

void CSV::pushRow(const std::vector<std::string> ROW)
{
    if(ROW.size() == (size_t)_columns)
    {
        _table.push_back(ROW);
        _rows++;
        _updateFile();
    }
    else
    {
        std::cerr << "Error: row provided is not appropriate size" << std::endl;
    }
}

void CSV::pushCol(const std::vector<std::string> COL)
{
    if(COL.size() == (size_t)_rows + 1)
    {
        _header.push_back(COL.at(0));
        for(int i = 0; i < _rows; i++)
        {
            _table.at(i).push_back(COL.at(i + 1));
        }
        _columns++;
        _updateFile();
    }
    else
    {
        std::cerr << "Error: column provided is not appropriate size\nFirst index must be column title" << std::endl;
    }
}

bool CSV::errorCheck(const int ROW, const int COL) const
{
    if(_columns == 0 || _rows == 0)
    {
        std::cerr << "Error: no table created or table is empty" << std::endl;
        return false;
    }
    else if(ROW > _rows || COL > _columns)
    {
        std::cerr << "Error: out of range value accessed" << std::endl;
        return false;
    }
    return true;
}
