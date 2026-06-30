#pragma once
#include "rectTransform.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
class UIElement
{
public:
    UIElement(PivotType pivotType, FlexType flexType, float x, float y, float width = 100.0f, float height = 100.0f);
    glm::vec2 GetStartPointPosition();
    glm::vec2 GetCenterPosition();
    void SetCenterPosition(float x, float y);
    glm::vec2 GetSize();
    void SetSize(float width, float height);
    void AddElement(UIElement *uiElement);
    void ClearElement();
    std::vector<UIElement *> &GetChildElement();

    virtual glm::vec4 GetUV() { return glm::vec4(0, 0, 0, 0); }
    virtual std::string GetTextureID() { return ""; }
    virtual void OnPointerEnter(){}
    virtual void OnPointerExit(){}
    virtual void OnPointerClick(){}
    
protected:
    RectTransform rectTransform;
    std::vector<UIElement *> childElements;
};