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
    float m_Mass;
    float m_Location[3];
    float m_Velocity[3];
    float m_N1;
    int   m_N2;
};

class Star
{
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

    fileStream.close();
}

extern "C" void Fill(int* Cnt, float* LVM)
{
    *Cnt = fileHeader.m_Count;

    for(int i = 0; i < fileHeader.m_DarkCnt; ++i)
    {
        *LVM++ = Darks[i].m_Location[0];
        *LVM++ = Darks[i].m_Location[1];
        *LVM++ = Darks[i].m_Location[2];
        *LVM++ = Darks[i].m_Velocity[0];
        *LVM++ = Darks[i].m_Velocity[1];
        *LVM++ = Darks[i].m_Velocity[2];
        *LVM++ = Darks[i].m_Mass;
    }

    for(int i = 0; i < fileHeader.m_StarCnt; ++i)
    {
        *LVM++ = Stars[i].m_Location[0];
        *LVM++ = Stars[i].m_Location[1];
        *LVM++ = Stars[i].m_Location[2];
        *LVM++ = Stars[i].m_Velocity[0];
        *LVM++ = Stars[i].m_Velocity[1];
        *LVM++ = Stars[i].m_Velocity[2];
        *LVM++ = Stars[i].m_Mass;
    }
}