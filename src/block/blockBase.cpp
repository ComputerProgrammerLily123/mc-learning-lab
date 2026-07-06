#include "blockBase.h"
unsigned BlockBase::GetID() const
{
    return id;
}
const std::string BlockBase::GetName() const
{
    return name;
}
const std::array<int, FACE_COUNT> BlockBase::GetUVOffsets() const
{
    return uvOffsets;
}
const std::vector<std::string> BlockBase::GetTextures() const
{
    return textures;
}
bool BlockBase::GetSolid() const
{
    return isSolid;
}
void BlockBase::SetID(unsigned id)
{
    this->id = id;
}
void BlockBase::SetName(const std::string &name)
{
    this->name = name;
}
void BlockBase::SetUVOffsets(const std::array<int, FACE_COUNT> &uvOffsets)
{
    this->uvOffsets = uvOffsets;
}
void BlockBase::SetTextures(const std::vector<std::string> &textures)
{
    this->textures = textures;
}
void BlockBase::SetSolid(bool isSolid)
{
    this->isSolid = isSolid;
}