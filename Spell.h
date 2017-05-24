#pragma once
#include "Elements.h"

class Spell
{
	Element spell_element;
public:
	Spell() {};
	Spell(Element e) : spell_element(e) {};
	Element get_element() { return spell_element; }
};

