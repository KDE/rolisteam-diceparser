/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
 *                                                                          *
 *  This file is part of DiceParser                                         *
 *                                                                          *
 * DiceParser is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the                            *
 * Free Software Foundation, Inc.,                                          *
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
 ***************************************************************************/
#include "result.h"
#include <QUuid>

Result::Result()
    : m_resultTypes(static_cast<int>(Dice::RESULT_TYPE::NONE))
    , m_id(QString("\"%1\"").arg(QUuid::createUuid().toString()))
    , m_previous(nullptr)
{
}
Result::~Result() {}

Result* Result::getPrevious() const
{
    return m_previous;
}

void Result::setPrevious(Result* p)
{
    Q_ASSERT(p != this);
    m_previous= p;
}

bool Result::isStringResult() const
{
    return false;
}
void Result::clear() {}
bool Result::hasResultOfType(Dice::RESULT_TYPE type) const
{
    return (m_resultTypes & static_cast<int>(type));
}
void Result::generateDotTree(QString& s)
{
    auto str= toString(true);
    if(s.contains(str))
        return;
    s.append(str);
    s.append(";\n");

    if(nullptr != m_previous)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_previous->toString(false));
        s.append("[label=\"previousResult\"]\n");
        m_previous->generateDotTree(s);
    }
    else
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append("nullptr");
        s.append(" [label=\"previousResult\"];\n");
    }
}

QString Result::getId() const
{
    return m_id;
}

QString Result::getStringResult() const
{
    return {};
}
