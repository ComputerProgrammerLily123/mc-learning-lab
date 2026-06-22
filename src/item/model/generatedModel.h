#pragma once
#include "itemModelBase.h"
class GeneratedModel : public ItemModelBase
{
public:
    std::string GetTexture(const std::string& layer = "layer0") const override;
};