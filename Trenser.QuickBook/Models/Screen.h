#pragma once
#include <string>
#include <vector>

class Theatre;
class Seat;

class Screen
{
private:
    std::string m_screenId;
    Theatre* m_theatre;
    std::string m_name;
    int m_totalRows;
    int m_totalColumns;
    std::vector<std::vector<Seat*>> m_seatGrid;
    int m_screenStatus; // enum placeholder
public:
    Screen();
    Screen(const std::string& screenId,
        Theatre* theatre,
        const std::string& name,
        int totalRows,
        int totalColumns,
        const std::vector<std::vector<Seat*>>& seatGrid,
        int screenStatus);
    const std::string& getScreenId() const;
    Theatre* getTheatre() const;
    const std::string& getName() const;
    int getTotalRows() const;
    int getTotalColumns() const;
    const std::vector<std::vector<Seat*>>& getSeatGrid() const;
    int getScreenStatus() const;
    void setScreenId(const std::string& screenId);
    void setTheatre(Theatre* theatre);
    void setName(const std::string& name);
    void setTotalRows(int totalRows);
    void setTotalColumns(int totalColumns);
    void setSeatGrid(const std::vector<std::vector<Seat*>>& seatGrid);
    void setScreenStatus(int screenStatus);
};


