#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <random>

class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            // This checks for a trailing comma with no data after it.
            if (!lineStream && cell.empty())
            {
                // If there was a trailing comma then add an empty element.
                m_data.push_back("");
            }
        }
    private:
        std::vector<std::string>    m_data;
};

//Overload the >> operator
std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

int main( int argc,  char** argv )
{
    std::ifstream       file("Bible-Graph.csv");
    CSVRow              row;
    int                 count = 0;
    bool                reading = true;
    std::string         answer = "";

    if (!file.is_open()){
        return -1;
    }

    std::vector<CSVRow> verses;
    
    while(file >> row)
    {
        count++;
        verses.push_back(row);
    }

    //seed the random number generator
    std::random_device seeder;
    std::mt19937 rng(seeder());
    

    //random int for random book
    std::uniform_int_distribution<int> dist(1, count);
    int book = dist(rng);

    //random int for random chapter
    int no_chapters = std::stoi(std::regex_replace(verses[book][1], std::regex(R"([^\d])"), ""));
    std::uniform_int_distribution<int> dist2(1, no_chapters);
    int r_chapter = dist2(rng);

    //random int for random verse
    int no_verses = std::stoi(std::regex_replace(verses[book][r_chapter + 1], std::regex(R"([^\d])"), ""));
    std::uniform_int_distribution<int> dist3(1, no_verses);
    int r_verse = dist3(rng);

    std::string book_name =  std::regex_replace(verses[book][0], std::regex(R"([^A-Za-z\d\s+])"), "");

    while(reading){
        std::cout << book_name << " " << r_chapter << ":" << r_verse << "\n";

        std::cout << "Are you finished? y or n \n";

        getline(std::cin, answer);
        if(answer == "y"){
            reading = false;
        }else if(answer == "n"){
            reading = true;
        }else{
            std::cout << "Please press y or n and press enter.\n";
        }
        
    }
}
