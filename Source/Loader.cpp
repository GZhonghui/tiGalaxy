#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>

class Dark
{
public:
    Dark() = default;
    virtual ~Dark() = default;

public:
    float m_Mass;
    float m_Location[3];
    float m_Velocity[3];
    float m_N1;
    int   m_N2;
};

class Star
{
public:
    Star() = default;
    virtual ~Star() = default;

public:
    float m_Mass;
    float m_Location[3];
    float m_Velocity[3];
    float m_N1[3];
    int   m_N2;
};

class Info
{
public:
    Info() = default;
    virtual ~Info() = default;

public:
    double m_N1;
    int    m_Count;
    int    m_N2[2];
    int    m_DarkCnt;
    int    m_StarCnt;
};

std::vector<Dark> Darks;
std::vector<Star> Stars;

Info fileHeader;

extern "C" void Reset()
{
    Darks.clear();
    Stars.clear();
}

extern "C" void Load(const char* fileName)
{
    Reset();

    std::string filePath = std::string("./DATA/") + fileName;
    std::ifstream fileStream(filePath.c_str(), std::ios::in | std::ios::binary);

    if(!fileStream.is_open()) return;

    fileStream.read((char*)&fileHeader, sizeof(Info));

    Darks.resize(fileHeader.m_DarkCnt);
    Stars.resize(fileHeader.m_StarCnt);

    for(int i = 0; i < fileHeader.m_DarkCnt; ++i)
    {
        fileStream.read((char*)&Darks[i], sizeof(Dark));
    }

    for(int i = 0; i < fileHeader.m_StarCnt; ++i)
    {
        fileStream.read((char*)&Stars[i], sizeof(Star));
    }

    printf("%d %d\n",fileHeader.m_DarkCnt, fileHeader.m_StarCnt);

    fileStream.close();
}

extern "C" void Fill()
{

}
