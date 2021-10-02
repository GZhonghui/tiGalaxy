#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>

#define BUILD_FOR_WINDOWS
#ifdef BUILD_FOR_WINDOWS
#define LOADER_API __declspec(dllexport)
#else
#define LOADER_API
#endif

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

inline bool endsWith(std::string const& Value, std::string const& Ending)
{
    if (Ending.size() > Value.size()) return false;
    return std::equal(Ending.rbegin(), Ending.rend(), Value.rbegin());
}

extern "C" LOADER_API void Reset()
{
    Darks.clear();
    Stars.clear();
}

extern "C" LOADER_API void Load(const char* fileName, int* Cnt)
{
    Reset();
    *Cnt = 0;

    std::string filePath = std::string("./DATA/") + fileName;

    if (endsWith(fileName, "bin"))
    {
        std::ifstream fileStream(filePath.c_str(), std::ios::in | std::ios::binary);

        if (!fileStream.is_open()) return;

        fileStream.read((char*)&fileHeader, sizeof(Info));

        Darks.resize(fileHeader.m_DarkCnt);
        Stars.resize(fileHeader.m_StarCnt);

        for (int i = 0; i < fileHeader.m_DarkCnt; ++i)
        {
            fileStream.read((char*)&Darks[i], sizeof(Dark));
        }

        for (int i = 0; i < fileHeader.m_StarCnt; ++i)
        {
            fileStream.read((char*)&Stars[i], sizeof(Star));
        }

        fileStream.close();
    }
    else if (endsWith(fileName, "tab"))
    {
        std::ifstream fileStream(filePath.c_str(), std::ios::in);

        if (!fileStream.is_open()) return;

        std::string Buffer;

        fileHeader.m_Count = 0;
        fileHeader.m_DarkCnt = 0;
        fileHeader.m_StarCnt = 0;

        Star newStar;

        while (!fileStream.eof())
        {
            getline(fileStream, Buffer);
            std::istringstream bufferStream(Buffer.c_str());

            bufferStream
                >> newStar.m_Mass
                >> newStar.m_Location[0]
                >> newStar.m_Location[1]
                >> newStar.m_Location[2]
                >> newStar.m_Velocity[0]
                >> newStar.m_Velocity[1]
                >> newStar.m_Velocity[2];

            Stars.push_back(newStar);

            fileHeader.m_Count += 1;
        }

        fileHeader.m_StarCnt = fileHeader.m_Count;

        fileStream.close();
    }
    else if (endsWith(fileName, "dat"))
    {
        std::ifstream fileStream(filePath.c_str(), std::ios::in);

        if (!fileStream.is_open()) return;

        std::string Buffer;

        fileHeader.m_Count = 0;
        fileHeader.m_DarkCnt = 0;
        fileHeader.m_StarCnt = 0;

        Star newStar;

        // Hard Code
        newStar.m_Mass = 0.001;

        while (!fileStream.eof())
        {
            getline(fileStream, Buffer);
            std::istringstream bufferStream(Buffer.c_str());

            bufferStream
                >> newStar.m_Location[2]
                >> newStar.m_Location[1]
                >> newStar.m_Location[0]
                >> newStar.m_Velocity[2]
                >> newStar.m_Velocity[1]
                >> newStar.m_Velocity[0];

            // Hard Code
            newStar.m_Location[0] *= 10;
            newStar.m_Location[1] *= 10;
            newStar.m_Location[2] *= 10;
            newStar.m_Velocity[0] *= 10;
            newStar.m_Velocity[1] *= 10;
            newStar.m_Velocity[2] *= 10;

            Stars.push_back(newStar);

            fileHeader.m_Count += 1;
        }

        fileHeader.m_StarCnt = fileHeader.m_Count;

        fileStream.close();
    }
    else if (endsWith(fileName, "snap"))
    {
        std::ifstream fileStream(filePath.c_str(), std::ios::in);

        if (!fileStream.is_open()) return;

        std::string Buffer;

        fileHeader.m_Count = 0;
        fileHeader.m_DarkCnt = 0;
        fileHeader.m_StarCnt = 0;

        getline(fileStream, Buffer);
        std::istringstream bufferStream(Buffer.c_str());
        bufferStream >> fileHeader.m_Count;

        fileHeader.m_StarCnt = fileHeader.m_Count;
        Stars.resize(fileHeader.m_Count);

        int   N1;
        float N2;

        getline(fileStream, Buffer);
        bufferStream.clear();
        bufferStream.str(Buffer.c_str());
        bufferStream >> N1;

        getline(fileStream, Buffer);
        bufferStream.clear();
        bufferStream.str(Buffer.c_str());
        bufferStream >> N2;

        for (int i = 0; i < fileHeader.m_Count; ++i)
        {
            getline(fileStream, Buffer);
            bufferStream.clear();
            bufferStream.str(Buffer.c_str());

            bufferStream >> Stars[i].m_Mass;
        }

        for (int i = 0; i < fileHeader.m_Count; ++i)
        {
            getline(fileStream, Buffer);
            bufferStream.clear();
            bufferStream.str(Buffer.c_str());

            bufferStream
                >> Stars[i].m_Location[0]
                >> Stars[i].m_Location[1]
                >> Stars[i].m_Location[2];
        }

        for (int i = 0; i < fileHeader.m_Count; ++i)
        {
            getline(fileStream, Buffer);
            bufferStream.clear();
            bufferStream.str(Buffer.c_str());

            bufferStream
                >> Stars[i].m_Velocity[0]
                >> Stars[i].m_Velocity[1]
                >> Stars[i].m_Velocity[2];
        }

        fileStream.close();
    }
    else return;

    *Cnt = fileHeader.m_Count;
}

extern "C" LOADER_API void Fill(float* LVM)
{
    for (int i = 0; i < fileHeader.m_DarkCnt; ++i)
    {
        *LVM++ = Darks[i].m_Location[0];
        *LVM++ = Darks[i].m_Location[1];
        *LVM++ = Darks[i].m_Location[2];
        *LVM++ = Darks[i].m_Velocity[0];
        *LVM++ = Darks[i].m_Velocity[1];
        *LVM++ = Darks[i].m_Velocity[2];
        *LVM++ = Darks[i].m_Mass;
    }

    for (int i = 0; i < fileHeader.m_StarCnt; ++i)
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