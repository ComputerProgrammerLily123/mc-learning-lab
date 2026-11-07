#include "resourceLocation.h"

ResourceLocation::ResourceLocation(const std::string& namespace_, const std::string& path_)
{
    if(!IsValidNamespace(namespace_) || !IsValidPath(path_))
    {
        this->namespace_ = "minecraft";
        this->path_ = "none";
        return;
    }
    this->namespace_ = namespace_;
    this->path_ = path_;
}
ResourceLocation::ResourceLocation(const std::string& path_)
{
    auto pos = path_.find_first_of(':');
    if(pos != path_.npos)
    {
        this->namespace_ = path_.substr(0, pos);
        this->path_ = path_.substr(pos + 1);
        if(!IsValidNamespace(this->namespace_) || !IsValidPath(this->path_))
        {
            this->namespace_ = "minecraft";
            this->path_ = "none";
        }
        return;
    }
    if(!IsValidPath(path_))
    {
        this->namespace_ = "minecraft";
        this->path_ = "none";
        return;
    }
    this->namespace_ = "minecraft";
    this->path_ = path_;
}
const std::string& ResourceLocation::GetNamespace() const noexcept
{
    return namespace_;
}
const std::string& ResourceLocation::GetPath() const noexcept
{
    return path_;
}
bool ResourceLocation::operator==(const ResourceLocation& other) const
{
    return namespace_ == other.GetNamespace() && path_ == other.GetPath();
}
bool ResourceLocation::operator!=(const ResourceLocation& other) const
{
    return namespace_ != other.GetNamespace() || path_ != other.GetPath();
}
bool ResourceLocation::IsValidNamespace(const std::string& value)
{
    if (value.empty())
    {
        return false;
    }

    for (char c : value)
    {
        const bool isLowercase = c >= 'a' && c <= 'z';
        const bool isDigit = c >= '0' && c <= '9';
        const bool isAllowedSymbol = c == '_' || c == '-' || c == '.';

        if (!isLowercase && !isDigit && !isAllowedSymbol)
        {
            return false;
        }
    }

    return true;
}
bool ResourceLocation::IsValidPath(const std::string& value)
{
    if (value.empty() || value.front() == '/' || value.back() == '/')
    {
        return false;
    }

    std::size_t segmentStart = 0;

    while (segmentStart < value.size())
    {
        const std::size_t slash = value.find('/', segmentStart);
        const std::size_t segmentEnd = slash == std::string::npos ? value.size() : slash;

        const std::string& segment = value.substr(segmentStart, segmentEnd - segmentStart);

        // Reject "//", "/./", and "/../".
        if (segment.empty() || segment == "." || segment == "..")
        {
            return false;
        }

        for (char c : segment)
        {
            const bool isLowercase = c >= 'a' && c <= 'z';
            const bool isDigit = c >= '0' && c <= '9';
            const bool isAllowedSymbol = c == '_' || c == '-' || c == '.';

            if (!isLowercase && !isDigit && !isAllowedSymbol)
            {
                return false;
            }
        }

        if (slash == std::string::npos)
        {
            break;
        }

        segmentStart = slash + 1;
    }

    return true;
}