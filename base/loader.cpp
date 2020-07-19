#include <fstream>
#include <vector>
#include "loader.h"
#include "func_tool.h"

using namespace std;

int to_int(const string& chaine) { return strtol(chaine.c_str(), NULL, 10); }

Loader::Loader(const string& file_path)
{
    ifstream file(file_path.c_str());
    if (!file)
    {
        std::cerr << file_path << " : file not found!";
        exit(EXIT_FAILURE);
    }

    cout << "Loading map configuration ...\n";
    string buffer;
    vector<string> lignes;
    while (getline(file, buffer))
        lignes.push_back(buffer);

    world_x = to_int(lignes[1].substr(6));
    world_y = to_int(lignes[2].substr(7));
    tile_w = to_int(lignes[3].substr(10));
    tile_h = to_int(lignes[4].substr(11));
    int i_line(7);
    string chaine(lignes[i_line]);
    while (chaine.find("tileset") != chaine.npos)
    {
        m_tileset.push_back(split(split(lignes[i_line], "=")[1], ",")[0]);
        ++i_line;
        chaine = lignes[i_line];
    }
    vector< vector<int> > _layers;
    while (true)
    {
        i_line += 4;
        if (i_line < (int)lignes.size())
        {
            _layers.push_back(vector<int>());
            while (lignes[i_line] != "")
            {
                vector<string> ligne(split(lignes[i_line], ","));
                for (int i=0; i<world_x; ++i)
                    _layers.back().push_back(to_int(ligne[i]));
                i_line ++;
            }
        }
        else
            break;
    }
    m_map.resize(world_y);
    for (int i=0; i<world_y; ++i)
        m_map[i].resize(world_x, 0);
    for (int k=0; k<(int)_layers.size(); ++k)
    {
        vector< vector<int> > tmp;
        for (int i=0; i<world_y; ++i)
        {
            tmp.push_back(vector<int>());
            for (int j=0; j<world_x; ++j)
            {
                int index(i*world_x+j);
                if (!m_map[i][j])
                    m_map[i][j] = _layers[k][index];
                tmp[tmp.size()-1].push_back(_layers[k][index]);
            }
        }
        layers.push_back(tmp);
    }

}
